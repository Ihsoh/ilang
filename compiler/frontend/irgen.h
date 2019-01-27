#ifndef	__FRONTEND_IRGEN__
#define	__FRONTEND_IRGEN__

#include <stdio.h>

#include "../../parser.h"

typedef struct {
	ParserContext 		*psrctx;
	FILE 				*out;

	int					lblctr;
} IRGeneratorContext;




extern void fe_irgen_generate(IRGeneratorContext *ctx);
extern IRGeneratorContext * fe_irgen_new_context(ParserContext *psrctx, FILE *out);
extern void fe_irgen_free_context(IRGeneratorContext *ctx);

#endif
