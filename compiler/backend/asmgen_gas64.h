#ifndef	__COMPILER_BACKEND_ASMGEN_GAS64_H__
#define	__COMPILER_BACKEND_ASMGEN_GAS64_H__

#include <stdio.h>

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
} ASMGeneratorGas64Context;




extern void be_asmgen_gas64_generate(ASMGeneratorGas64Context *ctx);
extern ASMGeneratorGas64Context * be_asmgen_gas64_new_context(ParserContext *psrctx, FILE *out);
extern void be_asmgen_gas64_free_context(ASMGeneratorGas64Context *ctx);

#endif
