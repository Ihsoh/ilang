#include "comment.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "../../string.h"
#include "../../lexer.h"
#include "../../util.h"

#define	_TOKEN_TEXT						0x1000
#define	_TOKEN_LITERAL_CHAR				0x1001
#define	_TOKEN_LITERAL_STRING			0x1002

#define	_TOKEN_SINGLE_LINE_COMMENT		0xe000
#define	_TOKEN_MULTILINE_COMMENT		0xe001




/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“'”。
	if (chr == '\'') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_CHAR);

		// 捕获了一个不是“'”的字符。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		// 如果“'”之后的第一个字符是“\”，则继续捕获一个字符。
		if (chr == '\\') {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_char_esc(chr), {
				// MATCHED
			}, {
				// NOT MATCHED
				return NULL;
			})
		} else {
			if (!util_is_literal_char_item(chr)) {
				return NULL;
			}
		}

		// 捕获“'”，字符字面量必须以“'”结束。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		return lexer_clone_token(ctx, &token);
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
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_STRING);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_item(chr) && chr != '"' && chr != -1, {
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
	捕获单行注释。
*/
LEXER_MATCHER(single_line_comment)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, _TOKEN_SINGLE_LINE_COMMENT);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '/', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
				// MATCHED
			}, {
				// NOT MATCHED
				return lexer_clone_token(ctx, &token);
			})
		}

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	多行注释。
*/
LEXER_MATCHER(multiline_comment)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, _TOKEN_MULTILINE_COMMENT);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '*', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '*', {
				// MATCHED
				if (chr == -1) {
					return NULL;
				}
			}, {
				// NOT MATCHED
				LEXER_NEXT_CHAR(ctx, &token)
				LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '/', {
					// MATCHED
					return lexer_clone_token(ctx, &token);
				}, {
					// NOT MATCHED
					if (chr == -1) {
						return NULL;
					}
				})
			})
		}

		return NULL;
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	普通文本。
*/
LEXER_MATCHER(text)
	*abandon = 0;

	LexerToken token;

	lexer_next_char(ctx);

	lexer_init_token(ctx, &token, _TOKEN_TEXT);

	char chr;
	for (;;) {
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\'' && chr != '"' && !(chr == '/' && lexer_peek_char(ctx, 2) == '/') && !(chr == '/' && lexer_peek_char(ctx, 2) == '*') && chr != -1,
		{
			// MATCHED
		}, {
			// NOT MATCHED
			return lexer_clone_token(ctx, &token);
		})
	}
LEXER_MATCHER_END



#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(literal_char),
	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(single_line_comment),
	_MATCHER_LIST_ITEM(multiline_comment),

	_MATCHER_LIST_ITEM(text),

	{NULL, NULL}
};

void pp_comment_process(
	ResizableString *dst,
	const char *file,
	const char *src,
	int len
) {
	assert(dst);
	assert(file);
	assert(src);

	LexerContext *ctx = lexer_new_context(file, src, len, _matchers);
	lexer_parse_first(ctx);

	LexerToken *token;
	while ((token = lexer_next_token(ctx)) != NULL) {
		switch (token->type) {
			case _TOKEN_LITERAL_CHAR:
			case _TOKEN_LITERAL_STRING:
			case _TOKEN_TEXT: {
				rstr_append_with_raw(dst, token->content, token->len);
				break;
			}
			case _TOKEN_SINGLE_LINE_COMMENT: {
				for (int i = 0; i < token->len; i++) {
					rstr_append_with_char(dst, ' ');
				}
				break;
			}
			case _TOKEN_MULTILINE_COMMENT: {
				for (int i = 0; i < token->len; i++) {
					if (token->content[i] == '\n') {
						rstr_append_with_char(dst, '\n');
					} else {
						rstr_append_with_char(dst, ' ');
					}
				}
				break;
			}
			default: {
				assert(0);
			}
		}
	}

	lexer_free_context(ctx);
}
