#ifndef	__COMPILER_PREPROCESSOR_LEXER_H__
#define	__COMPILER_PREPROCESSOR_LEXER_H__

#include <inttypes.h>

#include "../../lexer.h"

#define	PP_TOKEN_INVALID				0xffff

#define	PP_TOKEN_SPACE					0x0000

#define	PP_TOKEN_KEYWORD_INCLUDE		0x1000
#define	PP_TOKEN_KEYWORD_DEFINE			0x1001
#define	PP_TOKEN_KEYWORD_UNDEF			0x1002
#define	PP_TOKEN_KEYWORD_IF				0x1003
#define	PP_TOKEN_KEYWORD_IFDEF			0x1004
#define	PP_TOKEN_KEYWORD_IFNDEF			0x1005
#define	PP_TOKEN_KEYWORD_ELIF			0x1006
#define	PP_TOKEN_KEYWORD_ELIFDEF		0x1007
#define	PP_TOKEN_KEYWORD_ELIFNDEF		0x1008
#define	PP_TOKEN_KEYWORD_ELSE			0x1009
#define	PP_TOKEN_KEYWORD_ENDIF			0x100a
#define	PP_TOKEN_KEYWORD_INFO			0x100b
#define	PP_TOKEN_KEYWORD_WARN			0x100c
#define	PP_TOKEN_KEYWORD_ERROR			0x100d

#define	PP_TOKEN_CMD_LINE				0x2000

#define	PP_TOKEN_LINE					0x3000

extern LexerContext * pp_lexer_new_context(
	const char *file,
	const char *source,
	int len
);
extern void pp_lexer_free_context(LexerContext * ctx);

#endif
