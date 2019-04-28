#ifndef __COMPILER_BACKEND_LEXER_H__
#define __COMPILER_BACKEND_LEXER_H__

#include <inttypes.h>
#include <stddef.h>

#include "../../lexer.h"
#include "../../string.h"

#define	BE_TOKEN_INVALID				0xffff

#define	BE_TOKEN_SPACE					0x0000

#define	BE_TOKEN_LITERAL_UINT_BIN		0x1000
#define	BE_TOKEN_LITERAL_UINT_OCT		0x1001
#define	BE_TOKEN_LITERAL_UINT_DEC		0x1002
#define	BE_TOKEN_LITERAL_UINT_HEX		0x1003
#define	BE_TOKEN_LITERAL_REAL			0x1004
#define	BE_TOKEN_LITERAL_CHAR			0x1005
#define	BE_TOKEN_LITERAL_STRING			0x1006

#define	BE_TOKEN_IDENTIFIER				0x2000

#define	BE_TOKEN_KEYWORD_FUNC			0x3000	// function
#define	BE_TOKEN_KEYWORD_BR				0x3001	// br
#define BE_TOKEN_KEYWORD_RETURN         0x3002	// return
#define	BE_TOKEN_KEYWORD_LABEL			0x3003	// label
#define	BE_TOKEN_KEYWORD_CAST			0x3004	// cast
#define	BE_TOKEN_KEYWORD_STRUCT			0x3005	// struct
#define	BE_TOKEN_KEYWORD_VAR			0x3006	// var
// #define	BE_TOKEN_KEYWORD_ASM			0x3007	// asm
#define	BE_TOKEN_KEYWORD_DUMMY			0x3008	// dummy
#define	BE_TOKEN_KEYWORD_ALIGN			0x3009	// align
#define	BE_TOKEN_KEYWORD_PACKED			0x300a	// packed
#define	BE_TOKEN_KEYWORD_STORE			0x300b	// store
#define	BE_TOKEN_KEYWORD_LOAD			0x300c	// load
#define	BE_TOKEN_KEYWORD_SIZEOF			0x300d	// sizeof
#define	BE_TOKEN_KEYWORD_ALIGNOF		0x300e	// alignof
#define	BE_TOKEN_KEYWORD_CBR			0x300f	// cbr
#define	BE_TOKEN_KEYWORD_ASSIGN			0x3010	// assign
#define	BE_TOKEN_KEYWORD_ASM			0x3011	// asm
#define	BE_TOKEN_KEYWORD_ASM_SET_REG	0x3012	// asm_set_reg
#define	BE_TOKEN_KEYWORD_ASM_GET_REG	0x3013	// asm_get_reg

#define	BE_TOKEN_KEYWORD_CHAR			0x3100	// char
#define	BE_TOKEN_KEYWORD_INT8			0x3101	// int8
#define	BE_TOKEN_KEYWORD_INT16			0x3102	// int16
#define	BE_TOKEN_KEYWORD_INT32			0x3103	// int32
#define	BE_TOKEN_KEYWORD_INT64			0x3104	// int64
#define	BE_TOKEN_KEYWORD_UINT8			0x3105	// uint8
#define	BE_TOKEN_KEYWORD_UINT16			0x3106	// uint16
#define	BE_TOKEN_KEYWORD_UINT32			0x3107	// uint32
#define	BE_TOKEN_KEYWORD_UINT64			0x3108	// uint64
#define	BE_TOKEN_KEYWORD_FLOAT			0x3109	// float
#define	BE_TOKEN_KEYWORD_DOUBLE			0x310a	// double

#define	BE_TOKEN_KEYWORD_VA_LIST		0x310b	// __va_list

#define	BE_TOKEN_PNCT_SEMICOLON				0x4000	// ;
#define	BE_TOKEN_PNCT_PARENTHESES_LEFT		0x4001	// (
#define	BE_TOKEN_PNCT_PARENTHESES_RIGHT		0x4002	// )
#define	BE_TOKEN_PNCT_COLON					0x4003	// :
#define	BE_TOKEN_PNCT_BRACES_LEFT			0x4004	// {
#define	BE_TOKEN_PNCT_BRACES_RIGHT			0x4005	// }
#define	BE_TOKEN_PNCT_COMMA					0x4006	// ,
#define	BE_TOKEN_PNCT_ELLIPSIS				0x4007	// ...
#define	BE_TOKEN_PNCT_ASSIGN				0x4008	// =
#define	BE_TOKEN_PNCT_BOR					0x4009	// |
#define	BE_TOKEN_PNCT_BXOR					0x400a	// ^
#define	BE_TOKEN_PNCT_BAND					0x400b	// &
#define	BE_TOKEN_PNCT_EQUAL					0x400c	// ==
#define	BE_TOKEN_PNCT_NOT_EQUAL				0x400d	// !=
#define	BE_TOKEN_PNCT_LT					0x400e	// <
#define	BE_TOKEN_PNCT_LE					0x400f	// <=
#define	BE_TOKEN_PNCT_GT					0x4010	// >
#define	BE_TOKEN_PNCT_GE					0x4011	// >=
#define	BE_TOKEN_PNCT_SHIFT_LEFT			0x4012	// <<
#define	BE_TOKEN_PNCT_SHIFT_RIGHT			0x4013	// >>
#define	BE_TOKEN_PNCT_ADD					0x4014	// +
#define	BE_TOKEN_PNCT_SUB					0x4015	// -
#define	BE_TOKEN_PNCT_MUL					0x4016	// *
#define	BE_TOKEN_PNCT_DIV					0x4017	// /
#define	BE_TOKEN_PNCT_MOD					0x4018	// %
#define	BE_TOKEN_PNCT_NOT					0x4019	// !
#define	BE_TOKEN_PNCT_BNOT					0x401a	// ~
#define	BE_TOKEN_PNCT_RIGHT_ARROW			0x401b	// ->
#define	BE_TOKEN_PNCT_BRACKETS_LEFT			0x401c	// [
#define	BE_TOKEN_PNCT_BRACKETS_RIGHT		0x401d	// ]
#define	BE_TOKEN_PNCT_OR					0x401e	// ||
#define	BE_TOKEN_PNCT_AND					0x401f	// &&
#define	BE_TOKEN_PNCT_QUESTION_MARK			0x4020	// ?

#define	BE_TOKEN_PREPROCESS					0xd000	// #...\n

#define	BE_TOKEN_SINGLE_LINE_COMMENT		0xe000	// //...
#define	BE_TOKEN_MULTILINE_COMMENT			0xe001	// /* ... */

extern LexerContext * be_lexer_new_context(
	const char *file,
	const char *source,
	int len
);
extern void be_lexer_free_context(
	LexerContext * ctx
);

extern uint32_t be_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
);

extern uint64_t be_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
);

extern float be_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
);

extern double be_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
);

extern char be_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
);

extern void be_lexer_unescape_string(
	LexerContext *ctx,
	ResizableString *target,
	const char *source,
	size_t source_len
);

#endif
