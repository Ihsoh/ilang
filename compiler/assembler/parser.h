#ifndef __COMPILER_ASSEMBLER_PARSER_H__
#define __COMPILER_ASSEMBLER_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

#define	ASM_NODE_INVALID				0x0000

#define	ASM_NODE_MODULE					0x0100

#define	ASM_NODE_IDENTIFIER				0x0200

#define	ASM_NODE_LITERAL_UINT			0x0300
#define	ASM_NODE_LITERAL_REAL			0x0301
#define	ASM_NODE_LITERAL_CHAR			0x0302
#define	ASM_NODE_LITERAL_STRING			0x0303




#define	ASM_NODE_EXPR						0x0e00

#define	ASM_NODE_EXPR_OR					0x0e20

#define	ASM_NODE_EXPR_AND					0x0e30

#define	ASM_NODE_EXPR_BOR					0x0e40

#define	ASM_NODE_EXPR_BXOR					0x0e50

#define	ASM_NODE_EXPR_BAND					0x0e60

#define	ASM_NODE_EXPR_EQ					0x0e70
#define	ASM_NODE_EXPR_NEQ					0x0e71

#define	ASM_NODE_EXPR_LT					0x0e80
#define	ASM_NODE_EXPR_LE					0x0e81
#define	ASM_NODE_EXPR_GT					0x0e82
#define	ASM_NODE_EXPR_GE					0x0e83

#define	ASM_NODE_EXPR_SHIFT_LEFT			0x0e90
#define	ASM_NODE_EXPR_SHIFT_RIGHT			0x0e91

#define	ASM_NODE_EXPR_ADD					0x0ea0
#define	ASM_NODE_EXPR_SUB					0x0ea1

#define	ASM_NODE_EXPR_MUL					0x0eb0
#define	ASM_NODE_EXPR_DIV					0x0eb1
#define	ASM_NODE_EXPR_MOD					0x0eb2

#define	ASM_NODE_EXPR_NOT					0x0ec0
#define	ASM_NODE_EXPR_POSITIVE				0x0ec1
#define	ASM_NODE_EXPR_NEGATIVE				0x0ec2
#define	ASM_NODE_EXPR_BNOT					0x0ec3

#define	ASM_NODE_EXPR_ATOM					0x0ee0





typedef struct {
	int dummy;
} AsmParserContextData;

extern ParserContext * asm_parser_new_context(
	const char *file,
	const char *source,
	int len
);
extern void asm_parser_free_context(
	ParserContext * ctx
);
extern void asm_parser_parse(
	ParserContext * ctx
);

#endif
