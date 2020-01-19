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

#define	ASM_TOKEN_KEYWORD_DIRECTIVE_BIT			0x3000
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_SET			0x3001
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_IF			0x3002
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ELIF		0x3003
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ENDIF		0x3004
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_FOR			0x3005
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ENDFOR		0x3006
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_CONTINUE	0x3007
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_BREAK		0x3008
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_MACRO		0x3009
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ENDMACRO	0x300a
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ARGC		0x300b
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_ARGV		0x300c
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_BYTE		0x300d
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_2BYTE		0x300e
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_4BYTE		0x300f
#define	ASM_TOKEN_KEYWORD_DIRECTIVE_8BYTE		0x3010

#define	ASM_TOKEN_KEYWORD_INSTRUCTION			0x4000

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

#define	ASM_TOKEN_SINGLE_LINE_COMMENT			0xe000	// //...
#define	ASM_TOKEN_MULTILINE_COMMENT				0xe001	// /* ... */

#endif