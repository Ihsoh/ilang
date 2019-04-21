#include "semantics.h"

#include "../../parser.h"
#include "../../util.h"
#include "../../common.h"

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>




static void _module_item(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case BE_NODE_VAR: {
			// _var(ctx, node);
			break;
		}
		case BE_NODE_STRUCT: {
			// _struct(ctx, node);
			break;
		}
		case BE_NODE_FUNC: {
			// _func(ctx, node);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _module(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *child = node->childs[i];

		_module_item(ctx, child);
	}
}

void be_sem_process(
	ParserContext *ctx
) {
	assert(ctx);
	assert(ctx->ast);

	ParserASTNode *node_module = ctx->ast;
	_module(ctx, node_module);
}
