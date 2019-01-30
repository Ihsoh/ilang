#ifndef	__COMPILER_FRONTEND_SEMANTICS_H__
#define	__COMPILER_FRONTEND_SEMANTICS_H__

#include "../../parser.h"

#include <stdio.h>

#define	FE_SEM_NODE_CATEGORY_UNKNOWN	0
#define	FE_SEM_NODE_CATEGORY_TYPE		1
#define	FE_SEM_NODE_CATAGORY_EXPR		2

extern void fe_sem_process(
	ParserContext *ctx
);

extern void fe_sem_parser_print_ast(
	ParserContext * ctx,
	FILE *file
);

extern int fe_sem_get_struct_member_index(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
);

extern ParserASTNode * fe_sem_get_struct_member_type_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
);

extern int32_t fe_sem_get_primitive_type_weight(
	uint8_t type
);

extern size_t fe_sem_get_primitive_type_size(
	uint8_t type
);

extern bool fe_sem_is_signed_type(
	uint8_t type
);

extern bool fe_sem_is_unsigned_type(
	uint8_t type
);

extern bool fe_sem_is_integer_type(
	uint8_t type
);

extern bool fe_sem_is_float_type(
	uint8_t type
);

extern bool fe_sem_is_primitive_type(
	uint8_t type
);

extern bool fe_sem_is_pointer_type(
	uint8_t type
);

extern bool fe_sem_is_number_type(
	uint8_t type
);

extern bool fe_sem_is_compatible_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
);

extern uint8_t fe_sem_get_type_by_type_node(
	ParserContext *ctx,
	ParserASTNode *node
);

extern ParserASTNode * fe_sem_search_node_along_parent(
	ParserASTNode *node,
	...
);

extern ParserSymbol * fe_sem_get_func_symbol_by_node(
	ParserContext *ctx,
	ParserASTNode *node
);

#endif
