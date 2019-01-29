#ifndef __COMPILER_LEXER_H__
#define __COMPILER_LEXER_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#include "../lexer.h"
#include "../string.h"

typedef struct {
	bool (*is_delimiter)(int chr);
} CommonLexerContext;

#define	COMMON_LEXER_MATCHER_DECL(name)	\
	extern LexerToken * common_lexer_matcher_##name(	\
		LexerContext *ctx,	\
		CommonLexerContext *cmn_ctx,	\
		int type	\
	);

COMMON_LEXER_MATCHER_DECL(space)
COMMON_LEXER_MATCHER_DECL(single_line_comment)
COMMON_LEXER_MATCHER_DECL(multiline_comment)
COMMON_LEXER_MATCHER_DECL(preprocess)
COMMON_LEXER_MATCHER_DECL(literal_uint_bin)
COMMON_LEXER_MATCHER_DECL(literal_uint_oct)
COMMON_LEXER_MATCHER_DECL(literal_uint_dec)
COMMON_LEXER_MATCHER_DECL(literal_uint_hex)
COMMON_LEXER_MATCHER_DECL(literal_real)
COMMON_LEXER_MATCHER_DECL(literal_char)
COMMON_LEXER_MATCHER_DECL(literal_string)

#endif
