#include "lexer.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "array.h"
#include "common.h"

static void _add_token(LexerContext *ctx, LexerToken *token) {
	assert(ctx);
	assert(token);
	assert((ctx->head == NULL && ctx->tail == NULL) || (ctx->head != NULL && ctx->tail != NULL));

	if (ctx->current == NULL) {
		ctx->current = token;
		token->prev = NULL;
		token->next = NULL;
	} else {
		ctx->current->next = token;
		token->prev = ctx->current;
		token->next = NULL;
		ctx->current = token;
		ctx->tail = token;
	}

	if (ctx->head == NULL && ctx->tail == NULL) {
		ctx->head = ctx->current;
		ctx->tail = ctx->current;
	}
}

static void _store_char_context(LexerContext *ctx) {
	assert(ctx);

	ctx->stored_index = ctx->index;
	ctx->stored_line = ctx->line;
	ctx->stored_column = ctx->column;
}

static void _resume_char_context(LexerContext *ctx) {
	assert(ctx);

	ctx->index = ctx->stored_index;
	ctx->line = ctx->stored_line;
	ctx->column = ctx->stored_column;
}




static void _default_invalid_token(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->file);

	fprintf(
		stderr,
		"Lexer Error(%s:%d,%d): invalid token, near:\n",
		ctx->file, ctx->line, ctx->column
	);

	fputc('\n', stderr);
	fputc('\t', stderr);

	bool flag = true;
	for (int i = 0; i < 60 && ctx->index + i < ctx->len; i++) {
		char chr = ctx->source[ctx->index + i];
		if (isspace(chr)) {
			if (flag) {
				fputc(' ', stderr);
				flag = false;
			}
		} else {
			fputc(chr, stderr);
			flag = true;
		}
	}

	fputc('\n', stderr);
	fputc('\n', stderr);
	exit(1);
}

static void _default_alloc_mem_error(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->file);

	fprintf(stderr, "Lexer Error(%s:%d,%d): cannot allocate memory!\n", ctx->file, ctx->line, ctx->column);
	exit(1);
}


LexerContext * lexer_new_context(
	const char *file,
	const char *source,
	int len,
	LexerTokenMatcher *matchers
) {
	assert(file);
	assert(source);
	assert(len >= 0);
	assert(matchers[0].name != NULL);

	LexerContext *ctx = (LexerContext *)malloc(sizeof(LexerContext));
	if (ctx == NULL) {
		return NULL;
	}

	ctx->file = file;
	rarr_init(&(ctx->rarr_file), sizeof(char *));
	
	ctx->source = source;
	ctx->len = len;
	ctx->index = -1;
	ctx->line = 1;
	ctx->column = 0;

	ctx->stored_index = -1;
	ctx->stored_line = 1;
	ctx->stored_column = 0;

	ctx->head = NULL;
	ctx->tail = NULL;
	ctx->current = NULL;

	ctx->tokens = NULL;

	ctx->curstack_top = -1;

	ctx->matchers = matchers;

	ctx->data = NULL;

	ctx->invalid_token = _default_invalid_token;
	ctx->alloc_mem_error = _default_alloc_mem_error;

	return ctx;
}

void lexer_free_context(LexerContext * ctx) {
	assert(ctx);

	for (int i = 0; i < RARR_LEN(&(ctx->rarr_file)); i++) {
		free(RARR_ELEMENT(&(ctx->rarr_file), i, char *));
	}
	rarr_free(&(ctx->rarr_file));

	LexerToken *token = ctx->tokens;
	while (token != NULL) {
		LexerToken *temp = token;
		token = token->tks_next;
		lexer_free_token(ctx, temp);
	}

	free(ctx);
}




int lexer_next_char(LexerContext *ctx) {
	assert(ctx);

	if (ctx->index + 1 < ctx->len) {
		ctx->index++;
		char chr = ctx->source[ctx->index];
		if (chr == '\n') {
			ctx->line++;
			ctx->column = 0;
		} else {
			ctx->column++;
		}
		return chr;
	} else {
		ctx->index = ctx->len;
		return -1;
	}
}

bool lexer_match_string(
	LexerContext *ctx,
	const char *str
) {
	assert(ctx);
	assert(str);

	size_t len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (lexer_peek_char(ctx, 1 + i) != str[i]) {
			return false;
		}
	}

	return true;
}

int lexer_peek_char(LexerContext *ctx, int n) {
	assert(ctx);

	int index = ctx->index + n;
	if (index < 0 || index >= ctx->len) {
		return -1;
	} else {
		return ctx->source[index];
	}
}




void lexer_reset_token(LexerContext *ctx, int head) {
	assert(ctx);

	if (head) {
		ctx->current = ctx->head;
	} else {
		ctx->current = ctx->tail;
	}
}

LexerToken * lexer_next_token(LexerContext *ctx) {
	assert(ctx);

	LexerToken *token = ctx->current;

	if (token != NULL) {
		ctx->current = token->next;
	}

	return token;
}

LexerToken * lexer_prev_token(LexerContext *ctx) {
	assert(ctx);

	LexerToken *token = ctx->current;

	if (token != NULL) {
		ctx->current = token->prev;
	}

	return ctx->current;
}

void lexer_init_token(LexerContext *ctx, LexerToken *token, int type) {
	assert(ctx);
	assert(token);
	
	token->type = type;

	token->len = 1;
	token->content = ctx->source + ctx->index;

	token->file = ctx->file;
	token->line = ctx->line;
	token->column = ctx->column;

	token->prev = NULL;
	token->next = NULL;

	token->matcher_data = NULL;
}

LexerToken * lexer_new_token(LexerContext *ctx, int type) {
	assert(ctx);

	LexerToken *token = (LexerToken *)malloc(sizeof(LexerToken));
	if (token == NULL) {
		ctx->alloc_mem_error(ctx);
	}

	lexer_init_token(ctx, token, type);

	token->tks_next = ctx->tokens;
	ctx->tokens = token;

	return token;
}

LexerToken * lexer_clone_token(LexerContext *ctx, LexerToken *token) {
	assert(ctx);
	assert(token);

	LexerToken *new_token = (LexerToken *)malloc(sizeof(LexerToken));
	if (new_token == NULL) {
		ctx->alloc_mem_error(ctx);
	}

	memcpy(new_token, token, sizeof(LexerToken));

	new_token->tks_next = ctx->tokens;
	ctx->tokens = new_token;

	return new_token;
}

void lexer_free_token(LexerContext *ctx, LexerToken *token) {
	assert(ctx);
	assert(token);

	free(token);
}

void lexer_store_current(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->curstack_top + 1 < LEXER_CONTEXT_CURSTACK_SIZE);

	ctx->curstack[++ctx->curstack_top] = ctx->current;
}

void lexer_resume_current(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->curstack_top >= 0);

	ctx->current = ctx->curstack[ctx->curstack_top--];
}

void lexer_abandon_curstack_top(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->curstack_top >= 0);

	ctx->curstack_top--;
}

void lexer_parse(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->matchers[0].name != NULL);

	const LexerTokenMatcher *matchers = ctx->matchers;

	while (ctx->index < ctx->len - 1) {
		LexerToken *token = NULL;
		int abandon = 0;
		int last_line = 0, last_column = -1;
		for (const LexerTokenMatcher *matcher = matchers; matcher->name != NULL;matcher++) {
			_store_char_context(ctx);
			int a;
			LexerToken *t = matcher->match(ctx, &a);
			if (t != NULL && (token == NULL || t->len > token->len)) {
				token = t;
				abandon = a;
				last_line = ctx->line;
				last_column = ctx->column;
			}
			_resume_char_context(ctx);
		}

		if (token != NULL) {
			ctx->index += token->len;
			ctx->line = last_line;
			ctx->column = last_column;
			if (!abandon) {
				_add_token(ctx, token);
			}
		} else {
			lexer_next_char(ctx);
			ctx->invalid_token(ctx);
		}
	}

	lexer_reset_token(ctx, 1);
}

void lexer_parse_first(LexerContext *ctx) {
	assert(ctx);
	assert(ctx->matchers[0].name != NULL);

	const LexerTokenMatcher *matchers = ctx->matchers;

	while (ctx->index < ctx->len - 1) {
		LexerToken *token = NULL;
		int abandon = 0;
		int last_line = 0, last_column = -1;
		bool finded = false;
		for (const LexerTokenMatcher *matcher = matchers; !finded && matcher->name != NULL;matcher++) {
			_store_char_context(ctx);
			int a;
			token = matcher->match(ctx, &a);
			if (token != NULL) {
				abandon = a;
				last_line = ctx->line;
				last_column = ctx->column;
				finded = true;
			}
			_resume_char_context(ctx);
		}

		if (token != NULL) {
			ctx->index += token->len;
			ctx->line = last_line;
			ctx->column = last_column;
			if (!abandon) {
				_add_token(ctx, token);
			}
		} else {
			lexer_next_char(ctx);
			ctx->invalid_token(ctx);
		}
	}

	lexer_reset_token(ctx, 1);
}

bool lexer_compare_token(
	LexerToken *a,
	LexerToken *b
) {
	assert(a);
	assert(b);

	return (a->len == b->len) && (strncmp(a->content, b->content, a->len) == 0);
}

void lexer_change_file_by_token(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	char *cstr = (char *)malloc(token->len + 1);
	memcpy(cstr, token->content, token->len);
	cstr[token->len] = '\0';

	ctx->file = cstr;
	rarr_append_with_raw(&(ctx->rarr_file), &cstr, 1);
}

void lexer_print_token_info(
	LexerToken *token,
	FILE *file
) {
	assert(token);
	assert(file);

	fputs("Name: ", file);
	cmn_print_token(token, file);
	fputc('\n', file);

	fprintf(file, "Length: %d\n", token->len);
	fprintf(file, "Type: 0x%x(%d)\n", token->type, token->type);
	fprintf(file, "File: %s\n", token->file);
	fprintf(file, "Line: %d\n", token->line);
	fprintf(file, "Column: %d\n", token->column);
}

void lexer_print_tokens(
	LexerContext * ctx,
	FILE *file
) {
	assert(ctx);
	assert(file);

	LexerToken *token = ctx->head;
	
	while (token != NULL) {
		lexer_print_token_info(token, file);
		fputc('\n', file);

		token = token->next;
	}
}
