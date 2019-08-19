#include "parser.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stddef.h>

#include "lexer.h"
#include "util.h"
#include "common.h"




static void _default_alloc_mem_error(ParserContext *ctx) {
	assert(ctx);

	if (ctx->current_token != NULL) {
		LexerToken *token = ctx->current_token;
		fprintf(
			stderr,
			"Parser Error(%s:%d,%d): cannot allocate memory!\n",
			token->file,
			token->line,
			token->column
		);
	} else {
		fprintf(
			stderr,
			"Parser Error: cannot allocate memory!\n"
		);
	}

	exit(1);
}

static void _default_syntax_error(ParserContext *ctx) {
	assert(ctx);

	if (ctx->current_token != NULL) {
		LexerToken *token = ctx->current_token;
		fprintf(
			stderr,
			"Parser Error(%s:%d,%d): syntax error!\n",
			token->file,
			token->line,
			token->column
		);
		cmn_print_code_by_token(stderr, token, 20, 20);
	} else {
		fprintf(
			stderr,
			"Parser Error: syntax error!\n"
		);
	}

	exit(1);
}

static void _default_syntax_error_msg(
	ParserContext *ctx,
	const char *msg
) {
	assert(ctx);
	assert(msg);

	fprintf(
		stderr,
		"Parser Error: syntax error, %s\n",
		msg
	);

	exit(1);
}

static void _default_syntax_error_token_msg(
	ParserContext *ctx,
	LexerToken *token,
	const char *msg
) {
	assert(ctx);
	assert(token);
	assert(msg);

	fprintf(
		stderr,
		"Parser Error(%s:%d:%d): syntax error, token '",
		token->file,
		token->line,
		token->column
	);
	cmn_print_token(token, stderr);
	fprintf(
		stderr,
		"', %s\n",
		msg
	);

	cmn_print_code_by_token(stderr, token, 20, 20);

	exit(1);
}

static void _default_syntax_error_node_msg(
	ParserContext *ctx,
	ParserASTNode *node,
	const char *msg
) {
	assert(ctx);
	assert(msg);

	if (node->file != NULL) {
		fprintf(
			stderr,
			"Parser Error(%s:%d,%d): syntax error, %s\n",
			node->file,
			node->line,
			node->column,
			msg
		);
	} else {
		fprintf(
			stderr,
			"Parser Error: syntax error, %s\n",
			msg
		);
	}

	exit(1);
}




ParserContext * parser_new_context_with_data(
	LexerContext *lexctx,
	size_t szdata,
	void *data
) {
	assert(lexctx);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	ParserContext *ctx = (ParserContext *)malloc(sizeof(ParserContext) + szdata);
	if (ctx == NULL) {
		return NULL;
	}
	memcpy(&(ctx->data[0]), data, szdata);

	ctx->lexctx = lexctx;
	
	ctx->ast = NULL;

	ctx->nodes = NULL;

	ctx->previous_node = NULL;
	ctx->last_node = NULL;

	ctx->current_token = NULL;

	ctx->arch = 0;

	ctx->alloc_mem_error = _default_alloc_mem_error;
	ctx->syntax_error = _default_syntax_error;
	ctx->syntax_error_msg = _default_syntax_error_msg;
	ctx->syntax_error_token_msg = _default_syntax_error_token_msg;
	ctx->syntax_error_node_msg = _default_syntax_error_node_msg;

	return ctx;
}

ParserContext * parser_new_context(
	LexerContext *lexctx
) {
	return parser_new_context_with_data(lexctx, 0, NULL);
}

void parser_free_context(
	ParserContext *ctx
) {
	assert(ctx);

	ParserASTNode *node = ctx->nodes;
	while (node != NULL) {
		ParserASTNode *temp = node;
		node = node->next;
		parser_free_node(ctx, temp);
	}

	free(ctx);
}

ParserASTNode * parser_new_node(
	ParserContext *ctx,
	int type,
	char *type_name,
	LexerToken *token,
	size_t szdata,
	void *data
) {
	assert(ctx);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	ParserASTNode *node = (ParserASTNode *)malloc(sizeof(ParserASTNode) + szdata);
	if (node == NULL) {
		ctx->alloc_mem_error(ctx);
	}
	memcpy(&(node->data[0]), data, szdata);

	node->type = type;
	node->type_name = type_name;

	node->token = token;

	if (token != NULL) {
		node->file = token->file;
		node->line = token->line;
		node->column = token->column;
	} else {
		LexerToken *current_token = ctx->current_token;
		if (current_token != NULL) {
			node->file = current_token->file;
			node->line = current_token->line;
			node->column = current_token->column;
		} else {
			node->file = NULL;
			node->line = -1;
			node->column = -1;
		}
	}

	node->next = ctx->nodes;
	node->parent = NULL;

	ctx->nodes = node;

	node->symtbl = NULL;

	node->szchilds = 0;
	node->nchilds = 0;
	node->childs = NULL;

	node->releaser = NULL;

	return node;
}

void parser_free_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	if (node->releaser != NULL) {
		node->releaser(node);
	}

	if (node->childs != NULL) {
		free(node->childs);
	}

	ParserSymbol *symbol = node->symtbl;
	while (symbol != NULL) {
		ParserSymbol *temp = symbol;
		symbol = symbol->next;
		parser_free_symbol(ctx, temp);
	}

	free(node);
}

void parser_insert_node(
	ParserContext *ctx,
	ParserASTNode **pos,
	ParserASTNode *node
) {
	assert(ctx);
	assert(pos);
	assert(*pos);
	assert(node);

	node->parent = (*pos)->parent;
	node->file = (*pos)->file;
	node->line = (*pos)->line;
	node->column = (*pos)->column;

	(*pos)->parent = node;

	parser_add_child(ctx, node, *pos);
	
	*pos = node;
}

ParserASTNode * parser_add_child(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *child
) {
	assert(ctx);
	assert(node);
	assert(child);

	if (node->nchilds < node->szchilds) {
		node->childs[node->nchilds++] = child;
		return node;
	} else {
		int old_szchilds = node->szchilds;
		int old_size = old_szchilds * sizeof(ParserASTNode *);
		int new_szchilds = old_szchilds == 0 ? 1 : 2 * old_szchilds;
		int new_size = new_szchilds * sizeof(ParserASTNode *);

		ParserASTNode **new_childs = (ParserASTNode **)malloc(new_size);
		if (new_childs == NULL) {
			ctx->alloc_mem_error(ctx);
		}

		ParserASTNode **old_childs = node->childs;
		if (old_childs != NULL) {
			memcpy(new_childs, old_childs, old_size);
			free(old_childs);
		}

		node->childs = new_childs;
		node->childs[node->nchilds++] = child;
		node->szchilds = new_szchilds;

		return node;
	}
}

ParserSymbol * parser_new_symbol(
	ParserContext *ctx,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
) {
	assert(ctx);
	assert(type != 0);
	assert(token);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	ParserSymbol *symbol = (ParserSymbol *)malloc(sizeof(ParserSymbol) + szdata);
	if (symbol == NULL) {
		ctx->alloc_mem_error(ctx);
	}

	symbol->type = type;
	symbol->token = token;
	symbol->next = NULL;
	symbol->symtbl = NULL;

	symbol->releaser = NULL;

	symbol->szdata = szdata;
	memcpy(&(symbol->data[0]), data, szdata);

	return symbol;
}

ParserSymbol * parser_clone_symbol(
	ParserContext *ctx,
	ParserSymbol *source
) {
	assert(ctx);
	assert(source);

	size_t size = sizeof(ParserSymbol) + source->szdata;

	ParserSymbol *symbol = (ParserSymbol *)malloc(size);
	if (symbol == NULL) {
		ctx->alloc_mem_error(ctx);
	}

	memcpy(symbol, source, size);

	return symbol;
}

static ParserSymbol * _parser_add_symbol(
	ParserContext *ctx,
	ParserSymbol **symtbl,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
) {
	assert(ctx);
	assert(symtbl);
	assert(type != 0);
	assert(token);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	ParserSymbol *symbol = parser_new_symbol(ctx, type, token, szdata, data);

	symbol->next = *symtbl;

	*symtbl = symbol;

	return symbol;
}

ParserSymbol * parser_add_symptr_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserSymbol *symptr
) {
	assert(ctx);
	assert(node);
	assert(symptr);

	if (parser_get_symbol_from_node(ctx, node, 0, symptr->token)) {
		ctx->syntax_error_token_msg(
			ctx,
			symptr->token,
			"redefined symbol."
		);
	}

	symptr->next = node->symtbl;

	node->symtbl = symptr;

	return symptr;
}

ParserSymbol * parser_add_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
) {
	assert(ctx);
	assert(node);
	assert(type != 0);
	assert(token);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	if (parser_get_symbol_from_node(ctx, node, 0, token)) {
		ctx->syntax_error_token_msg(
			ctx,
			token,
			"redefined symbol."
		);
	}

	return _parser_add_symbol(ctx, &(node->symtbl), type, token, szdata, data);
}

static ParserSymbol * _parser_get_symbol(
	ParserContext *ctx,
	ParserSymbol *symtbl,
	int type,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	ParserSymbol *symbol = symtbl;
	while (symbol) {
		LexerToken *t = symbol->token;

		if (lexer_compare_token(token, t)
				&& (type == 0 || type == symbol->type)) {
			return symbol;
		}

		symbol = symbol->next;
	}

	return NULL;
}

static int _parser_get_symbol_index(
	ParserContext *ctx,
	ParserSymbol *symtbl,
	int type,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	// TODO: 因为符号是从链表的前方插入。。。这就导致符号的顺序是反过来的，
	//		最尾部的符号才是第一个符号。。。
	int size = 0;
	ParserSymbol *symbol = symtbl;
	while (symbol) {
		symbol = symbol->next;
		size++;
	}

	int index = 0;

	symbol = symtbl;
	while (symbol) {
		LexerToken *t = symbol->token;

		if (lexer_compare_token(token, t)
				&& (type == 0 || type == symbol->type)) {
			return size - index - 1;
		}

		symbol = symbol->next;
		index++;
	}

	assert(0);
	return -1;
}

ParserSymbol * parser_get_symbol_from_node(
	ParserContext *ctx,
	ParserASTNode *node,
	int type,
	LexerToken *token
) {
	assert(ctx);
	assert(node);
	assert(token);

	ParserSymbol *symbol = _parser_get_symbol(ctx, node->symtbl, type, token);
	if (symbol != NULL) {
		return symbol;
	} else {
		if (node->parent != NULL) {
			return parser_get_symbol_from_node(ctx, node->parent, type, token);
		} else {
			return NULL;
		}
	}
}

void parser_free_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol
) {
	assert(ctx);
	assert(symbol);

	ParserSymbol *sym = symbol->symtbl;
	while (sym) {
		ParserSymbol *temp = sym->next;
		if (sym->releaser != NULL) {
			sym->releaser(sym);
		}
		free(sym);
		sym = temp;
	}

	if (symbol->releaser != NULL) {
		symbol->releaser(symbol);
	}
	free(symbol);
}

ParserSymbol * parser_add_symptr_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	ParserSymbol *symptr
) {
	assert(ctx);
	assert(symbol);
	assert(symptr);

	if (_parser_get_symbol(ctx, symbol->symtbl, 0, symptr->token)) {
		ctx->syntax_error_token_msg(
			ctx,
			symptr->token,
			"redefined symbol."
		);
	}

	symptr->next = symbol->symtbl;

	symbol->symtbl = symptr;

	return symptr;
}

ParserSymbol * parser_add_symbol_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
) {
	assert(ctx);
	assert(symbol);
	assert(type != 0);
	assert(token);
	if (szdata > 0) {
		assert(data != NULL);
	} else {
		assert(data == NULL);
	}

	if (_parser_get_symbol(ctx, symbol->symtbl, 0, token)) {
		ctx->syntax_error_token_msg(
			ctx,
			token,
			"redefined symbol."
		);
	}

	return _parser_add_symbol(ctx, &(symbol->symtbl), type, token, szdata, data);
}

ParserSymbol * parser_get_symbol_from_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token
) {
	assert(ctx);
	assert(symbol);
	assert(token);

	return _parser_get_symbol(ctx, symbol->symtbl, type, token);
}

int parser_get_symbol_index_from_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token
) {
	assert(ctx);
	assert(symbol);
	assert(token);

	return _parser_get_symbol_index(ctx, symbol->symtbl, type, token);
}
