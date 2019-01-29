#include "lexcmd.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../lexer.h"
#include "../util.h"

static int _is_delimiter(int chr) {
	return chr == -1 || !isalpha(chr) || chr != '_';
}

/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	*abandon = 1;

	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, COMMON_LEXCMD_TOKEN_SPACE);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, COMMON_LEXCMD_TOKEN_LITERAL_UINT_DEC);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (_is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“"”。
	if (chr == '"') {
		lexer_init_token(ctx, &token, COMMON_LEXCMD_TOKEN_LITERAL_STRING);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '"' && chr != -1, {
				// MATCHED
				if (chr == '\\') {
					LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_esc(chr), {
						// MATCHED
					}, {
						// NOT MATCHED
						return NULL;
					})
				}
			}, {
				// NOT MATCHED
				if (chr == -1) {
					return NULL;
				}

				lexer_next_char(ctx);
				token.len++;
				return lexer_clone_token(ctx, &token);
			})
		}
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获关键字。
*/
LEXER_MATCHER(keyword)
	*abandon = 0;

	LexerToken token;

	#define	_KEYWORD(text, type)	{(text), sizeof((text)) - 1, (type)}

	static struct _Keyword {
		const char	*text;
		const int	len;
		const int	type;
	} keywords[] = {
		_KEYWORD("#line", COMMON_LEXCMD_TOKEN_KEYWORD_LINE),

		{NULL, 0, COMMON_LEXCMD_TOKEN_INVALID}
	};

	#undef	_KEYWORD

	char chr = lexer_next_char(ctx);

	if (chr == '#') {
		lexer_init_token(ctx, &token, COMMON_LEXCMD_TOKEN_INVALID);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				break;
			})
		}

		for (struct _Keyword *keyword = keywords; keyword->text != NULL; keyword++) {
			if (keyword->len == token.len
					&& strncmp(keyword->text, token.content, keyword->len) == 0) {
				token.type = keyword->type;
				break;
			}
		}

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(space),

	_MATCHER_LIST_ITEM(literal_uint_dec),

	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(keyword),

	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM

LexerContext * common_lexcmd_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	return lexer_new_context(file, source, len, _matchers);
}

void common_lexcmd_free_context(LexerContext *ctx) {
	assert(ctx);

	lexer_free_context(ctx);
}

uint32_t common_lexcmd_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case COMMON_LEXCMD_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint32_dec_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}
