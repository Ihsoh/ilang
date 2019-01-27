#ifndef	__FRONTEND_IRGEN_LLVMIR_H__
#define	__FRONTEND_IRGEN_LLVMIR_H__

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
} IRGeneratorLlvmirContext;




extern void fe_irgen_llvmir_generate(IRGeneratorLlvmirContext *ctx);
extern IRGeneratorLlvmirContext * fe_irgen_llvmir_new_context(ParserContext *psrctx, FILE *out);
extern void fe_irgen_llvmir_free_context(IRGeneratorLlvmirContext *ctx);

#endif
