#ifndef __FRONTEND_LEXCMD_H__
#define __FRONTEND_LEXCMD_H__

#include <inttypes.h>

#include "../../lexer.h"

#define	FE_LEXCMD_TOKEN_INVALID					0xffff

#define	FE_LEXCMD_TOKEN_SPACE					0x0000

#define	FE_LEXCMD_TOKEN_LITERAL_UINT_DEC		0x1000
#define	FE_LEXCMD_TOKEN_LITERAL_STRING			0x1001

#define	FE_LEXCMD_TOKEN_IDENTIFIER				0x2000

#define	FE_LEXCMD_TOKEN_KEYWORD_LINE			0x3000	// #line

extern LexerContext * fe_lexcmd_new_context(
	const char *file,
	const char *source,
	int len
);
extern void fe_lexcmd_free_context(LexerContext *ctx);
extern uint32_t fe_lexcmd_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
);

#endif
