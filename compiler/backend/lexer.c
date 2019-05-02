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

#include "../lexer.h"

static bool _is_delimiter(int chr) {
	return chr == -1 || !isalpha(chr) || chr != '_' || chr != '.' || chr != '$';
}

static CommonLexerContext _common_lexer_context = {
	.is_delimiter = _is_delimiter
};

#define	_COMMON_LEXER_MATCHER(name, type)	\
	(common_lexer_matcher_##name(ctx, &_common_lexer_context, (type)))




/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(space, BE_TOKEN_SPACE);
LEXER_MATCHER_END

/*
	捕获单行注释。
*/
LEXER_MATCHER(single_line_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(single_line_comment, BE_TOKEN_SINGLE_LINE_COMMENT);
LEXER_MATCHER_END

/*
	多行注释。
*/
LEXER_MATCHER(multiline_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(multiline_comment, BE_TOKEN_MULTILINE_COMMENT);
LEXER_MATCHER_END

/*
	#...\n
*/
LEXER_MATCHER(preprocess)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(preprocess, BE_TOKEN_PREPROCESS);
LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
LEXER_MATCHER(literal_uint_bin)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_bin, BE_TOKEN_LITERAL_UINT_BIN);
LEXER_MATCHER_END

/*
	捕获八进制数字字面量。
*/
LEXER_MATCHER(literal_uint_oct)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_oct, BE_TOKEN_LITERAL_UINT_OCT);
LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_dec, BE_TOKEN_LITERAL_UINT_DEC);
LEXER_MATCHER_END

/*
	捕获十六进制数字字面量。
*/
LEXER_MATCHER(literal_uint_hex)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_hex, BE_TOKEN_LITERAL_UINT_HEX);
LEXER_MATCHER_END

/*
	捕获实型字面量。
*/
LEXER_MATCHER(literal_real)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_real, BE_TOKEN_LITERAL_REAL);
LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_char, BE_TOKEN_LITERAL_CHAR);
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_string, BE_TOKEN_LITERAL_STRING);
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
		_KEYWORD("function", BE_TOKEN_KEYWORD_FUNC),
		_KEYWORD("br", BE_TOKEN_KEYWORD_BR),
		_KEYWORD("return", BE_TOKEN_KEYWORD_RETURN),
		_KEYWORD("label", BE_TOKEN_KEYWORD_LABEL),
		_KEYWORD("cast", BE_TOKEN_KEYWORD_CAST),
		_KEYWORD("struct", BE_TOKEN_KEYWORD_STRUCT),
		_KEYWORD("var", BE_TOKEN_KEYWORD_VAR),
		_KEYWORD("asm", BE_TOKEN_KEYWORD_ASM),
		_KEYWORD("dummy", BE_TOKEN_KEYWORD_DUMMY),
		_KEYWORD("align", BE_TOKEN_KEYWORD_ALIGN),
		_KEYWORD("packed", BE_TOKEN_KEYWORD_PACKED),

		_KEYWORD("char", BE_TOKEN_KEYWORD_CHAR),
		_KEYWORD("int8", BE_TOKEN_KEYWORD_INT8),
		_KEYWORD("int16", BE_TOKEN_KEYWORD_INT16),
		_KEYWORD("int32", BE_TOKEN_KEYWORD_INT32),
		_KEYWORD("int64", BE_TOKEN_KEYWORD_INT64),
		_KEYWORD("uint8", BE_TOKEN_KEYWORD_UINT8),
		_KEYWORD("uint16", BE_TOKEN_KEYWORD_UINT16),
		_KEYWORD("uint32", BE_TOKEN_KEYWORD_UINT32),
		_KEYWORD("uint64", BE_TOKEN_KEYWORD_UINT64),
		_KEYWORD("float", BE_TOKEN_KEYWORD_FLOAT),
		_KEYWORD("double", BE_TOKEN_KEYWORD_DOUBLE),

		_KEYWORD("store", BE_TOKEN_KEYWORD_STORE),
		_KEYWORD("load", BE_TOKEN_KEYWORD_LOAD),
		_KEYWORD("sizeof", BE_TOKEN_KEYWORD_SIZEOF),
		_KEYWORD("alignof", BE_TOKEN_KEYWORD_ALIGNOF),
		_KEYWORD("cbr", BE_TOKEN_KEYWORD_CBR),
		_KEYWORD("assign", BE_TOKEN_KEYWORD_ASSIGN),

		_KEYWORD("asm", BE_TOKEN_KEYWORD_ASM),
		_KEYWORD("asm_set_reg", BE_TOKEN_KEYWORD_ASM_SET_REG),
		_KEYWORD("asm_get_reg", BE_TOKEN_KEYWORD_ASM_GET_REG),

		_KEYWORD("ref", BE_TOKEN_KEYWORD_REF),

		_KEYWORD("trunc", BE_TOKEN_KEYWORD_TRUNC),
		_KEYWORD("sext", BE_TOKEN_KEYWORD_SEXT),
		_KEYWORD("sitofp", BE_TOKEN_KEYWORD_SITOFP),
		_KEYWORD("inttoptr", BE_TOKEN_KEYWORD_INTTOPTR),
		_KEYWORD("zext", BE_TOKEN_KEYWORD_ZEXT),
		_KEYWORD("uitofp", BE_TOKEN_KEYWORD_UITOFP),
		_KEYWORD("fptosi", BE_TOKEN_KEYWORD_FPTOSI),
		_KEYWORD("fptoui", BE_TOKEN_KEYWORD_FPTOUI),
		_KEYWORD("fpext", BE_TOKEN_KEYWORD_FPEXT),
		_KEYWORD("fptrunc", BE_TOKEN_KEYWORD_FPTRUNC),
		_KEYWORD("fptoint", BE_TOKEN_KEYWORD_PTRTOINT),
		_KEYWORD("bitcast", BE_TOKEN_KEYWORD_BITCAST),




		{NULL, 0, BE_TOKEN_INVALID}
	};

	#undef	_KEYWORD

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_' || chr == '.' || chr == '$') {
		lexer_init_token(ctx, &token, BE_TOKEN_IDENTIFIER);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_' || chr == '.' || chr == '@', {
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




LEXER_TEXT_MATCHER(pnct_semicolon, ";", BE_TOKEN_PNCT_SEMICOLON, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_left, "(", BE_TOKEN_PNCT_PARENTHESES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_right, ")", BE_TOKEN_PNCT_PARENTHESES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_colon, ":", BE_TOKEN_PNCT_COLON, 0)
LEXER_TEXT_MATCHER(pnct_braces_left, "{", BE_TOKEN_PNCT_BRACES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_braces_right, "}", BE_TOKEN_PNCT_BRACES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_comma, ",", BE_TOKEN_PNCT_COMMA, 0)
LEXER_TEXT_MATCHER(pnct_ellipsis, "...", BE_TOKEN_PNCT_ELLIPSIS, 0)
LEXER_TEXT_MATCHER(pnct_assign, "=", BE_TOKEN_PNCT_ASSIGN, 0)
LEXER_TEXT_MATCHER(pnct_bor, "|", BE_TOKEN_PNCT_BOR, 0)
LEXER_TEXT_MATCHER(pnct_bxor, "^", BE_TOKEN_PNCT_BXOR, 0)
LEXER_TEXT_MATCHER(pnct_band, "&", BE_TOKEN_PNCT_BAND, 0)
LEXER_TEXT_MATCHER(pnct_equal, "==", BE_TOKEN_PNCT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_not_equal, "!=", BE_TOKEN_PNCT_NOT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_lt, "<", BE_TOKEN_PNCT_LT, 0)
LEXER_TEXT_MATCHER(pnct_le, "<=", BE_TOKEN_PNCT_LE, 0)
LEXER_TEXT_MATCHER(pnct_gt, ">", BE_TOKEN_PNCT_GT, 0)
LEXER_TEXT_MATCHER(pnct_ge, ">=", BE_TOKEN_PNCT_GE, 0)
LEXER_TEXT_MATCHER(pnct_shift_left, "<<", BE_TOKEN_PNCT_SHIFT_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_shift_right, ">>", BE_TOKEN_PNCT_SHIFT_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_add, "+", BE_TOKEN_PNCT_ADD, 0)
LEXER_TEXT_MATCHER(pnct_sub, "-", BE_TOKEN_PNCT_SUB, 0)
LEXER_TEXT_MATCHER(pnct_mul, "*", BE_TOKEN_PNCT_MUL, 0)
LEXER_TEXT_MATCHER(pnct_div, "/", BE_TOKEN_PNCT_DIV, 0)
LEXER_TEXT_MATCHER(pnct_mod, "%", BE_TOKEN_PNCT_MOD, 0)
LEXER_TEXT_MATCHER(pnct_not, "!", BE_TOKEN_PNCT_NOT, 0)
LEXER_TEXT_MATCHER(pnct_bnot, "~", BE_TOKEN_PNCT_BNOT, 0)
LEXER_TEXT_MATCHER(pnct_right_arrow, "->", BE_TOKEN_PNCT_RIGHT_ARROW, 0)
LEXER_TEXT_MATCHER(pnct_brackets_left, "[", BE_TOKEN_PNCT_BRACKETS_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_brackets_right, "]", BE_TOKEN_PNCT_BRACKETS_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_or, "||", BE_TOKEN_PNCT_OR, 0)
LEXER_TEXT_MATCHER(pnct_and, "&&", BE_TOKEN_PNCT_AND, 0)
LEXER_TEXT_MATCHER(pnct_question_mark, "?", BE_TOKEN_PNCT_QUESTION_MARK, 0)




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
	_MATCHER_LIST_ITEM(pnct_right_arrow),
	_MATCHER_LIST_ITEM(pnct_brackets_left),
	_MATCHER_LIST_ITEM(pnct_brackets_right),
	_MATCHER_LIST_ITEM(pnct_or),
	_MATCHER_LIST_ITEM(pnct_and),
	_MATCHER_LIST_ITEM(pnct_question_mark),

	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM

LexerContext * be_lexer_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	return lexer_new_context(file, source, len, _matchers);
}

void be_lexer_free_context(
	LexerContext * ctx
) {
	assert(ctx);

	lexer_free_context(ctx);
}

uint32_t be_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case BE_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint32_bin_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint32_oct_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint32_dec_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint32_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

uint64_t be_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case BE_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint64_bin_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint64_oct_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint64_dec_val(token->content, token->len);
		}
		case BE_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint64_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

float be_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == BE_TOKEN_LITERAL_REAL) {
		return util_get_float_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0f;
}

double be_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == BE_TOKEN_LITERAL_REAL) {
		return util_get_double_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0;
}

char be_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == BE_TOKEN_LITERAL_CHAR) {
		return util_get_char_val(token->content, token->len);
	} else {
		assert(0);
	}

	return '\0';
}

void be_lexer_unescape_string(
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
