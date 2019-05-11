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




static void _ir_type(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
);

static void _ir_func_type_params(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_func_type_params
) {
	assert(ctx);
	assert(rstr);
	assert(node_func_type_params);
	assert(node_func_type_params->type == FE_NODE_FUNC_PARAMS);

	if (node_func_type_params->nchilds > 0) {
		ParserASTNode *node_param = node_func_type_params->childs[0];
		assert(node_param->type == FE_NODE_FUNC_PARAMS_ITEM);

		#define _NODE_PARAM	\
			switch (node_param->type) {	\
				case FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {	\
					rstr_append_with_cstr(rstr, "...");	\
					break;	\
				}	\
				case FE_NODE_FUNC_PARAMS_ITEM: {	\
					assert(node_param->nchilds == 2);	\
					ParserASTNode *node_id = node_param->childs[0];	\
					ParserASTNode *node_type = node_param->childs[1];	\
						\
					rstr_append_with_raw(rstr, node_id->token->content, node_id->token->len);	\
						\
					rstr_append_with_char(rstr, ':');	\
						\
					ResizableString rstr_type;	\
					rstr_init(&rstr_type);	\
					_ir_type(ctx, &rstr_type, node_type);	\
					rstr_append_with_rstr(rstr, &rstr_type);	\
					rstr_free(&rstr_type);	\
					break;	\
				}	\
				default: {	\
					assert(0);	\
					break;	\
				}	\
			}

		_NODE_PARAM

		for (int i = 1; i < node_func_type_params->nchilds; i++) {
			node_param = node_func_type_params->childs[i];
			assert(node_param->type == FE_NODE_FUNC_PARAMS_ITEM
					|| node_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM);

			rstr_append_with_cstr(rstr, ", ");

			_NODE_PARAM
		}

		#undef	_NODE_PARAM
	}
}










static void _ir_type(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR: {
			rstr_append_with_cstr(rstr, "char");
			break;
		}
		case FE_NODE_TYPE_INT8: {
			rstr_append_with_cstr(rstr, "int8");
			break;
		}
		case FE_NODE_TYPE_UINT8: {
			rstr_append_with_cstr(rstr, "uint8");
			break;
		}
		case FE_NODE_TYPE_INT16: {
			rstr_append_with_cstr(rstr, "int16");
			break;
		}
		case FE_NODE_TYPE_UINT16: {
			rstr_append_with_cstr(rstr, "uint16");
			break;
		}
		case FE_NODE_TYPE_INT32: {
			rstr_append_with_cstr(rstr, "int32");
			break;
		}
		case FE_NODE_TYPE_UINT32: {
			rstr_append_with_cstr(rstr, "uint32");
			break;
		}
		case FE_NODE_TYPE_INT64: {
			rstr_append_with_cstr(rstr, "int64");
			break;
		}
		case FE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "uint64");
			break;
		}
		case FE_NODE_TYPE_FLOAT: {
			rstr_append_with_cstr(rstr, "float");
			break;
		}
		case FE_NODE_TYPE_DOUBLE: {
			rstr_append_with_cstr(rstr, "double");
			break;
		}
		case FE_NODE_TYPE_VA_LIST: {
			rstr_append_with_cstr(rstr, "__va_list");
			break;
		}
		case FE_NODE_TYPE_STRUCT: {
			assert(node_type->nchilds == 1);

			ParserASTNode *node_id = node_type->childs[0];
			assert(node_id->type == FE_NODE_IDENTIFIER);

			rstr_append_with_cstr(rstr, "struct ");
			rstr_append_with_raw(rstr, node_id->token->content, node_id->token->len);
			break;
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_dims = node_type->childs[0];
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);
			assert(node_array_dims->nchilds > 0);

			ParserASTNode *node_t = node_type->childs[1];

			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == FE_NODE_LITERAL_UINT);

				rstr_append_with_cstr(rstr, "[");
				rstr_append_with_raw(rstr, node_array_dim->token->content, node_array_dim->token->len);
				rstr_append_with_cstr(rstr, "]");
			}

			_ir_type(ctx, rstr, node_t);

			break;
		}
		case FE_NODE_TYPE_FUNC: {
			assert(node_type->nchilds == 1 || node_type->nchilds == 2);
			ParserASTNode *node_params = NULL;
			ParserASTNode *node_t = NULL;

			node_params = node_type->childs[0];
			assert(node_params->type == FE_NODE_FUNC_PARAMS);

			if (node_type->nchilds == 2) {
				node_t = node_type->childs[1];
			}

			rstr_append_with_cstr(rstr, "function");

			rstr_append_with_cstr(rstr, "(");
			_ir_func_type_params(ctx, rstr, node_params);
			rstr_append_with_cstr(rstr, ")");

			if (node_t != NULL) {
				rstr_append_with_char(rstr, ':');
				_ir_type(ctx, rstr, node_t);
			}
			
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			assert(node_type->nchilds == 1);
			ParserASTNode *node_t = node_type->childs[0];

			rstr_append_with_char(rstr, '*');

			_ir_type(ctx, rstr, node_t);
			break;
		}
		case FE_NODE_TYPE_VOID: {
			// 什么都不做。
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}







static void _ir_var(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);
	assert(node->nchilds > 0);

	int align = FE_VAR_AST_NODE_GET_ALIGN(node);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == FE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_identifier = node_var_item->childs[0];
		assert(node_identifier->type == FE_NODE_IDENTIFIER);
		ParserASTNode *node_type = node_var_item->childs[1];
		ParserASTNode *node_expr = NULL;

		if (node_var_item->nchilds == 3) {
			node_expr = node_var_item->childs[2];
		}

		

	}
}

static void _ir_struct(IRGeneratorContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == FE_NODE_IDENTIFIER);
	ParserASTNode *node_struct_body = node->childs[1];
	if (node_struct_body->type == FE_NODE_DUMMY) {
		return;
	}
	assert(node_struct_body->type == FE_NODE_STRUCT_BODY);


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

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->global));

	_ir_module(ctx, ctx->psrctx->ast);

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->head));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->body));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->foot));
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

	ctx->global = rstr_new();
	ctx->head = rstr_new();
	ctx->body = rstr_new();
	ctx->foot = rstr_new();
	ctx->local_var_defs = NULL;

	rstr_init(ctx->global);
	rstr_init(ctx->head);
	rstr_init(ctx->body);
	rstr_init(ctx->foot);

	ctx->counter = 0;
	ctx->func_symbol = NULL;

	return ctx;
}

void fe_irgen_free_context(IRGeneratorContext *ctx) {
	assert(ctx);
	assert(ctx->global);
	assert(ctx->head);
	assert(ctx->body);
	assert(ctx->foot);

	rstr_free(ctx->global);
	rstr_free(ctx->head);
	rstr_free(ctx->body);
	rstr_free(ctx->foot);

	free(ctx->global);
	free(ctx->head);
	free(ctx->body);
	free(ctx->foot);

	ctx->global = NULL;
	ctx->head = NULL;
	ctx->body = NULL;
	ctx->foot = NULL;

	free(ctx);
}
