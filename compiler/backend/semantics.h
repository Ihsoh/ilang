#ifndef	__COMPILER_BACKEND_SEMANTICS_H__
#define	__COMPILER_BACKEND_SEMANTICS_H__

#include "../../parser.h"

#include <stdio.h>

#define	BE_SEM_NODE_CATEGORY_UNKNOWN	0
#define	BE_SEM_NODE_CATEGORY_TYPE		1
#define	BE_SEM_NODE_CATAGORY_EXPR		2

extern void be_sem_process(
	ParserContext *ctx
);

extern void be_sem_parser_print_ast(
	ParserContext * ctx,
	FILE *file
);

extern size_t be_sem_calc_type_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
);

#endif
