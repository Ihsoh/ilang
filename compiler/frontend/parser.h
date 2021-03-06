#ifndef	__COMPILER_FRONTEND_PARSER_H__
#define	__COMPILER_FRONTEND_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

#define	FE_NODE_INVALID				0x0000

#define	FE_NODE_MODULE				0x0100

#define	FE_NODE_IDENTIFIER			0x0200

#define	FE_NODE_LITERAL_UINT		0x0300
#define	FE_NODE_LITERAL_REAL		0x0301
#define	FE_NODE_LITERAL_CHAR		0x0302
#define	FE_NODE_LITERAL_STRING		0x0303

#define	FE_NODE_TYPE				0x0400

#define	FE_NODE_TYPE_CHAR			0x0400
#define	FE_NODE_TYPE_INT8			0x0401
#define	FE_NODE_TYPE_INT16			0x0402
#define	FE_NODE_TYPE_INT32			0x0403
#define	FE_NODE_TYPE_INT64			0x0404
#define	FE_NODE_TYPE_UINT8			0x0405
#define	FE_NODE_TYPE_UINT16			0x0406
#define	FE_NODE_TYPE_UINT32			0x0407
#define	FE_NODE_TYPE_UINT64			0x0408
#define	FE_NODE_TYPE_FLOAT			0x0409
#define	FE_NODE_TYPE_DOUBLE			0x040a

#define	FE_NODE_TYPE_VA_LIST		0x040b

#define	FE_NODE_TYPE_STRUCT			0x0410

#define	FE_NODE_TYPE_ARRAY			0x0420
#define	FE_NODE_TYPE_ARRAY_DIMS		0x0421

#define	FE_NODE_TYPE_FUNC			0x0430

#define	FE_NODE_TYPE_POINTER		0x0440

#define	FE_NODE_TYPE_VOID			0x04ff


#define	FE_NODE_DUMMY				0x0900

#define	FE_NODE_FUNC						0x0a00
#define	FE_NODE_FUNC_PARAMS					0x0a01
#define	FE_NODE_FUNC_PARAMS_ITEM			0x0a02
#define	FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM	0x0a03




#define	FE_NODE_STATS_BLOCK					0x0b00

#define	FE_NODE_STAT_IF						0x0b10
#define	FE_NODE_STAT_ELIF					0x0b11
#define	FE_NODE_STAT_ELSE					0x0b12

#define	FE_NODE_STAT_WHILE					0x0b20
#define	FE_NODE_STAT_DO						0x0b30

#define	FE_NODE_STAT_FOR					0x0b40
#define	FE_NODE_STAT_FOR_INIT				0x0b41
#define	FE_NODE_STAT_FOR_EXPR_COND			0x0b42
#define	FE_NODE_STAT_FOR_EXPR				0x0b43

#define	FE_NODE_STAT_CONTINUE				0x0b50
#define	FE_NODE_STAT_BREAK					0x0b60
#define	FE_NODE_STAT_RETURN					0x0b70
#define	FE_NODE_STAT_GOTO					0x0b80
#define	FE_NODE_STAT_LABEL					0x0b90

#define	FE_NODE_STAT_ASM					0x0ba0
#define	FE_NODE_STAT_ASM_OPTION				0x0ba1

#define	FE_NODE_STAT_EXPR					0x0bb0

#define	FE_NODE_STAT_DUMMY					0x0bc0

#define	FE_NODE_STAT_VA_START				0x0bd0
#define	FE_NODE_STAT_VA_END					0x0bd1
#define	FE_NODE_STAT_VA_COPY				0x0bd2




#define	FE_NODE_VAR							0x0c00
#define	FE_NODE_VAR_ITEM					0x0c01




#define	FE_NODE_STRUCT						0x0d00
#define	FE_NODE_STRUCT_BODY					0x0d01




#define	FE_NODE_EXPR						0x0e00

#define	FE_NODE_EXPR_ASSIGN					0x0e10
#define	FE_NODE_EXPR_ADD_ASSIGN				0x0e11
#define	FE_NODE_EXPR_SUB_ASSIGN				0x0e12
#define	FE_NODE_EXPR_MUL_ASSIGN				0x0e13
#define	FE_NODE_EXPR_DIV_ASSIGN				0x0e14
#define	FE_NODE_EXPR_MOD_ASSIGN				0x0e15
#define	FE_NODE_EXPR_BAND_ASSIGN			0x0e16
#define	FE_NODE_EXPR_BOR_ASSIGN				0x0e17
#define	FE_NODE_EXPR_BXOR_ASSIGN			0x0e18
#define	FE_NODE_EXPR_SHIFT_LEFT_ASSIGN		0x0e19
#define	FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN		0x0e1a

#define	FE_NODE_EXPR_OR						0x0e20

#define	FE_NODE_EXPR_AND					0x0e30

#define	FE_NODE_EXPR_BOR					0x0e40

#define	FE_NODE_EXPR_BXOR					0x0e50

#define	FE_NODE_EXPR_BAND					0x0e60

#define	FE_NODE_EXPR_EQ						0x0e70
#define	FE_NODE_EXPR_NEQ					0x0e71

#define	FE_NODE_EXPR_LT						0x0e80
#define	FE_NODE_EXPR_LE						0x0e81
#define	FE_NODE_EXPR_GT						0x0e82
#define	FE_NODE_EXPR_GE						0x0e83
#define	FE_NODE_EXPR_INSTANCEOF				0x0e84

#define	FE_NODE_EXPR_SHIFT_LEFT				0x0e90
#define	FE_NODE_EXPR_SHIFT_RIGHT			0x0e91

#define	FE_NODE_EXPR_ADD					0x0ea0
#define	FE_NODE_EXPR_SUB					0x0ea1

#define	FE_NODE_EXPR_MUL					0x0eb0
#define	FE_NODE_EXPR_DIV					0x0eb1
#define	FE_NODE_EXPR_MOD					0x0eb2

#define	FE_NODE_EXPR_NOT					0x0ec0
#define	FE_NODE_EXPR_POSITIVE				0x0ec1
#define	FE_NODE_EXPR_NEGATIVE				0x0ec2
#define	FE_NODE_EXPR_BNOT					0x0ec3
#define	FE_NODE_EXPR_INC_LEFT				0x0ec4
#define	FE_NODE_EXPR_DEC_LEFT				0x0ec5
#define	FE_NODE_EXPR_SIZEOF					0x0ec6
#define	FE_NODE_EXPR_ALIGNOF				0x0ec7
#define	FE_NODE_EXPR_CAST					0x0ec8
#define	FE_NODE_EXPR_REF					0x0ec9
#define	FE_NODE_EXPR_DEREF					0x0eca
#define	FE_NODE_EXPR_INC_RIGHT				0x0ecb
#define	FE_NODE_EXPR_DEC_RIGHT				0x0ecc
#define	FE_NODE_EXPR_COND					0x0ecd
#define	FE_NODE_EXPR_VA_ARG					0x0ece

#define	FE_NODE_EXPR_MEMBER					0x0ed0
#define	FE_NODE_EXPR_PMEMBER				0x0ed1
#define	FE_NODE_EXPR_ARRAY_INDEX			0x0ed2
#define	FE_NODE_EXPR_FUNC_CALL				0x0ed3
#define	FE_NODE_EXPR_FUNC_CALL_PARAMS		0x0ed4

#define	FE_NODE_EXPR_ATOM					0x0ee0

#define	FE_NODE_ATTR_ALIGN					0x0f00
#define	FE_NODE_ATTR_PACKED					0x0f01



#define FE_SYM_VAR					0x01
#define	FE_SYM_STRUCT				0x02
#define	FE_SYM_FUNC					0x03
#define	FE_SYM_LABEL				0x04
#define	FE_SYM_STRUCT_MEMBER_VAR	0x05




#define	FE_TYPE_UNKNOWN				0x00
#define	FE_TYPE_CHAR				0x01
#define	FE_TYPE_INT8				0x02
#define	FE_TYPE_INT16				0x03
#define	FE_TYPE_INT32				0x04
#define	FE_TYPE_INT64				0x05
#define	FE_TYPE_UINT8				0x06
#define	FE_TYPE_UINT16				0x07
#define	FE_TYPE_UINT32				0x08
#define	FE_TYPE_UINT64				0x09
#define	FE_TYPE_FLOAT				0x0a
#define	FE_TYPE_DOUBLE				0x0b

#define	FE_TYPE_VA_LIST				0x0c

#define	FE_TYPE_STRUCT				0x0d

#define	FE_TYPE_ARRAY				0x0e

#define	FE_TYPE_FUNC				0x0f

#define	FE_TYPE_POINTER				0x10

#define	FE_TYPE_VOID				0xff




#define	FE_IS_TYPE_NODE(type)		(((type) & 0xff00) == FE_NODE_TYPE)
#define	FE_IS_EXPR_NODE(type)		(((type) & 0xff00) == FE_NODE_EXPR)



#define	FE_ARCH_32		1
#define	FE_ARCH_64		2




typedef struct {
	ParserASTNode		*align_node;
	ParserASTNode		*packed_node;
} FeParserContextData;

#define	FE_PARSER_CONTEXT_DATA_SET_ALIGN_NODE(ctx, n)	\
	(((FeParserContextData *)&((ctx)->data[0]))->align_node = (n))
#define	FE_PARSER_CONTEXT_DATA_SET_PACKED_NODE(ctx, n)	\
	(((FeParserContextData *)&((ctx)->data[0]))->packed_node = (n))

#define	FE_PARSER_CONTEXT_DATA_GET_ALIGN_NODE(ctx)	\
	(((FeParserContextData *)&((ctx)->data[0]))->align_node)
#define	FE_PARSER_CONTEXT_DATA_GET_PACKED_NODE(ctx)	\
	(((FeParserContextData *)&((ctx)->data[0]))->packed_node)




typedef struct {
	const char 	*str;
	size_t		len;
} FeParserConstexprString;

typedef struct {
	uint8_t							type;
	ParserASTNode					*type_node;

	bool							constant;
	uint8_t							constant_type;

	bool							has_conststr;
	FeParserConstexprString			conststr;

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
} FeParserExprASTNodeData;



#define	FE_EXPR_AST_NODE_GET_TYPE(node)				(((FeParserExprASTNodeData *)&((node)->data[0]))->type)
#define	FE_EXPR_AST_NODE_GET_TYPE_NODE(node)		(((FeParserExprASTNodeData *)&((node)->data[0]))->type_node)
#define	FE_EXPR_AST_NODE_GET_CONSTANT(node)			(((FeParserExprASTNodeData *)&((node)->data[0]))->constant)
#define	FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)	(((FeParserExprASTNodeData *)&((node)->data[0]))->constant_type)


#define	FE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node)		(((FeParserExprASTNodeData *)&((node)->data[0]))->has_conststr)
#define	FE_EXPR_AST_NODE_GET_CONSTSTR(node)			(&(((FeParserExprASTNodeData *)&((node)->data[0]))->conststr))


#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.char_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int8_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int16_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int32_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int64_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint8_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint16_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint32_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint64_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.float_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.double_val)
#define FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node)	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.ptr_val)


#define	FE_EXPR_AST_NODE_SET_TYPE(node, t)				(((FeParserExprASTNodeData *)&((node)->data[0]))->type = (t))
#define	FE_EXPR_AST_NODE_SET_TYPE_NODE(node, tnode)		(((FeParserExprASTNodeData *)&((node)->data[0]))->type_node = (tnode))
#define	FE_EXPR_AST_NODE_SET_CONSTANT(node, c)			(((FeParserExprASTNodeData *)&((node)->data[0]))->constant = (c))
#define	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE(node, ct)	(((FeParserExprASTNodeData *)&((node)->data[0]))->constant_type = (ct))


#define	FE_EXPR_AST_NODE_SET_HAS_CONSTSTR(node, hc)	(((FeParserExprASTNodeData *)&((node)->data[0]))->has_conststr = (hc))


#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_CHAR_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.char_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_CHAR);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT8_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int8_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_INT8);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT16_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int16_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_INT16);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int32_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_INT32);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.int64_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_INT64);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT8_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint8_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_UINT8);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT16_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint16_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_UINT16);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint32_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_UINT32);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.uint64_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_UINT64);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_FLOAT_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.float_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_FLOAT);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_DOUBLE_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.double_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_DOUBLE);	\
}
#define	FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(node, v) {	\
	(((FeParserExprASTNodeData *)&((node)->data[0]))->constexpr_result.ptr_val = (v));	\
	FE_EXPR_AST_NODE_SET_CONSTANT_TYPE((node), FE_TYPE_POINTER);	\
}


#define	FE_EXPR_AST_NODE_COPY_CONSTANT(target, source)	(FE_EXPR_AST_NODE_SET_CONSTANT((target), FE_EXPR_AST_NODE_GET_CONSTANT((source))))

#define	FE_EXPR_AST_NODE_COPY_CONSTANT2(target, source1, source2)	\
	(FE_EXPR_AST_NODE_SET_CONSTANT(	\
		(target),	\
		FE_EXPR_AST_NODE_GET_CONSTANT((source1)) && FE_EXPR_AST_NODE_GET_CONSTANT((source2))	\
	))

#define	FE_EXPR_AST_NODE_COPY_CONSTANT3(target, source1, source2, source3)	\
	(FE_EXPR_AST_NODE_SET_CONSTANT(	\
		(target),	\
		FE_EXPR_AST_NODE_GET_CONSTANT((source1)) && FE_EXPR_AST_NODE_GET_CONSTANT((source2)) && FE_EXPR_AST_NODE_GET_CONSTANT((source3))	\
	))

#define	FE_EXPR_AST_NODE_COPY(target, source)		(memcpy(&((target)->data), &((source)->data), sizeof(FeParserExprASTNodeData)))




#define	FE_VAR_TYPE_UNKNOWN				0
#define	FE_VAR_TYPE_GLOBAL				1
#define	FE_VAR_TYPE_LOCAL				2
#define	FE_VAR_TYPE_STRUCT_MEMBER		3

typedef struct {
	/*
		标识变量的作用域。

		FE_VAR_TYPE_GLOBAL：全局变量
		FE_VAR_TYPE_LOCAL：局部变量
		FE_VAR_TYPE_STRUCT_MEMBER：结构体成员变量
	*/
	uint8_t			type;

	ParserASTNode	*align_node;
	int				align;
} FeParserVarASTNodeData;

#define	FE_VAR_AST_NODE_GET_TYPE(node)					(((FeParserVarASTNodeData *)&((node)->data[0]))->type)
#define	FE_VAR_AST_NODE_GET_ALIGN_NODE(node)			(((FeParserVarASTNodeData *)&((node)->data[0]))->align_node)
#define	FE_VAR_AST_NODE_GET_ALIGN(node)					(((FeParserVarASTNodeData *)&((node)->data[0]))->align)

#define	FE_VAR_AST_NODE_SET_TYPE(node, t)				(((FeParserVarASTNodeData *)&((node)->data[0]))->type = (t))
#define	FE_VAR_AST_NODE_SET_ALIGN_NODE(node, n)			(((FeParserVarASTNodeData *)&((node)->data[0]))->align_node = (n))
#define	FE_VAR_AST_NODE_SET_ALIGN(node, a)				(((FeParserVarASTNodeData *)&((node)->data[0]))->align = (a))




typedef struct {
	ParserSymbol	*symbol;
} FeParserVarItemASTNodeData;

#define	FE_VAR_ITEM_AST_NODE_GET_SYMBOL(node)			(((FeParserVarItemASTNodeData *)&((node)->data[0]))->symbol)

#define	FE_VAR_ITEM_AST_NODE_SET_SYMBOL(node, s)		(((FeParserVarItemASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ParserSymbol	*symbol;
} FeParserFuncASTNodeData;

#define	FE_FUNC_AST_NODE_GET_SYMBOL(node)		(((FeParserFuncASTNodeData *)&((node)->data[0]))->symbol)

#define	FE_FUNC_AST_NODE_SET_SYMBOL(node, s)	(((FeParserFuncASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ParserSymbol	*symbol;
} FeParserFuncParamASTNodeData;

#define	FE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node)		(((FeParserFuncParamASTNodeData *)&((node)->data[0]))->symbol)

#define	FE_FUNC_PARAM_AST_NODE_SET_SYMBOL(node, s)	(((FeParserFuncParamASTNodeData *)&((node)->data[0]))->symbol = (s))




typedef struct {
	ResizableString label_repeat;
	ResizableString label_end;
} FeParserLoopASTNodeData;

#define	FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node)		(&(((FeParserLoopASTNodeData *)&((node)->data[0]))->label_repeat))
#define	FE_LOOP_AST_NODE_GET_LABEL_END(node)		(&(((FeParserLoopASTNodeData *)&((node)->data[0]))->label_end))




typedef struct {
	ParserASTNode		*packed_node;
	bool				packed;
} FeParserStructASTNodeData;

#define	FE_STRUCT_AST_NODE_GET_PACKED_NODE(node)		(((FeParserStructASTNodeData *)&((node)->data[0]))->packed_node)
#define	FE_STRUCT_AST_NODE_GET_PACKED(node)				(((FeParserStructASTNodeData *)&((node)->data[0]))->packed)

#define	FE_STRUCT_AST_NODE_SET_PACKED_NODE(node, v)		(((FeParserStructASTNodeData *)&((node)->data[0]))->packed_node = (v))
#define	FE_STRUCT_AST_NODE_SET_PACKED(node, v)			(((FeParserStructASTNodeData *)&((node)->data[0]))->packed = (v))




extern ParserASTNode * fe_parser_new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
);

extern ParserContext * fe_parser_new_context(
	const char *file,
	const char *source,
	int len,
	int arch
);

extern void fe_parser_free_context(ParserContext * ctx);

extern void fe_parser_parse(ParserContext * ctx);

extern void fe_parser_print_ast(ParserContext * ctx, FILE *file);

extern uint32_t fe_parser_get_uint32_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern uint64_t fe_parser_get_uint64_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern uint64_t fe_parser_get_uint_val(
	ParserContext *ctx,
	ParserASTNode *node
);

extern float fe_parser_get_float_val(
	ParserContext *ctx,
	ParserASTNode *node
);
extern double fe_parser_get_double_val(
	ParserContext *ctx,
	ParserASTNode *node
);

extern char fe_parser_get_char_val(
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
} FeParserVarSymbolData;

#define	FE_VAR_SYMBOL_GET_TYPE(symbol)					(((FeParserVarSymbolData *)&((symbol)->data[0]))->type)
#define	FE_VAR_SYMBOL_GET_TYPE_NODE(symbol)				(((FeParserVarSymbolData *)&((symbol)->data[0]))->type_node)
#define	FE_VAR_SYMBOL_GET_FUNC_SYMBOL(symbol)			(((FeParserVarSymbolData *)&((symbol)->data[0]))->func_symbol)
#define	FE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol)		(((FeParserVarSymbolData *)&((symbol)->data[0]))->has_code_gen_name)
#define	FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol)			(&(((FeParserVarSymbolData *)&((symbol)->data[0]))->code_gen_name))
#define	FE_VAR_SYMBOL_GET_ALIGN(symbol)					(((FeParserVarSymbolData *)&((symbol)->data[0]))->align)

#define	FE_VAR_SYMBOL_SET_FUNC_SYMBOL(symbol, v)		(((FeParserVarSymbolData *)&((symbol)->data[0]))->func_symbol = (v))
#define	FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, v)	(((FeParserVarSymbolData *)&((symbol)->data[0]))->has_code_gen_name = (v))
#define	FE_VAR_SYMBOL_SET_ALIGN(symbol, v)				(((FeParserVarSymbolData *)&((symbol)->data[0]))->align = (v))




extern ParserSymbol * fe_parser_add_var_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node
);




typedef struct {
	uint8_t			type;
	ParserASTNode	*type_node;
} FeParserStructMemberVarSymbolData;

#define	FE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE(symbol)		(((FeParserStructMemberVarSymbolData *)&((symbol)->data[0]))->type)
#define	FE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE_NODE(symbol)	(((FeParserStructMemberVarSymbolData *)&((symbol)->data[0]))->type_node)

extern ParserSymbol * fe_parser_add_struct_member_var_symbol_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node
);




typedef struct {
	ParserASTNode	*struct_node;
	ParserASTNode	*body_node;
	bool			dummy;
} FeParserStructSymbolData;

#define	FE_STRUCT_SYMBOL_GET_STRUCT_NODE(symbol)		(((FeParserStructSymbolData *)&((symbol)->data[0]))->struct_node)
#define	FE_STRUCT_SYMBOL_SET_STRUCT_NODE(symbol, v)		(((FeParserStructSymbolData *)&((symbol)->data[0]))->struct_node = (v))
#define	FE_STRUCT_SYMBOL_GET_BODY_NODE(symbol)			(((FeParserStructSymbolData *)&((symbol)->data[0]))->body_node)
#define	FE_STRUCT_SYMBOL_SET_BODY_NODE(symbol, bn)		(((FeParserStructSymbolData *)&((symbol)->data[0]))->body_node = (bn))
#define	FE_STRUCT_SYMBOL_GET_DUMMY(symbol)				(((FeParserStructSymbolData *)&((symbol)->data[0]))->dummy)
#define	FE_STRUCT_SYMBOL_SET_DUMMY(symbol, d)			(((FeParserStructSymbolData *)&((symbol)->data[0]))->dummy = (d))

extern ParserSymbol * fe_parser_new_struct_symbol(
	ParserContext *ctx,
	LexerToken *token,
	ParserASTNode *struct_node,
	ParserASTNode *body_node,
	bool dummy
);

extern ParserSymbol * fe_parser_add_struct_symbol_to_node(
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
} FeParserFuncSymbolData;

#define	FE_FUNC_SYMBOL_GET_PARAMS_NODE(symbol)				(((FeParserFuncSymbolData *)&((symbol)->data[0]))->params_node)
#define	FE_FUNC_SYMBOL_GET_RETURN_TYPE_NODE(symbol)			(((FeParserFuncSymbolData *)&((symbol)->data[0]))->return_type_node)
#define	FE_FUNC_SYMBOL_GET_FUNC_TYPE_NODE(symbol)			(((FeParserFuncSymbolData *)&((symbol)->data[0]))->func_type_node)
#define	FE_FUNC_SYMBOL_GET_FUNC_POINTER_TYPE_NODE(symbol)	(((FeParserFuncSymbolData *)&((symbol)->data[0]))->func_pointer_type_node)
#define	FE_FUNC_SYMBOL_GET_DUMMY(symbol)					(((FeParserFuncSymbolData *)&((symbol)->data[0]))->dummy)

#define	FE_FUNC_SYMBOL_SET_DUMMY(symbol, d)					(((FeParserFuncSymbolData *)&((symbol)->data[0]))->dummy = (d))

#define	FE_FUNC_SYMBOL_NEXT_NO(symbol)						(((FeParserFuncSymbolData *)&((symbol)->data[0]))->counter++)

extern ParserSymbol * fe_parser_add_func_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	ParserASTNode *params_node,
	ParserASTNode *return_type_node,
	bool dummy
);




extern bool fe_parser_has_unsigned_mark(
	ParserContext *ctx,
	ParserASTNode *node
);

extern bool fe_parser_has_float_mark(
	ParserContext *ctx,
	ParserASTNode *node
);

#endif
