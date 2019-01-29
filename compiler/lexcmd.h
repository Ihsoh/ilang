#ifndef __COMPILER_LEXCMD_H__
#define __COMPILER_LEXCMD_H__

#include <inttypes.h>

#include "../lexer.h"

#define	COMMON_LEXCMD_TOKEN_INVALID					0xffff

#define	COMMON_LEXCMD_TOKEN_SPACE					0x0000

#define	COMMON_LEXCMD_TOKEN_LITERAL_UINT_DEC		0x1000
#define	COMMON_LEXCMD_TOKEN_LITERAL_STRING			0x1001

#define	COMMON_LEXCMD_TOKEN_IDENTIFIER				0x2000

#define	COMMON_LEXCMD_TOKEN_KEYWORD_LINE			0x3000	// #line

extern LexerContext * common_lexcmd_new_context(
	const char *file,
	const char *source,
	int len
);
extern void common_lexcmd_free_context(LexerContext *ctx);
extern uint32_t common_lexcmd_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
);

#endif
