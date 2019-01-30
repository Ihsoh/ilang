#ifndef __COMPILER_FRONTEND_LEXER_H__
#define __COMPILER_FRONTEND_LEXER_H__

#include <inttypes.h>
#include <stddef.h>

#include "../../lexer.h"
#include "../../string.h"

#define	FE_TOKEN_INVALID				0xffff

#define	FE_TOKEN_SPACE					0x0000

#define	FE_TOKEN_LITERAL_UINT_BIN		0x1000
#define	FE_TOKEN_LITERAL_UINT_OCT		0x1001
#define	FE_TOKEN_LITERAL_UINT_DEC		0x1002
#define	FE_TOKEN_LITERAL_UINT_HEX		0x1003
#define	FE_TOKEN_LITERAL_REAL			0x1004
#define	FE_TOKEN_LITERAL_CHAR			0x1005
#define	FE_TOKEN_LITERAL_STRING			0x1006

#define	FE_TOKEN_IDENTIFIER				0x2000

#define	FE_TOKEN_KEYWORD_FUNC			0x3000	// function
#define	FE_TOKEN_KEYWORD_IF				0x3001	// if
#define	FE_TOKEN_KEYWORD_ELIF			0x3002	// elif
#define	FE_TOKEN_KEYWORD_ELSE			0x3003	// else
#define	FE_TOKEN_KEYWORD_WHILE			0x3004	// while
#define	FE_TOKEN_KEYWORD_DO				0x3005	// do
#define	FE_TOKEN_KEYWORD_FOR			0x3006	// for
#define	FE_TOKEN_KEYWORD_BREAK			0x3007	// break
#define	FE_TOKEN_KEYWORD_CONTINUE		0x3008	// continue
#define FE_TOKEN_KEYWORD_RETURN         0x3009	// return
#define	FE_TOKEN_KEYWORD_GOTO			0x300a	// goto
#define	FE_TOKEN_KEYWORD_LABEL			0x300b	// label
#define	FE_TOKEN_KEYWORD_SIZEOF			0x300c	// sizeof
#define	FE_TOKEN_KEYWORD_CAST			0x300d	// cast
#define	FE_TOKEN_KEYWORD_STRUCT			0x300e	// struct
#define	FE_TOKEN_KEYWORD_VAR			0x300f	// var
#define	FE_TOKEN_KEYWORD_ASM			0x3010	// asm
#define	FE_TOKEN_KEYWORD_INTERN			0x3011	// intern
#define	FE_TOKEN_KEYWORD_EXTERN			0x3012	// extern
#define	FE_TOKEN_KEYWORD_DUMMY			0x3013	// dummy
#define	FE_TOKEN_KEYWORD_ALIGNOF		0x3014	// alignof
#define	FE_TOKEN_KEYWORD_ALIGN			0x3015	// align
#define	FE_TOKEN_KEYWORD_PACKED			0x3016	// packed

#define	FE_TOKEN_KEYWORD_VA_START		0x3017	// __va_start
#define	FE_TOKEN_KEYWORD_VA_ARG			0x3018	// __va_arg
#define	FE_TOKEN_KEYWORD_VA_END			0x3019	// __va_end
#define	FE_TOKEN_KEYWORD_VA_COPY		0x301a	// __va_copy

#define	FE_TOKEN_KEYWORD_CHAR			0x3100	// char
#define	FE_TOKEN_KEYWORD_INT8			0x3101	// int8
#define	FE_TOKEN_KEYWORD_INT16			0x3102	// int16
#define	FE_TOKEN_KEYWORD_INT32			0x3103	// int32
#define	FE_TOKEN_KEYWORD_INT64			0x3104	// int64
#define	FE_TOKEN_KEYWORD_UINT8			0x3105	// uint8
#define	FE_TOKEN_KEYWORD_UINT16			0x3106	// uint16
#define	FE_TOKEN_KEYWORD_UINT32			0x3107	// uint32
#define	FE_TOKEN_KEYWORD_UINT64			0x3108	// uint64
#define	FE_TOKEN_KEYWORD_FLOAT			0x3109	// float
#define	FE_TOKEN_KEYWORD_DOUBLE			0x310a	// double

#define	FE_TOKEN_KEYWORD_VA_LIST		0x310b	// __va_list

#define	FE_TOKEN_PNCT_SEMICOLON				0x4000	// ;
#define	FE_TOKEN_PNCT_PARENTHESES_LEFT		0x4001	// (
#define	FE_TOKEN_PNCT_PARENTHESES_RIGHT		0x4002	// )
#define	FE_TOKEN_PNCT_COLON					0x4003	// :
#define	FE_TOKEN_PNCT_BRACES_LEFT			0x4004	// {
#define	FE_TOKEN_PNCT_BRACES_RIGHT			0x4005	// }
#define	FE_TOKEN_PNCT_COMMA					0x4006	// ,
#define	FE_TOKEN_PNCT_ELLIPSIS				0x4007	// ...
#define	FE_TOKEN_PNCT_ASSIGN				0x4008	// =
#define	FE_TOKEN_PNCT_ADD_ASSIGN			0x4009	// +=
#define	FE_TOKEN_PNCT_SUB_ASSIGN			0x400a	// -=
#define	FE_TOKEN_PNCT_MUL_ASSIGN			0x400b	// *=
#define	FE_TOKEN_PNCT_DIV_ASSIGN			0x400c	// /=
#define	FE_TOKEN_PNCT_MOD_ASSIGN			0x400d	// %=
#define	FE_TOKEN_PNCT_BAND_ASSIGN			0x400e	// &=
#define	FE_TOKEN_PNCT_BOR_ASSIGN			0x400f	// |=
#define	FE_TOKEN_PNCT_BXOR_ASSIGN			0x4010	// ^=
#define	FE_TOKEN_PNCT_SHIFT_LEFT_ASSIGN		0x4011	// <<=
#define	FE_TOKEN_PNCT_SHIFT_RIGHT_ASSIGN	0x4012	// >>=
#define	FE_TOKEN_PNCT_OR					0x4013	// ||
#define	FE_TOKEN_PNCT_AND					0x4014	// &&
#define	FE_TOKEN_PNCT_BOR					0x4015	// |
#define	FE_TOKEN_PNCT_BXOR					0x4016	// ^
#define	FE_TOKEN_PNCT_BAND					0x4017	// &
#define	FE_TOKEN_PNCT_EQUAL					0x4018	// ==
#define	FE_TOKEN_PNCT_NOT_EQUAL				0x4019	// !=
#define	FE_TOKEN_PNCT_LT					0x401a	// <
#define	FE_TOKEN_PNCT_LE					0x401b	// <=
#define	FE_TOKEN_PNCT_GT					0x401c	// >
#define	FE_TOKEN_PNCT_GE					0x401d	// >=
#define	FE_TOKEN_PNCT_SHIFT_LEFT			0x401e	// <<
#define	FE_TOKEN_PNCT_SHIFT_RIGHT			0x401f	// >>
#define	FE_TOKEN_PNCT_ADD					0x4020	// +
#define	FE_TOKEN_PNCT_SUB					0x4021	// -
#define	FE_TOKEN_PNCT_MUL					0x4022	// *
#define	FE_TOKEN_PNCT_DIV					0x4023	// /
#define	FE_TOKEN_PNCT_MOD					0x4024	// %
#define	FE_TOKEN_PNCT_NOT					0x4025	// !
#define	FE_TOKEN_PNCT_BNOT					0x4026	// ~
#define	FE_TOKEN_PNCT_INC					0x4027	// ++
#define	FE_TOKEN_PNCT_DEC					0x4028	// --
#define	FE_TOKEN_PNCT_DOT					0x4029	// .
#define	FE_TOKEN_PNCT_RIGHT_ARROW			0x402a	// ->
#define	FE_TOKEN_PNCT_BRACKETS_LEFT			0x402b	// [
#define	FE_TOKEN_PNCT_BRACKETS_RIGHT		0x402c	// ]
#define	FE_TOKEN_PNCT_QUESTION_MARK			0x402d	// ?

#define	FE_TOKEN_PREPROCESS					0xd000	// #...\n

#define	FE_TOKEN_SINGLE_LINE_COMMENT		0xe000	// //...
#define	FE_TOKEN_MULTILINE_COMMENT			0xe001	// /* ... */




extern LexerContext * fe_lexer_new_context(
	const char *file,
	const char *source,
	int len
);
extern void fe_lexer_free_context(LexerContext * ctx);

extern uint32_t fe_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
);
extern uint64_t fe_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
);

extern float fe_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
);
extern double fe_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
);

extern char fe_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
);

extern void fe_lexer_unescape_string(
	LexerContext *ctx,
	ResizableString *target,
	const char *source,
	size_t source_len
);

#endif
