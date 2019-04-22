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



#endif
