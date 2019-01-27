#include "irgen.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"

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
	fprintf(stderr, "IRGen Error: %s\n", msg);
	exit(1);
}

static void _ir_var(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == FE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_identifier = node_var_item->childs[0];
		ParserASTNode *node_type = node_var_item->childs[1];

		if (node_var_item->nchilds == 2) {

		} else {

		}	
	}
}

static void _ir_struct(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);
}

static void _ir_func(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_FUNC);
}

static void _ir_module_item(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case FE_NODE_VAR: {
			_ir_var(ctx, node);
			break;
		}
		case FE_NODE_STRUCT: {
			_ir_struct(ctx, node);
			break;
		}
		case FE_NODE_FUNC: {
			_ir_func(ctx, node);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _ir_module(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_MODULE);

	for (int i = 0; i < node->nchilds; i++) {
		_ir_module_item(ctx, node->childs[i]);
	}
}




void fe_irgen_generate(IRGeneratorContext *ctx) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == FE_NODE_MODULE);
	assert(ctx->out);

	_ir_module(ctx, ctx->psrctx->ast);
}

IRGeneratorContext * fe_irgen_new_context(ParserContext *psrctx, FILE *out) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	
	IRGeneratorContext *ctx = (IRGeneratorContext *)malloc(sizeof(IRGeneratorContext));
	if (ctx == NULL) {
		_error("cannot allocate memory for a new context!");
	}

	ctx->psrctx = psrctx;
	ctx->out = out;

	ctx->lblctr = 0;

	return ctx;
}

void fe_irgen_free_context(IRGeneratorContext *ctx) {
	assert(ctx);

	free(ctx);
}
