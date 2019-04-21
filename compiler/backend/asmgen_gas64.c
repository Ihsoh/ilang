#include "asmgen_gas64.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "semantics.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define	_OUT_CHAR(ctx, chr)	{ fputc((chr), (ctx)->out); }
#define	_OUT_STR(ctx, str, len) { for (int __i = 0; __i < (len); __i++) { _OUT_CHAR((ctx), (str)[__i]) } }
#define	_OUT_CSTR(ctx, str) { _OUT_STR((ctx), (str), strlen((str))) }
#define	_OUT_FMT(ctx, fmt, ...) { fprintf((ctx)->out, fmt, __VA_ARGS__); }

static void _error(const char * msg) {
	assert(msg);
	fprintf(stderr, "ASMGen(GAS) Error: %s\n", msg);
	exit(1);
}







static void _asm_module(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_MODULE);

	_OUT_STR(ctx, ctx->global.buffer, ctx->global.len);

	for (int i = 0; i < node->nchilds; i++) {
		// _asm_module_item(ctx, node->childs[i]);
	}
}




void be_asmgen_gas64_generate(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == BE_NODE_MODULE);
	assert(ctx->out);

	_asm_module(ctx, ctx->psrctx->ast);
}

ASMGeneratorGas64Context * be_asmgen_gas64_new_context(
	ParserContext *psrctx,
	FILE *out
) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	
	ASMGeneratorGas64Context *ctx = (ASMGeneratorGas64Context *)malloc(sizeof(ASMGeneratorGas64Context));
	if (ctx == NULL) {
		_error("cannot allocate memory for a new context!");
	}

	ctx->psrctx = psrctx;
	ctx->out = out;

	rstr_init(&(ctx->global));

	return ctx;
}

void be_asmgen_gas64_free_context(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);

	rstr_free(&(ctx->global));

	free(ctx);
}
