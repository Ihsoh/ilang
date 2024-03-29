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
#define	ASM_NODE_INSTRUCTION_BUNCH		0x0401

#define	ASM_NODE_REG					0x0500

#define	ASM_NODE_MEM					0x0600

#define	ASM_NODE_DIRECT_ADDRESS			0x0700






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




#define	ASM_INS_AST_NODE_INS_SIZE		4
typedef struct {
	size_t			count;
	Instruction 	*ins[ASM_INS_AST_NODE_INS_SIZE];

	struct {
		uint8_t		len;		// Opcode的长度。值：1、2、3。
		uint8_t		o1;
		uint8_t		o2;
		uint8_t		o3;
	} opcode;
} AsmParserInsASTNodeData;

#define	ASM_INS_AST_NODE_DATA(node)				((AsmParserInsASTNodeData *)&((node)->data[0]))
#define	ASM_INS_AST_NODE_GET_COUNT(node)		(ASM_INS_AST_NODE_DATA(node)->count)
#define	ASM_INS_AST_NODE_ADD_INS(node, v)	\
	(ASM_INS_AST_NODE_GET_COUNT(node) >= ASM_INS_AST_NODE_INS_SIZE ?	\
		NULL :	\
		(ASM_INS_AST_NODE_DATA(node)->ins[ASM_INS_AST_NODE_GET_COUNT(node)++] = (v)))
#define	ASM_INS_AST_NODE_GET_INS(node, v)	\
	((v) >= ASM_INS_AST_NODE_GET_COUNT(node) ?	\
		NULL :	\
		(ASM_INS_AST_NODE_DATA(node)->ins[(v)]))

#define	ASM_INS_AST_NODE_SET_OPCODE_LEN(node, v)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.len = (v))
#define	ASM_INS_AST_NODE_GET_OPCODE_LEN(node)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.len)

#define	ASM_INS_AST_NODE_SET_OPCODE_O1(node, v)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o1 = (v))
#define	ASM_INS_AST_NODE_GET_OPCODE_O1(node)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o1)

#define	ASM_INS_AST_NODE_SET_OPCODE_O2(node, v)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o2 = (v))
#define	ASM_INS_AST_NODE_GET_OPCODE_O2(node)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o2)

#define	ASM_INS_AST_NODE_SET_OPCODE_O3(node, v)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o3 = (v))
#define	ASM_INS_AST_NODE_GET_OPCODE_O3(node)	\
	(ASM_INS_AST_NODE_DATA(node)->opcode.o3)







typedef struct {
	InsRegister		*reg;
} AsmParserRegASTNodeData;

#define	ASM_REG_AST_NODE_GET_REG(node)			(((AsmParserRegASTNodeData *)&((node)->data[0]))->reg)

#define	ASM_REG_AST_NODE_SET_REG(node, v)		(((AsmParserRegASTNodeData *)&((node)->data[0]))->reg = (v))



#define ASM_MEM_ADDR_SIZE_UNKNOWN	0
#define	ASM_MEM_ADDR_SIZE_16		1
#define	ASM_MEM_ADDR_SIZE_32		2
#define	ASM_MEM_ADDR_SIZE_64		3

#define	ASM_MEM_TYPE_UNKNOWN	0
#define	ASM_MEM_TYPE_BYTE		1
#define	ASM_MEM_TYPE_WORD		2
#define	ASM_MEM_TYPE_DWORD		3
#define	ASM_MEM_TYPE_QWORD		4

typedef struct {
	// ASM_MEM_ADDR_SIZE_*
	int				addr_size;

	// ASM_MEM_TYPE_*
	int				type;

	// INS_AM_*
	int				seg;

	// INS_AM_*
	// Base
	int				reg1;

	// INS_AM_*
	// Index
	int				reg2;

	// addr_size为ASM_MEM_ADDR_SIZE_32和ASM_MEM_ADDR_SIZE_64时可用。
	// Scale
	ParserASTNode	*node_scale;
	uint8_t			scale;

	ParserASTNode	*node_disp;
	uint64_t		disp;

	int				mod;		// 00B 01B 10B
								// 0   1   2
	int				rm;			// 0 ~ 7

	int				sib_base;	// -1无效。
	int				sib_index;	// -1无效。
	int				sib_ss;		// -1无效。
} AsmParserMemASTNodeData;

#define	ASM_MEM_AST_NODE_GET_ADDR_SIZE(node)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->addr_size)
#define	ASM_MEM_AST_NODE_GET_TYPE(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->type)
#define	ASM_MEM_AST_NODE_GET_SEG(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->seg)
#define	ASM_MEM_AST_NODE_GET_REG1(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->reg1)
#define	ASM_MEM_AST_NODE_GET_REG2(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->reg2)
#define	ASM_MEM_AST_NODE_GET_NODE_DISP(node)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->node_disp)
#define	ASM_MEM_AST_NODE_GET_DISP(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->disp)
#define	ASM_MEM_AST_NODE_GET_NODE_SCALE(node)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->node_scale)
#define	ASM_MEM_AST_NODE_GET_SCALE(node)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->scale)
#define	ASM_MEM_AST_NODE_GET_MOD(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->mod)
#define	ASM_MEM_AST_NODE_GET_RM(node)				(((AsmParserMemASTNodeData *)&((node)->data[0]))->rm)
#define	ASM_MEM_AST_NODE_GET_SIB_BASE(node)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_base)
#define	ASM_MEM_AST_NODE_GET_SIB_INDEX(node)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_index)
#define	ASM_MEM_AST_NODE_GET_SIB_SS(node)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_ss)

#define	ASM_MEM_AST_NODE_SET_ADDR_SIZE(node, v)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->addr_size = (v))
#define	ASM_MEM_AST_NODE_SET_TYPE(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->type = (v))
#define	ASM_MEM_AST_NODE_SET_SEG(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->seg = (v))
#define	ASM_MEM_AST_NODE_SET_REG1(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->reg1 = (v))
#define	ASM_MEM_AST_NODE_SET_REG2(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->reg2 = (v))
#define	ASM_MEM_AST_NODE_SET_NODE_DISP(node, v)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->node_disp = (v))
#define	ASM_MEM_AST_NODE_SET_DISP(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->disp = (v))
#define	ASM_MEM_AST_NODE_SET_NODE_SCALE(node, v)	(((AsmParserMemASTNodeData *)&((node)->data[0]))->node_scale = (v))
#define	ASM_MEM_AST_NODE_SET_SCALE(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->scale = (v))
#define	ASM_MEM_AST_NODE_SET_MOD(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->mod = (v))
#define	ASM_MEM_AST_NODE_SET_RM(node, v)			(((AsmParserMemASTNodeData *)&((node)->data[0]))->rm = (v))
#define	ASM_MEM_AST_NODE_SET_SIB_BASE(node, v)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_base = (v))
#define	ASM_MEM_AST_NODE_SET_SIB_INDEX(node, v)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_index = (v))
#define	ASM_MEM_AST_NODE_SET_SIB_SS(node, v)		(((AsmParserMemASTNodeData *)&((node)->data[0]))->sib_ss = (v))




typedef struct {
	// ASM_MEM_ADDR_SIZE_*
	int				addr_size;

	ParserASTNode	*node_base;
	uint64_t		base;

	ParserASTNode	*node_offset;
	uint64_t		offset;
} AsmParserDirectAddressASTNodeData;

#define	ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)	\
	((AsmParserDirectAddressASTNodeData *)&((node)->data[0]))

#define	ASM_DIRECT_ADDRESS_AST_NODE_GET_ADDR_SIZE(node)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->addr_size)
#define	ASM_DIRECT_ADDRESS_AST_NODE_GET_NODE_BASE(node)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->node_base)
#define	ASM_DIRECT_ADDRESS_AST_NODE_GET_BASE(node)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->base)
#define	ASM_DIRECT_ADDRESS_AST_NODE_GET_NODE_OFFSET(node)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->node_offset)
#define	ASM_DIRECT_ADDRESS_AST_NODE_GET_OFFSET(node)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->offset)

#define	ASM_DIRECT_ADDRESS_AST_NODE_SET_ADDR_SIZE(node, v)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->addr_size = (v))
#define	ASM_DIRECT_ADDRESS_AST_NODE_SET_NODE_BASE(node, v)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->node_base = (v))
#define	ASM_DIRECT_ADDRESS_AST_NODE_SET_BASE(node, v)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->base = (v))
#define	ASM_DIRECT_ADDRESS_AST_NODE_SET_NODE_OFFSET(node, v)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->node_offset = (v))
#define	ASM_DIRECT_ADDRESS_AST_NODE_SET_OFFSET(node, v)	\
	(ASM_DIRECT_ADDRESS_AST_NODE_DATA(node)->offset = (v))




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





#define	ASM_ARCH_BIT16		0x01	// 0b0001
#define	ASM_ARCH_BIT32		0x02	// 0b0010
#define	ASM_ARCH_BIT64		0x04	// 0b0100

#define	ASM_STEP_SCAN		1
#define	ASM_STEP_ENCODE		2

typedef struct {
	Map				symtable;
	int				arch;
	FILE			*out;
	uint64_t		address_counter;
	int				step;

	bool			mem_without_oprd_size;
	bool			default_oprd_size;
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
#define	ASM_PARSER_CONTEXT_DATA_GET_MEM_WITHOUT_OPRD_SIZE(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->mem_without_oprd_size)
#define	ASM_PARSER_CONTEXT_DATA_GET_DEFAULT_OPRD_SIZE(ctx)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->default_oprd_size)

#define	ASM_PARSER_CONTEXT_DATA_SET_ARCH(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->arch = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_OUT(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->out = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_ADDRESS_COUNTER(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->address_counter = (v))
#define	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->address_counter += (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_STEP(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->step = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_MEM_WITHOUT_OPRD_SIZE(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->mem_without_oprd_size = (v))
#define	ASM_PARSER_CONTEXT_DATA_SET_DEFAULT_OPRD_SIZE(ctx, v)	\
	(((AsmParserContextData *)&((ctx)->data[0]))->default_oprd_size = (v))



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

extern uint32_t asm_parser_get_uint32_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern uint64_t asm_parser_get_uint64_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern float asm_parser_get_float_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern double asm_parser_get_double_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern char asm_parser_get_char_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern bool asm_parser_has_unsigned_mark(
	ParserContext *ctx,
	ParserASTNode *node
);
extern bool asm_parser_has_float_mark(
	ParserContext *ctx,
	ParserASTNode *node
);

#endif
