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

	// 局部变量地址计数器。
	size_t				local_var_address_counter;

	// 所有局部变量在栈空间占用的大小。
	size_t				local_var_size;

	// 局部变量索引计数器。
	size_t				local_var_index_counter;

	size_t				counter;

	ParserSymbol		*func_symbol;

	int					platform;
} ASMGeneratorGas64Context;




extern void be_asmgen_gas64_generate(ASMGeneratorGas64Context *ctx);
extern ASMGeneratorGas64Context * be_asmgen_gas64_new_context(
	ParserContext *psrctx,
	FILE *out,
	int platform
);
extern void be_asmgen_gas64_free_context(ASMGeneratorGas64Context *ctx);

#endif
