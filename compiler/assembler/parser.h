#ifndef __COMPILER_ASSEMBLER_PARSER_H__
#define __COMPILER_ASSEMBLER_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

#include "../../map.h"

#include "ins.h"

#define	ASM_NODE_INVALID				0x0000

#define	ASM_NODE_MODULE					0x0100

#define	ASM_NODE_IDENTIFIER				0x0200

#define	ASM_NODE_LITERAL_UINT			0x0300
#define	ASM_NODE_LITERAL_REAL			0x0301
#define	ASM_NODE_LITERAL_CHAR			0x0302
#define	ASM_NODE_LITERAL_STRING			0x0303

#define	ASM_NODE_INSTRUCTION			0x0400



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
	Instruction 	*ins;
} AsmParserInsASTNodeData;

#define	ASM_INS_AST_NODE_GET_INS(node)			(((AsmParserInsASTNodeData *)&((node)->data[0]))->ins)

#define	ASM_INS_AST_NODE_SET_INS(node, v)		(((AsmParserInsASTNodeData *)&((node)->data[0]))->ins = (v))




#define	ASM_EXPR_EVAL_RESULT_TYPE_NULL		0
#define	ASM_EXPR_EVAL_RESULT_TYPE_UINT64	1
#define	ASM_EXPR_EVAL_RESULT_TYPE_DOUBLE	2
#define	ASM_EXPR_EVAL_RESULT_TYPE_STRING	3

typedef struct {
	uint8_t			type;
	union {
		uint64_t	u64;
		double		d;
		struct {
			const char	*ptr;
			size_t		len;
		} str;
	} value;
} AsmExprEvalResult;

typedef struct {
	AsmExprEvalResult	result;
} AsmParserExprASTNodeData;

#define	ASM_EXPR_AST_NODE_GET_RESULT(node)			(((AsmParserExprASTNodeData *)&((node)->data[0]))->result)





#define	ASM_ARCH_BIT16		1
#define	ASM_ARCH_BIT32		2
#define	ASM_ARCH_BIT64		3

#define	ASM_STEP_SCAN		1
#define	ASM_STEP_ENCODE		2

typedef struct {
	Map				symtable;
	int				arch;
	FILE			*out;
	uint64_t		address_counter;
	int				step;
} AsmParserContextData;

#define	ASM_PARSER_CONTEXT_DATA_GET_SYMTABLE(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->symtable)
#define	ASM_PARSER_CONTEXT_DATA_GET_ARCH(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->arch)
#define	ASM_PARSER_CONTEXT_DATA_GET_OUT(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->out)
#define	ASM_PARSER_CONTEXT_DATA_GET_ADDRESS_COUNTER(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->address_counter)
#define	ASM_PARSER_CONTEXT_DATA_GET_STEP(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->step)

#define	ASM_PARSER_CONTEXT_DATA_SET_ARCH(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->arch = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_OUT(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->out = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_ADDRESS(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->address_counter = (v))
#define	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->address_counter += (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_STEP(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->step = (v))




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

extern void asm_parser_print_ast(
	ParserContext * ctx,
	FILE *file
);

extern void asm_parser_set_symbol_by_rstr_key(
	ParserContext *ctx,
	ResizableString *key,
	uint64_t value
);
extern void asm_parser_set_symbol_by_token_key(
	ParserContext *ctx,
	LexerToken *key,
	uint64_t value
);
extern uint64_t asm_parser_get_symbol_by_rstr_key(
	ParserContext *ctx,
	ResizableString *key
);
extern uint64_t asm_parser_get_symbol_by_token_key(
	ParserContext *ctx,
	LexerToken *key
);

#endif
