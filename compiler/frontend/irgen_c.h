#ifndef	__FRONTEND_IRGEN_C_H__
#define	__FRONTEND_IRGEN_C_H__

#include <stdio.h>

#include "../../parser.h"
#include "../../string.h"

typedef struct {
	ParserContext 		*psrctx;
	FILE 				*out;
	ResizableString 	global;
} IRGeneratorCContext;




extern void fe_irgen_c_generate(IRGeneratorCContext *ctx);
extern IRGeneratorCContext * fe_irgen_c_new_context(ParserContext *psrctx, FILE *out);
extern void fe_irgen_c_free_context(IRGeneratorCContext *ctx);

#endif
