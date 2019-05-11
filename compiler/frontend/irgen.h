#ifndef	__COMPILER_FRONTEND_IRGEN__
#define	__COMPILER_FRONTEND_IRGEN__

#include <stdio.h>
#include <stddef.h>

#include "../../parser.h"
#include "../../string.h"

typedef struct {
	ParserContext 		*psrctx;
	FILE 				*out;

	ResizableString 	*global;
	ResizableString		*head;
	ResizableString		*body;
	ResizableString		*foot;

	ResizableString		*local_var_defs;

	size_t				counter;

	ParserSymbol		*func_symbol;
} IRGeneratorContext;




extern void fe_irgen_generate(IRGeneratorContext *ctx);
extern IRGeneratorContext * fe_irgen_new_context(ParserContext *psrctx, FILE *out);
extern void fe_irgen_free_context(IRGeneratorContext *ctx);

#endif
