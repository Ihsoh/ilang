#ifndef __COMPILER_ASSEMBLER_LEXER_H__
#define __COMPILER_ASSEMBLER_LEXER_H__

#include <inttypes.h>
#include <stddef.h>

#include "../../lexer.h"
#include "../../string.h"

#define	ASM_TOKEN_INVALID						0xffff

#define	ASM_TOKEN_SPACE							0x0000

#define	ASM_TOKEN_LITERAL_UINT_BIN				0x1000
#define	ASM_TOKEN_LITERAL_UINT_OCT				0x1001
#define	ASM_TOKEN_LITERAL_UINT_DEC				0x1002
#define	ASM_TOKEN_LITERAL_UINT_HEX				0x1003
#define	ASM_TOKEN_LITERAL_REAL					0x1004
#define	ASM_TOKEN_LITERAL_CHAR					0x1005
#define	ASM_TOKEN_LITERAL_STRING				0x1006

#define	ASM_TOKEN_IDENTIFIER					0x2000

#define	ASM_TOKEN_KEYWORD_BYTE					0x3000
#define	ASM_TOKEN_KEYWORD_WORD					0x3001
#define	ASM_TOKEN_KEYWORD_DWORD					0x3002
#define	ASM_TOKEN_KEYWORD_QWORD					0x3003
#define	ASM_TOKEN_KEYWORD_ADDR16				0x3004
#define	ASM_TOKEN_KEYWORD_ADDR32				0x3005
#define	ASM_TOKEN_KEYWORD_ADDR64				0x3006

#define	ASM_TOKEN_KEYWORD_HINT					0x3010
#define	ASM_TOKEN_KEYWORD_OPCODE_O1				0x3011
#define	ASM_TOKEN_KEYWORD_OPCODE_O2				0x3012
#define	ASM_TOKEN_KEYWORD_OPCODE_O3				0x3013
#define	ASM_TOKEN_KEYWORD_OPCODE_LEN			0x3014




#define	ASM_TOKEN_PNCT_SEMICOLON				0x4000	// ;
#define	ASM_TOKEN_PNCT_PARENTHESES_LEFT			0x4001	// (
#define	ASM_TOKEN_PNCT_PARENTHESES_RIGHT		0x4002	// )
#define	ASM_TOKEN_PNCT_COLON					0x4003	// :
#define	ASM_TOKEN_PNCT_COMMA					0x4004	// ,
#define	ASM_TOKEN_PNCT_ELLIPSIS					0x4005	// ...
#define	ASM_TOKEN_PNCT_BOR						0x4006	// |
#define	ASM_TOKEN_PNCT_BXOR						0x4007	// ^
#define	ASM_TOKEN_PNCT_BAND						0x4008	// &
#define	ASM_TOKEN_PNCT_EQUAL					0x4009	// ==
#define	ASM_TOKEN_PNCT_NOT_EQUAL				0x400a	// !=
#define	ASM_TOKEN_PNCT_LT						0x400b	// <
#define	ASM_TOKEN_PNCT_LE						0x400c	// <=
#define	ASM_TOKEN_PNCT_GT						0x400d	// >
#define	ASM_TOKEN_PNCT_GE						0x400e	// >=
#define	ASM_TOKEN_PNCT_SHIFT_LEFT				0x400f	// <<
#define	ASM_TOKEN_PNCT_SHIFT_RIGHT				0x4010	// >>
#define	ASM_TOKEN_PNCT_ADD						0x4011	// +
#define	ASM_TOKEN_PNCT_SUB						0x4012	// -
#define	ASM_TOKEN_PNCT_MUL						0x4013	// *
#define	ASM_TOKEN_PNCT_DIV						0x4014	// /
#define	ASM_TOKEN_PNCT_MOD						0x4015	// %
#define	ASM_TOKEN_PNCT_NOT						0x4016	// !
#define	ASM_TOKEN_PNCT_BNOT						0x4017	// ~
#define	ASM_TOKEN_PNCT_BRACKETS_LEFT			0x4018	// [
#define	ASM_TOKEN_PNCT_BRACKETS_RIGHT			0x4019	// ]
#define	ASM_TOKEN_PNCT_OR						0x401a	// ||
#define	ASM_TOKEN_PNCT_AND						0x401b	// &&
#define	ASM_TOKEN_PNCT_QUESTION_MARK			0x401c	// ?
#define	ASM_TOKEN_PNCT_ASSIGN					0x401d	// =


#define	ASM_TOKEN_KEYWORD_INSTRUCTION			0x5000
#define	ASM_TOKEN_KEYWORD_REGISTER				0x5001



#define	ASM_TOKEN_PREPROCESS					0xd000	// #...\n

#define	ASM_TOKEN_SINGLE_LINE_COMMENT			0xe000	// //...
#define	ASM_TOKEN_MULTILINE_COMMENT				0xe001	// /* ... */




extern LexerContext * asm_lexer_new_context(
	const char *file,
	const char *source,
	int len
);
extern void asm_lexer_free_context(
	LexerContext * ctx
);

extern uint32_t asm_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
);

extern uint64_t asm_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
);

extern float asm_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
);

extern double asm_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
);

extern char asm_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
);

extern void asm_lexer_unescape_string(
	LexerContext *ctx,
	ResizableString *target,
	const char *source,
	size_t source_len
);

extern bool asm_lexer_has_unsigned_mark(
	LexerContext *ctx,
	LexerToken *token
);

extern bool asm_lexer_has_float_mark(
	LexerContext *ctx,
	LexerToken *token
);

#endif
