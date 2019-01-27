#include "lexer.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "lexcmd.h"

static int _is_delimiter(int chr) {
	return chr == -1 || !isalpha(chr) || chr != '_';
}

/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	*abandon = 1;

	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, FE_TOKEN_SPACE);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获单行注释。
*/
LEXER_MATCHER(single_line_comment)
	*abandon = 1;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, FE_TOKEN_SINGLE_LINE_COMMENT);

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
	*abandon = 1;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, FE_TOKEN_MULTILINE_COMMENT);

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
	#...\n
*/
LEXER_MATCHER(preprocess)
	*abandon = 1;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '#') {
		lexer_init_token(ctx, &token, FE_TOKEN_PREPROCESS);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
				// MATCHED
			}, {
				// NOT MATCHED

				char file[1024];
				snprintf(
					file, sizeof(file),
					"LEXCMD(%s:%d,%d)",
					token.file, token.line, token.column
				);
				LexerContext *ctx_lexcmd = fe_lexcmd_new_context(
					file, token.content, token.len
				);

				lexer_parse(ctx_lexcmd);

				LexerToken *tk_cmd = lexer_next_token(ctx_lexcmd);
				assert(tk_cmd);

				if (tk_cmd->type == FE_LEXCMD_TOKEN_KEYWORD_LINE) {
					LexerToken *tk_line = lexer_next_token(ctx_lexcmd);
					if (tk_line == NULL
							|| tk_line->type != FE_LEXCMD_TOKEN_LITERAL_UINT_DEC) {
						ctx->invalid_token(ctx);
					}

					LexerToken *tk_filename = lexer_next_token(ctx_lexcmd);
					if (tk_filename != NULL
							&& tk_filename->type != FE_LEXCMD_TOKEN_LITERAL_STRING) {
						ctx->invalid_token(ctx);
					}

					uint32_t new_line = fe_lexcmd_get_uint32_val(ctx_lexcmd, tk_line);
					if (chr == '\n') {
						new_line--;
					}
					LEXER_SET_LINE(new_line)	
					if (tk_filename != NULL) {
						lexer_change_file_by_token(ctx, tk_filename);
					}				

					if (lexer_next_token(ctx_lexcmd) != NULL) {
						ctx->invalid_token(ctx);
					}
				} else {
					ctx->invalid_token(ctx);
				}

				fe_lexcmd_free_context(ctx_lexcmd);

				return lexer_clone_token(ctx, &token);
			})
		}

		assert(0);
	} else {
		return NULL;
	}	
LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
LEXER_MATCHER(literal_uint_bin)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_UINT_BIN);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'b', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1' || chr == '_', {
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
	捕获八进制数字字面量。
*/
LEXER_MATCHER(literal_uint_oct)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_UINT_OCT);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'o', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr >= '0' && chr <= '7', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '7') || chr == '_', {
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
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_UINT_DEC);

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
	捕获十六进制数字字面量。
*/
LEXER_MATCHER(literal_uint_hex)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_UINT_HEX);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'x', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		#define	_IS_HEX_DIGIT_CHAR(chr)	(((chr) >= '0' && (chr) <= '9') || ((chr) >= 'a' && (chr) <= 'f') || ((chr) >= 'A' && (chr) <= 'F'))

		LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
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

		#undef	_IS_HEX_DIGIT_CHAR

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获实型字面量。
*/
LEXER_MATCHER(literal_real)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_REAL);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == '.') {
					lexer_next_char(ctx);
					token.len++;
					break;
				} else {
					return NULL;
				}
			})
		}

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr >= '0' && chr <= '9', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

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
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“'”。
	if (chr == '\'') {
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_CHAR);

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
			// 如果字符不为字符常量所允许的字符则不匹配。
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
		lexer_init_token(ctx, &token, FE_TOKEN_LITERAL_STRING);

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
		_KEYWORD("function", FE_TOKEN_KEYWORD_FUNC),
		_KEYWORD("if", FE_TOKEN_KEYWORD_IF),
		_KEYWORD("elif", FE_TOKEN_KEYWORD_ELIF),
		_KEYWORD("else", FE_TOKEN_KEYWORD_ELSE),
		_KEYWORD("while", FE_TOKEN_KEYWORD_WHILE),
		_KEYWORD("do", FE_TOKEN_KEYWORD_DO),
		_KEYWORD("for", FE_TOKEN_KEYWORD_FOR),
		_KEYWORD("break", FE_TOKEN_KEYWORD_BREAK),
		_KEYWORD("continue", FE_TOKEN_KEYWORD_CONTINUE),
		_KEYWORD("return", FE_TOKEN_KEYWORD_RETURN),
		_KEYWORD("goto", FE_TOKEN_KEYWORD_GOTO),
		_KEYWORD("label", FE_TOKEN_KEYWORD_LABEL),
		_KEYWORD("sizeof", FE_TOKEN_KEYWORD_SIZEOF),
		_KEYWORD("cast", FE_TOKEN_KEYWORD_CAST),
		_KEYWORD("struct", FE_TOKEN_KEYWORD_STRUCT),
		_KEYWORD("var", FE_TOKEN_KEYWORD_VAR),
		_KEYWORD("asm", FE_TOKEN_KEYWORD_ASM),
		_KEYWORD("intern", FE_TOKEN_KEYWORD_INTERN),
		_KEYWORD("extern", FE_TOKEN_KEYWORD_EXTERN),
		_KEYWORD("dummy", FE_TOKEN_KEYWORD_DUMMY),
		_KEYWORD("alignof", FE_TOKEN_KEYWORD_ALIGNOF),
		_KEYWORD("align", FE_TOKEN_KEYWORD_ALIGN),
		_KEYWORD("packed", FE_TOKEN_KEYWORD_PACKED),

		_KEYWORD("char", FE_TOKEN_KEYWORD_CHAR),
		_KEYWORD("int8", FE_TOKEN_KEYWORD_INT8),
		_KEYWORD("int16", FE_TOKEN_KEYWORD_INT16),
		_KEYWORD("int32", FE_TOKEN_KEYWORD_INT32),
		_KEYWORD("int64", FE_TOKEN_KEYWORD_INT64),
		_KEYWORD("uint8", FE_TOKEN_KEYWORD_UINT8),
		_KEYWORD("uint16", FE_TOKEN_KEYWORD_UINT16),
		_KEYWORD("uint32", FE_TOKEN_KEYWORD_UINT32),
		_KEYWORD("uint64", FE_TOKEN_KEYWORD_UINT64),
		_KEYWORD("float", FE_TOKEN_KEYWORD_FLOAT),
		_KEYWORD("double", FE_TOKEN_KEYWORD_DOUBLE),

		{NULL, 0, FE_TOKEN_INVALID}
	};

	#undef	_KEYWORD

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_') {
		lexer_init_token(ctx, &token, FE_TOKEN_IDENTIFIER);

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




LEXER_TEXT_MATCHER(pnct_semicolon, ";", FE_TOKEN_PNCT_SEMICOLON, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_left, "(", FE_TOKEN_PNCT_PARENTHESES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_right, ")", FE_TOKEN_PNCT_PARENTHESES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_colon, ":", FE_TOKEN_PNCT_COLON, 0)
LEXER_TEXT_MATCHER(pnct_braces_left, "{", FE_TOKEN_PNCT_BRACES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_braces_right, "}", FE_TOKEN_PNCT_BRACES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_comma, ",", FE_TOKEN_PNCT_COMMA, 0)
LEXER_TEXT_MATCHER(pnct_ellipsis, "...", FE_TOKEN_PNCT_ELLIPSIS, 0)
LEXER_TEXT_MATCHER(pnct_assign, "=", FE_TOKEN_PNCT_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_add_assign, "+=", FE_TOKEN_PNCT_ADD_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_sub_assign, "-=", FE_TOKEN_PNCT_SUB_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_mul_assign, "*=", FE_TOKEN_PNCT_MUL_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_div_assign, "/=", FE_TOKEN_PNCT_DIV_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_mod_assign, "%=", FE_TOKEN_PNCT_MOD_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_band_assign, "&=", FE_TOKEN_PNCT_BAND_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_bor_assign, "|=", FE_TOKEN_PNCT_BOR_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_bxor_assign, "^=", FE_TOKEN_PNCT_BXOR_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_shift_left_assign, "<<=", FE_TOKEN_PNCT_SHIFT_LEFT_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_shift_right_assign, ">>=", FE_TOKEN_PNCT_SHIFT_RIGHT_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_or, "||", FE_TOKEN_PNCT_OR, 0)
LEXER_TEXT_MATCHER(pnct_and, "&&", FE_TOKEN_PNCT_AND, 0)
LEXER_TEXT_MATCHER(pnct_bor, "|", FE_TOKEN_PNCT_BOR, 0)
LEXER_TEXT_MATCHER(pnct_bxor, "^", FE_TOKEN_PNCT_BXOR, 0)
LEXER_TEXT_MATCHER(pnct_band, "&", FE_TOKEN_PNCT_BAND, 0)
LEXER_TEXT_MATCHER(pnct_equal, "==", FE_TOKEN_PNCT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_not_equal, "!=", FE_TOKEN_PNCT_NOT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_lt, "<", FE_TOKEN_PNCT_LT, 0)
LEXER_TEXT_MATCHER(pnct_le, "<=", FE_TOKEN_PNCT_LE, 0)
LEXER_TEXT_MATCHER(pnct_gt, ">", FE_TOKEN_PNCT_GT, 0)
LEXER_TEXT_MATCHER(pnct_ge, ">=", FE_TOKEN_PNCT_GE, 0)
LEXER_TEXT_MATCHER(pnct_shift_left, "<<", FE_TOKEN_PNCT_SHIFT_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_shift_right, ">>", FE_TOKEN_PNCT_SHIFT_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_add, "+", FE_TOKEN_PNCT_ADD, 0)
LEXER_TEXT_MATCHER(pnct_sub, "-", FE_TOKEN_PNCT_SUB, 0)
LEXER_TEXT_MATCHER(pnct_mul, "*", FE_TOKEN_PNCT_MUL, 0)
LEXER_TEXT_MATCHER(pnct_div, "/", FE_TOKEN_PNCT_DIV, 0)
LEXER_TEXT_MATCHER(pnct_mod, "%", FE_TOKEN_PNCT_MOD, 0)
LEXER_TEXT_MATCHER(pnct_not, "!", FE_TOKEN_PNCT_NOT, 0)
LEXER_TEXT_MATCHER(pnct_bnot, "~", FE_TOKEN_PNCT_BNOT, 0)
LEXER_TEXT_MATCHER(pnct_inc, "++", FE_TOKEN_PNCT_INC, 0)
LEXER_TEXT_MATCHER(pnct_dec, "--", FE_TOKEN_PNCT_DEC, 0)
LEXER_TEXT_MATCHER(pnct_dot, ".", FE_TOKEN_PNCT_DOT, 0)
LEXER_TEXT_MATCHER(pnct_right_arrow, "->", FE_TOKEN_PNCT_RIGHT_ARROW, 0)
LEXER_TEXT_MATCHER(pnct_brackets_left, "[", FE_TOKEN_PNCT_BRACKETS_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_brackets_right, "]", FE_TOKEN_PNCT_BRACKETS_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_question_mark, "?", FE_TOKEN_PNCT_QUESTION_MARK, 0)




#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(space),

	_MATCHER_LIST_ITEM(single_line_comment),
	_MATCHER_LIST_ITEM(multiline_comment),

	_MATCHER_LIST_ITEM(preprocess),

	_MATCHER_LIST_ITEM(literal_uint_bin),
	_MATCHER_LIST_ITEM(literal_uint_oct),
	_MATCHER_LIST_ITEM(literal_uint_dec),
	_MATCHER_LIST_ITEM(literal_uint_hex),
	_MATCHER_LIST_ITEM(literal_real),
	_MATCHER_LIST_ITEM(literal_char),
	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(keyword),

	_MATCHER_LIST_ITEM(pnct_semicolon),
	_MATCHER_LIST_ITEM(pnct_parentheses_left),
	_MATCHER_LIST_ITEM(pnct_parentheses_right),
	_MATCHER_LIST_ITEM(pnct_colon),
	_MATCHER_LIST_ITEM(pnct_braces_left),
	_MATCHER_LIST_ITEM(pnct_braces_right),
	_MATCHER_LIST_ITEM(pnct_comma),
	_MATCHER_LIST_ITEM(pnct_ellipsis),
	_MATCHER_LIST_ITEM(pnct_assign),
	_MATCHER_LIST_ITEM(pnct_add_assign),
	_MATCHER_LIST_ITEM(pnct_sub_assign),
	_MATCHER_LIST_ITEM(pnct_mul_assign),
	_MATCHER_LIST_ITEM(pnct_div_assign),
	_MATCHER_LIST_ITEM(pnct_mod_assign),
	_MATCHER_LIST_ITEM(pnct_band_assign),
	_MATCHER_LIST_ITEM(pnct_bor_assign),
	_MATCHER_LIST_ITEM(pnct_bxor_assign),
	_MATCHER_LIST_ITEM(pnct_shift_left_assign),
	_MATCHER_LIST_ITEM(pnct_shift_right_assign),
	_MATCHER_LIST_ITEM(pnct_or),
	_MATCHER_LIST_ITEM(pnct_and),
	_MATCHER_LIST_ITEM(pnct_bor),
	_MATCHER_LIST_ITEM(pnct_bxor),
	_MATCHER_LIST_ITEM(pnct_band),
	_MATCHER_LIST_ITEM(pnct_equal),
	_MATCHER_LIST_ITEM(pnct_not_equal),
	_MATCHER_LIST_ITEM(pnct_lt),
	_MATCHER_LIST_ITEM(pnct_le),
	_MATCHER_LIST_ITEM(pnct_gt),
	_MATCHER_LIST_ITEM(pnct_ge),
	_MATCHER_LIST_ITEM(pnct_shift_left),
	_MATCHER_LIST_ITEM(pnct_shift_right),
	_MATCHER_LIST_ITEM(pnct_add),
	_MATCHER_LIST_ITEM(pnct_sub),
	_MATCHER_LIST_ITEM(pnct_mul),
	_MATCHER_LIST_ITEM(pnct_div),
	_MATCHER_LIST_ITEM(pnct_mod),
	_MATCHER_LIST_ITEM(pnct_not),
	_MATCHER_LIST_ITEM(pnct_bnot),
	_MATCHER_LIST_ITEM(pnct_inc),
	_MATCHER_LIST_ITEM(pnct_dec),
	_MATCHER_LIST_ITEM(pnct_dot),
	_MATCHER_LIST_ITEM(pnct_right_arrow),
	_MATCHER_LIST_ITEM(pnct_brackets_left),
	_MATCHER_LIST_ITEM(pnct_brackets_right),
	_MATCHER_LIST_ITEM(pnct_question_mark),

	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM

LexerContext * fe_lexer_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	return lexer_new_context(file, source, len, _matchers);
}

void fe_lexer_free_context(LexerContext * ctx) {
	assert(ctx);

	lexer_free_context(ctx);
}

uint32_t fe_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case FE_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint32_bin_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint32_oct_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint32_dec_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint32_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

uint64_t fe_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case FE_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint64_bin_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint64_oct_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint64_dec_val(token->content, token->len);
		}
		case FE_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint64_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

float fe_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == FE_TOKEN_LITERAL_REAL) {
		return util_get_float_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0f;
}

double fe_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == FE_TOKEN_LITERAL_REAL) {
		return util_get_double_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0;
}

char fe_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == FE_TOKEN_LITERAL_CHAR) {
		return util_get_char_val(token->content, token->len);
	} else {
		assert(0);
	}

	return '\0';
}

void fe_lexer_unescape_string(
	LexerContext *ctx,
	ResizableString *target,
	const char *source,
	size_t source_len
) {
	assert(ctx);
	assert(target);
	assert(source);

	util_unescape_string(target, source, source_len);
}
