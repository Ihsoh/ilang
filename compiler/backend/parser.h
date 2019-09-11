#ifndef __COMPILER_BACKEND_PARSER_H__
#define __COMPILER_BACKEND_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

#define	BE_NODE_INVALID				0x0000

#define	BE_NODE_MODULE				0x0100

#define	BE_NODE_IDENTIFIER			0x0200

#define	BE_NODE_LITERAL_UINT		0x0300
#define	BE_NODE_LITERAL_REAL		0x0301
#define	BE_NODE_LITERAL_CHAR		0x0302
#define	BE_NODE_LITERAL_STRING		0x0303

#define	BE_NODE_U8CONST				0x0304
#define	BE_NODE_U16CONST			0x0305
#define	BE_NODE_U32CONST			0x0306
#define	BE_NODE_U64CONST			0x0307
#define	BE_NODE_I8CONST				0x0308
#define	BE_NODE_I16CONST			0x0309
#define	BE_NODE_I32CONST			0x030a
#define	BE_NODE_I64CONST			0x030b
#define	BE_NODE_FCONST				0x030c
#define	BE_NODE_DCONST				0x030d






#define	BE_NODE_TYPE				0x0400

#define	BE_NODE_TYPE_CHAR			0x0400
#define	BE_NODE_TYPE_INT8			0x0401
#define	BE_NODE_TYPE_INT16			0x0402
#define	BE_NODE_TYPE_INT32			0x0403
#define	BE_NODE_TYPE_INT64			0x0404
#define	BE_NODE_TYPE_UINT8			0x0405
#define	BE_NODE_TYPE_UINT16			0x0406
#define	BE_NODE_TYPE_UINT32			0x0407
#define	BE_NODE_TYPE_UINT64			0x0408
#define	BE_NODE_TYPE_FLOAT			0x0409
#define	BE_NODE_TYPE_DOUBLE			0x040a

#define	BE_NODE_TYPE_VA_LIST		0x040b

#define	BE_NODE_TYPE_STRUCT			0x0410

#define	BE_NODE_TYPE_ARRAY			0x0420
#define	BE_NODE_TYPE_ARRAY_DIMS		0x0421

#define	BE_NODE_TYPE_FUNC			0x0430

#define	BE_NODE_TYPE_POINTER		0x0440

#define	BE_NODE_TYPE_VOID			0x04ff


#define	BE_NODE_DUMMY				0x0900




#define	BE_NODE_FUNC						0x0a00
#define	BE_NODE_FUNC_PARAMS					0x0a01
#define	BE_NODE_FUNC_PARAMS_ITEM			0x0a02
#define	BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM	0x0a03




#define	BE_NODE_STATS_BLOCK					0x0b00

#define	BE_NODE_STAT_BR						0x0b10

#define	BE_NODE_STAT_RETURN					0x0b20

#define	BE_NODE_STAT_LABEL					0x0b30

#define	BE_NODE_STAT_ASM					0x0b40
#define	BE_NODE_STAT_ASM_SET_REG			0x0b41
#define	BE_NODE_STAT_ASM_GET_REG			0x0b42

#define	BE_NODE_STAT_EXPR					0x0b50

#define	BE_NODE_STAT_DUMMY					0x0b60

#define	BE_NODE_STAT_VA_START				0x0b70
#define	BE_NODE_STAT_VA_END					0x0b71
#define	BE_NODE_STAT_VA_COPY				0x0b72
#define	BE_NODE_STAT_VA_ARG					0x0b73

#define	BE_NODE_STAT_STORE					0x0b80
#define	BE_NODE_STAT_LOAD					0x0b81

#define	BE_NODE_STAT_ASSIGN					0x0b90

#define	BE_NODE_STAT_CBR					0x0ba0

#define	BE_NODE_STAT_XXX					0x0bb0	// TODO: XXX

#define	BE_NODE_STAT_TRUNC					0x0bc0
#define	BE_NODE_STAT_SEXT					0x0bc1
#define	BE_NODE_STAT_SITOFP					0x0bc2
#define	BE_NODE_STAT_INTTOPTR				0x0bc3
#define	BE_NODE_STAT_ZEXT					0x0bc4
#define	BE_NODE_STAT_UITOFP					0x0bc5
#define	BE_NODE_STAT_FPTOSI					0x0bc6
#define	BE_NODE_STAT_FPTOUI					0x0bc7
#define	BE_NODE_STAT_FPEXT					0x0bc8
#define	BE_NODE_STAT_FPTRUNC				0x0bc9
#define	BE_NODE_STAT_PTRTOINT				0x0bca
#define	BE_NODE_STAT_BITCAST				0x0bcb

#define	BE_NODE_STAT_CALL					0x0bd0
#define	BE_NODE_STAT_VCALL					0x0bd1
#define	BE_NODE_STAT_FUNC_CALL				0x0bd2
#define	BE_NODE_STAT_FUNC_CALL_PARAMS		0x0bd3

#define	BE_NODE_STAT_ADD					0x0be0
#define	BE_NODE_STAT_SUB					0x0be1
#define	BE_NODE_STAT_MUL					0x0be2
#define	BE_NODE_STAT_DIV					0x0be3
#define	BE_NODE_STAT_REM					0x0be4
#define	BE_NODE_STAT_MBR					0x0be5
#define	BE_NODE_STAT_IDX					0x0be6
#define	BE_NODE_STAT_NOT					0x0be7
#define	BE_NODE_STAT_NEG					0x0be8
#define	BE_NODE_STAT_BNOT					0x0be9
#define	BE_NODE_STAT_REF					0x0bea
#define	BE_NODE_STAT_SHL					0x0beb
#define	BE_NODE_STAT_SHR					0x0bec

#define	BE_NODE_STAT_EQ						0x0bed
#define	BE_NODE_STAT_NEQ					0x0bee
#define	BE_NODE_STAT_LT						0x0bef
#define	BE_NODE_STAT_LE						0x0bf0
#define	BE_NODE_STAT_GT						0x0bf1
#define	BE_NODE_STAT_GE						0x0bf2

#define	BE_NODE_STAT_BAND					0x0bf3
#define	BE_NODE_STAT_BOR					0x0bf4
#define	BE_NODE_STAT_BXOR					0x0bf5





#define	BE_NODE_VAR							0x0c00
#define	BE_NODE_VAR_ITEM					0x0c01




#define	BE_NODE_STRUCT						0x0d00
#define	BE_NODE_STRUCT_BODY					0x0d01




#define	BE_NODE_EXPR						0x0e00

#define	BE_NODE_EXPR_OR						0x0e20

#define	BE_NODE_EXPR_AND					0x0e30

#define	BE_NODE_EXPR_BOR					0x0e40

#define	BE_NODE_EXPR_BXOR					0x0e50

#define	BE_NODE_EXPR_BAND					0x0e60

#define	BE_NODE_EXPR_EQ						0x0e70
#define	BE_NODE_EXPR_NEQ					0x0e71

#define	BE_NODE_EXPR_LT						0x0e80
#define	BE_NODE_EXPR_LE						0x0e81
#define	BE_NODE_EXPR_GT						0x0e82
#define	BE_NODE_EXPR_GE						0x0e83

#define	BE_NODE_EXPR_SHIFT_LEFT				0x0e90
#define	BE_NODE_EXPR_SHIFT_RIGHT			0x0e91

#define	BE_NODE_EXPR_ADD					0x0ea0
#define	BE_NODE_EXPR_SUB					0x0ea1

#define	BE_NODE_EXPR_MUL					0x0eb0
#define	BE_NODE_EXPR_DIV					0x0eb1
#define	BE_NODE_EXPR_MOD					0x0eb2

#define	BE_NODE_EXPR_NOT					0x0ec0
#define	BE_NODE_EXPR_POSITIVE				0x0ec1
#define	BE_NODE_EXPR_NEGATIVE				0x0ec2
#define	BE_NODE_EXPR_BNOT					0x0ec3
#define	BE_NODE_EXPR_SIZEOF					0x0ec6
#define	BE_NODE_EXPR_ALIGNOF				0x0ec7
#define	BE_NODE_EXPR_CAST					0x0ec8
#define	BE_NODE_EXPR_COND					0x0ecd

#define	BE_NODE_EXPR_ATOM					0x0ee0








#define	BE_NODE_ATTR_ALIGN					0x0f00
#define	BE_NODE_ATTR_PACKED					0x0f01




#define BE_SYM_VAR					0x01
#define	BE_SYM_STRUCT				0x02
#define	BE_SYM_FUNC					0x03
#define	BE_SYM_LABEL				0x04
#define	BE_SYM_STRUCT_MEMBER_VAR	0x05




#define	BE_TYPE_UNKNOWN				0x00
#define	BE_TYPE_CHAR				0x01
#define	BE_TYPE_INT8				0x02
#define	BE_TYPE_INT16				0x03
#define	BE_TYPE_INT32				0x04
#define	BE_TYPE_INT64				0x05
#define	BE_TYPE_UINT8				0x06
#define	BE_TYPE_UINT16				0x07
#define	BE_TYPE_UINT32				0x08
#define	BE_TYPE_UINT64				0x09
#define	BE_TYPE_FLOAT				0x0a
#define	BE_TYPE_DOUBLE				0x0b

#define	BE_TYPE_VA_LIST				0x0c

#define	BE_TYPE_STRUCT				0x0d

#define	BE_TYPE_ARRAY				0x0e

#define	BE_TYPE_FUNC				0x0f

#define	BE_TYPE_POINTER				0x10

#define	BE_TYPE_VOID				0xff




#define	BE_IS_TYPE_NODE(type)		(((type) & 0xff00) == BE_NODE_TYPE)
#define	BE_IS_EXPR_NODE(type)		(((type) & 0xff00) == BE_NODE_EXPR)





#define	BE_ARCH_32		1
#define	BE_ARCH_64		2




typedef struct {
	ParserASTNode		*align_node;
	ParserASTNode		*packed_node;
} BeParserContextData;

#define	BE_PARSER_CONTEXT_DATA_SET_ALIGN_NODE(ctx, n)	\
	(((BeParserContextData *)&((ctx)->data[0]))->align_node = (n))
#define	BE_PARSER_CONTEXT_DATA_SET_PACKED_NODE(ctx, n)	\
	(((BeParserContextData *)&((ctx)->data[0]))->packed_node = (n))

#define	BE_PARSER_CONTEXT_DATA_GET_ALIGN_NODE(ctx)	\
	(((BeParserContextData *)&((ctx)->data[0]))->align_node)
#define	BE_PARSER_CONTEXT_DATA_GET_PACKED_NODE(ctx)	\
	(((BeParserContextData *)&((ctx)->data[0]))->packed_node)




typedef struct {
	const char 	*str;
	size_t		len;
} BeParserConstexprString;

typedef struct {
	uint8_t							type;
	ParserASTNode					*type_node;

	bool							constant;
	uint8_t							constant_type;

	bool							has_conststr;
	BeParserConstexprString			conststr;

	union {
		char						char_val;

		int8_t						int8_val;
		int16_t						int16_val;
		int32_t						int32_val;
		int64_t						int64_val;

		uint8_t						uint8_val;
		uint16_t					uint16_val;
		uint32_t					uint32_val;
		uint64_t					uint64_val;
		
		float						float_val;
		double						double_val;

		uint64_t					ptr_val;
	} constexpr_result;	
} BeParserExprASTNodeData;

#define	BE_EXPR_AST_NODE_GET_TYPE(node)				(((BeParserExprASTNodeData *)&((node)->data[0]))->type)
#define	BE_EXPR_AST_NODE_GET_TYPE_NODE(node)		(((BeParserExprASTNodeData *)&((node)->data[0]))->type_node)
#define	BE_EXPR_AST_NODE_GET_CONSTANT(node)			(((BeParserExprASTNodeData *)&((node)->data[0]))->constant)
#define	BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)	(((BeParserExprASTNodeData *)&((node)->data[0]))->constant_type)


#define	BE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node)		(((BeParserExprASTNodeData *)&((node)->data[0]))->has_conststr)
#define	BE_EXPR_AST_NODE_GET_CONSTSTR(node)			(&(((BeParserExprASTNodeData *)&((node)->data[0]))->conststr))


#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.char_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int8_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int16_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int32_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int64_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint8_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint16_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint32_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint64_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.float_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.double_val)
#define BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node)	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.ptr_val)


#define	BE_EXPR_AST_NODE_SET_TYPE(node, t)				(((BeParserExprASTNodeData *)&((node)->data[0]))->type = (t))
#define	BE_EXPR_AST_NODE_SET_TYPE_NODE(node, tnode)		(((BeParserExprASTNodeData *)&((node)->data[0]))->type_node = (tnode))
#define	BE_EXPR_AST_NODE_SET_CONSTANT(node, c)			(((BeParserExprASTNodeData *)&((node)->data[0]))->constant = (c))
#define	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE(node, ct)	(((BeParserExprASTNodeData *)&((node)->data[0]))->constant_type = (ct))


#define	BE_EXPR_AST_NODE_SET_HAS_CONSTSTR(node, hc)	(((BeParserExprASTNodeData *)&((node)->data[0]))->has_conststr = (hc))


#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_CHAR_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.char_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_CHAR);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT8_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int8_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_INT8);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT16_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int16_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_INT16);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int32_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_INT32);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int64_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_INT64);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT8_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint8_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_UINT8);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT16_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint16_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_UINT16);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint32_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_UINT32);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint64_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_UINT64);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_FLOAT_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.float_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_FLOAT);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_DOUBLE_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.double_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_DOUBLE);	\
}
#define	BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(node, v) {	\
	(((BeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.ptr_val = (v));	\
	BE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), BE_TYPE_POINTER);	\
}


#define	BE_EXPR_AST_NODE_COPY_CONSTANT(target, source)	(BE_EXPR_AST_NODE_SET_CONSTANT((target), BE_EXPR_AST_NODE_GET_CONSTANT((source))))

#define	BE_EXPR_AST_NODE_COPY_CONSTANT2(target, source1, source2)	\
	(BE_EXPR_AST_NODE_SET_CONSTANT(	\
		(target),	\
		BE_EXPR_AST_NODE_GET_CONSTANT((source1)) && BE_EXPR_AST_NODE_GET_CONSTANT((source2))	\
	))

#define	BE_EXPR_AST_NODE_COPY_CONSTANT3(target, source1, source2, source3)	\
	(BE_EXPR_AST_NODE_SET_CONSTANT(	\
		(target),	\
		BE_EXPR_AST_NODE_GET_CONSTANT((source1)) && BE_EXPR_AST_NODE_GET_CONSTANT((source2)) && BE_EXPR_AST_NODE_GET_CONSTANT((source3))	\
	))

#define	BE_EXPR_AST_NODE_COPY(target, source)		(memcpy(&((target)->data), &((source)->data), sizeof(BeParserExprASTNodeData)))




#define	BE_VAR_TYPE_UNKNOWN				0
#define	BE_VAR_TYPE_GLOBAL				1
#define	BE_VAR_TYPE_LOCAL				2
#define	BE_VAR_TYPE_FUNC_PARAM			3
#define	BE_VAR_TYPE_STRUCT_MEMBER		4

typedef struct {
	/*
		标识变量的作用域。

		BE_VAR_TYPE_GLOBAL：全局变量
		BE_VAR_TYPE_LOCAL：局部变量
		BE_VAR_TYPE_FUNC_PARAM：函数参数
		BE_VAR_TYPE_STRUCT_MEMBER：结构体成员变量
	*/
	uint8_t			type;

	ParserASTNode	*align_node;
	int				align;
} BeParserVarASTNodeData;

#define	BE_VAR_AST_NODE_GET_TYPE(node)					(((BeParserVarASTNodeData *)&((node)->data[0]))->type)
#define	BE_VAR_AST_NODE_GET_ALIGN_NODE(node)			(((BeParserVarASTNodeData *)&((node)->data[0]))->align_node)
#define	BE_VAR_AST_NODE_GET_ALIGN(node)					(((BeParserVarASTNodeData *)&((node)->data[0]))->align)

#define	BE_VAR_AST_NODE_SET_TYPE(node, t)				(((BeParserVarASTNodeData *)&((node)->data[0]))->type = (t))
#define	BE_VAR_AST_NODE_SET_ALIGN_NODE(node, n)			(((BeParserVarASTNodeData *)&((node)->data[0]))->align_node = (n))
#define	BE_VAR_AST_NODE_SET_ALIGN(node, a)				(((BeParserVarASTNodeData *)&((node)->data[0]))->align = (a))




typedef struct {
	ParserSymbol	*symbol;
} BeParserVarItemASTNodeData;

#define	BE_VAR_ITEM_AST_NODE_GET_SYMBOL(node)			(((BeParserVarItemASTNodeData *)&((node)->data[0]))->symbol)

#define	BE_VAR_ITEM_AST_NODE_SET_SYMBOL(node, s)		(((BeParserVarItemASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ParserSymbol	*symbol;
} BeParserFuncASTNodeData;

#define	BE_FUNC_AST_NODE_GET_SYMBOL(node)		(((BeParserFuncASTNodeData *)&((node)->data[0]))->symbol)

#define	BE_FUNC_AST_NODE_SET_SYMBOL(node, s)	(((BeParserFuncASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ParserSymbol	*symbol;
} BeParserFuncParamASTNodeData;

#define	BE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node)		(((BeParserFuncParamASTNodeData *)&((node)->data[0]))->symbol)

#define	BE_FUNC_PARAM_AST_NODE_SET_SYMBOL(node, s)	(((BeParserFuncParamASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ResizableString label_repeat;
	ResizableString label_end;
} BeParserLoopASTNodeData;

#define	BE_LOOP_AST_NODE_GET_LABEL_REPEAT(node)		(&(((BeParserLoopASTNodeData *)&((node)->data[0]))->label_repeat))
#define	BE_LOOP_AST_NODE_GET_LABEL_END(node)		(&(((BeParserLoopASTNodeData *)&((node)->data[0]))->label_end))




typedef struct {
	ParserASTNode		*packed_node;
	bool				packed;
} BeParserStructASTNodeData;

#define	BE_STRUCT_AST_NODE_GET_PACKED_NODE(node)		(((BeParserStructASTNodeData *)&((node)->data[0]))->packed_node)
#define	BE_STRUCT_AST_NODE_GET_PACKED(node)				(((BeParserStructASTNodeData *)&((node)->data[0]))->packed)

#define	BE_STRUCT_AST_NODE_SET_PACKED_NODE(node, v)		(((BeParserStructASTNodeData *)&((node)->data[0]))->packed_node = (v))
#define	BE_STRUCT_AST_NODE_SET_PACKED(node, v)			(((BeParserStructASTNodeData *)&((node)->data[0]))->packed = (v))




extern ParserASTNode * be_parser_new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
);

extern ParserContext * be_parser_new_context(
	const char *file,
	const char *source,
	int len,
	int arch
);

extern void be_parser_free_context(ParserContext * ctx);

extern void be_parser_parse(ParserContext * ctx);

extern void be_parser_print_ast(ParserContext * ctx, FILE *file);

extern uint32_t be_parser_get_uint32_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern uint64_t be_parser_get_uint64_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern uint64_t be_parser_get_uint_val(
	ParserContext *ctx,
	ParserASTNode *node
);

extern float be_parser_get_float_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern double be_parser_get_double_val(
	ParserContext *ctx,
	ParserASTNode *node
);

extern char be_parser_get_char_val(
	ParserContext *ctx,
	ParserASTNode *node
);




typedef struct {
	uint8_t				type;
	ParserASTNode		*type_node;

	ParserSymbol		*func_symbol;

	// 只有当has_code_gen_name为true时，code_gen_name才有意义。
	bool				has_code_gen_name;
	ResizableString		code_gen_name;

	int					align;

	// 只有局部变量有效，虚拟的栈空间地址，从0开始计数。
	// 当值为SIZE_MAX时，该字段无效，未初始化。
	size_t				address;

	// 变量类型的大小，当值为0时该字段无效，未初始化。
	size_t				type_size;

	// 0	BE_VAR_TYPE_UNKNOWN
	// 1	BE_VAR_TYPE_GLOBAL
	// 2	BE_VAR_TYPE_LOCAL
	// 3	BE_VAR_TYPE_STRUCT_MEMBER
	uint8_t				scope_type;
} BeParserVarSymbolData;

#define	BE_VAR_SYMBOL_GET_TYPE(symbol)					(((BeParserVarSymbolData *)&((symbol)->data[0]))->type)
#define	BE_VAR_SYMBOL_GET_TYPE_NODE(symbol)				(((BeParserVarSymbolData *)&((symbol)->data[0]))->type_node)
#define	BE_VAR_SYMBOL_GET_FUNC_SYMBOL(symbol)			(((BeParserVarSymbolData *)&((symbol)->data[0]))->func_symbol)
#define	BE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol)		(((BeParserVarSymbolData *)&((symbol)->data[0]))->has_code_gen_name)
#define	BE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol)			(&(((BeParserVarSymbolData *)&((symbol)->data[0]))->code_gen_name))
#define	BE_VAR_SYMBOL_GET_ALIGN(symbol)					(((BeParserVarSymbolData *)&((symbol)->data[0]))->align)
#define	BE_VAR_SYMBOL_GET_ADDRESS(symbol)				(((BeParserVarSymbolData *)&((symbol)->data[0]))->address)
#define	BE_VAR_SYMBOL_GET_TYPE_SIZE(symbol)				(((BeParserVarSymbolData *)&((symbol)->data[0]))->type_size)
#define	BE_VAR_SYMBOL_GET_SCOPE_TYPE(symbol)			(((BeParserVarSymbolData *)&((symbol)->data[0]))->scope_type)

#define	BE_VAR_SYMBOL_SET_FUNC_SYMBOL(symbol, v)		(((BeParserVarSymbolData *)&((symbol)->data[0]))->func_symbol = (v))
#define	BE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, v)	(((BeParserVarSymbolData *)&((symbol)->data[0]))->has_code_gen_name = (v))
#define	BE_VAR_SYMBOL_SET_ALIGN(symbol, v)				(((BeParserVarSymbolData *)&((symbol)->data[0]))->align = (v))
#define	BE_VAR_SYMBOL_SET_ADDRESS(symbol, v)			(((BeParserVarSymbolData *)&((symbol)->data[0]))->address = (v))
#define	BE_VAR_SYMBOL_SET_TYPE_SIZE(symbol, v)			(((BeParserVarSymbolData *)&((symbol)->data[0]))->type_size = (v))
#define	BE_VAR_SYMBOL_SET_SCOPE_TYPE(symbol, v)			(((BeParserVarSymbolData *)&((symbol)->data[0]))->scope_type = (v))




extern ParserSymbol * be_parser_add_var_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node,
	size_t type_size
);




typedef struct {
	uint8_t			type;
	ParserASTNode	*type_node;
	size_t			offset;
} BeParserStructMemberVarSymbolData;

#define	BE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE(symbol)		(((BeParserStructMemberVarSymbolData *)&((symbol)->data[0]))->type)
#define	BE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE_NODE(symbol)	(((BeParserStructMemberVarSymbolData *)&((symbol)->data[0]))->type_node)
#define	BE_STRUCT_MEMBER_VAR_SYMBOL_GET_OFFSET(symbol)		(((BeParserStructMemberVarSymbolData *)&((symbol)->data[0]))->offset)

extern ParserSymbol * be_parser_add_struct_member_var_symbol_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node,
	size_t offset
);




typedef struct {
	ParserASTNode	*struct_node;
	ParserASTNode	*body_node;
	bool			dummy;
} BeParserStructSymbolData;

#define	BE_STRUCT_SYMBOL_GET_STRUCT_NODE(symbol)		(((BeParserStructSymbolData *)&((symbol)->data[0]))->struct_node)
#define	BE_STRUCT_SYMBOL_SET_STRUCT_NODE(symbol, v)		(((BeParserStructSymbolData *)&((symbol)->data[0]))->struct_node = (v))
#define	BE_STRUCT_SYMBOL_GET_BODY_NODE(symbol)			(((BeParserStructSymbolData *)&((symbol)->data[0]))->body_node)
#define	BE_STRUCT_SYMBOL_SET_BODY_NODE(symbol, bn)		(((BeParserStructSymbolData *)&((symbol)->data[0]))->body_node = (bn))
#define	BE_STRUCT_SYMBOL_GET_DUMMY(symbol)				(((BeParserStructSymbolData *)&((symbol)->data[0]))->dummy)
#define	BE_STRUCT_SYMBOL_SET_DUMMY(symbol, d)			(((BeParserStructSymbolData *)&((symbol)->data[0]))->dummy = (d))

extern ParserSymbol * be_parser_new_struct_symbol(
	ParserContext *ctx,
	LexerToken *token,
	ParserASTNode *struct_node,
	ParserASTNode *body_node,
	bool dummy
);

extern ParserSymbol * be_parser_add_struct_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	ParserASTNode *body_node,
	bool dummy
);




typedef struct {
	ParserASTNode 	*params_node;
	ParserASTNode 	*return_type_node;
	ParserASTNode	*func_type_node;
	ParserASTNode 	*func_pointer_type_node;
	bool			dummy;

	unsigned int	counter;

	ParserASTNode	*func_name_node;
} BeParserFuncSymbolData;

#define	BE_FUNC_SYMBOL_GET_PARAMS_NODE(symbol)				(((BeParserFuncSymbolData *)&((symbol)->data[0]))->params_node)
#define	BE_FUNC_SYMBOL_GET_RETURN_TYPE_NODE(symbol)			(((BeParserFuncSymbolData *)&((symbol)->data[0]))->return_type_node)
#define	BE_FUNC_SYMBOL_GET_FUNC_TYPE_NODE(symbol)			(((BeParserFuncSymbolData *)&((symbol)->data[0]))->func_type_node)
#define	BE_FUNC_SYMBOL_GET_FUNC_POINTER_TYPE_NODE(symbol)	(((BeParserFuncSymbolData *)&((symbol)->data[0]))->func_pointer_type_node)
#define	BE_FUNC_SYMBOL_GET_DUMMY(symbol)					(((BeParserFuncSymbolData *)&((symbol)->data[0]))->dummy)
#define	BE_FUNC_SYMBOL_GET_FUNC_NAME_NODE(symbol)			(((BeParserFuncSymbolData *)&((symbol)->data[0]))->func_name_node)

#define	BE_FUNC_SYMBOL_SET_DUMMY(symbol, d)					(((BeParserFuncSymbolData *)&((symbol)->data[0]))->dummy = (d))
#define	BE_FUNC_SYMBOL_SET_FUNC_NAME_NODE(symbol, n)		(((BeParserFuncSymbolData *)&((symbol)->data[0]))->func_name_node = (n))

#define	BE_FUNC_SYMBOL_NEXT_NO(symbol)						(((BeParserFuncSymbolData *)&((symbol)->data[0]))->counter++)

extern ParserSymbol * be_parser_add_func_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	ParserASTNode *params_node,
	ParserASTNode *return_type_node,
	bool dummy
);




#endif
