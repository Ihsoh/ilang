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
	return chr == -1 || !isalpha(chr) || chr != '_';
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
	return _COMMON_LEXER_MATCHER(space, ASM_TOKEN_SPACE);
LEXER_MATCHER_END

/*
	捕获单行注释。
*/
LEXER_MATCHER(single_line_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(single_line_comment, ASM_TOKEN_SINGLE_LINE_COMMENT);
LEXER_MATCHER_END

/*
	多行注释。
*/
LEXER_MATCHER(multiline_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(multiline_comment, ASM_TOKEN_MULTILINE_COMMENT);
LEXER_MATCHER_END

/*
	#...\n
*/
LEXER_MATCHER(preprocess)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(preprocess, ASM_TOKEN_PREPROCESS);
LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
LEXER_MATCHER(literal_uint_bin)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_bin, ASM_TOKEN_LITERAL_UINT_BIN);
LEXER_MATCHER_END

/*
	捕获八进制数字字面量。
*/
LEXER_MATCHER(literal_uint_oct)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_oct, ASM_TOKEN_LITERAL_UINT_OCT);
LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_dec, ASM_TOKEN_LITERAL_UINT_DEC);
LEXER_MATCHER_END

/*
	捕获十六进制数字字面量。
*/
LEXER_MATCHER(literal_uint_hex)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_hex, ASM_TOKEN_LITERAL_UINT_HEX);
LEXER_MATCHER_END

/*
	捕获实型字面量。
*/
LEXER_MATCHER(literal_real)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_real, ASM_TOKEN_LITERAL_REAL);
LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_char, ASM_TOKEN_LITERAL_CHAR);
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_string, ASM_TOKEN_LITERAL_STRING);
LEXER_MATCHER_END













