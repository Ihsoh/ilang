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

static void _ir_identifier_name(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_identifier
) {
	assert(ctx);
	assert(rstr);
	assert(node_identifier);

	rstr_append_with_char(rstr, '_');
	rstr_append_with_raw(rstr, node_identifier->token->content, node_identifier->token->len);
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

			ResizableString rstr_id;
			rstr_init(&rstr_id);
			_ir_identifier_name(ctx, &rstr_id, node_id);
			rstr_append_with_rstr(rstr, &rstr_id);
			rstr_free(&rstr_id);
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




static void _ir_constexpr(
	IRGeneratorContext *ctx,
	ResizableString *rstr_type,
	ResizableString *rstr_val,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr_val);
	assert(node_constexpr);
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	if (rstr_type != NULL) {
		_ir_type(ctx, rstr_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_constexpr));
	}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node_constexpr)) {
		case FE_TYPE_CHAR: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<char>(%d)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<int8>(%d)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<int16>(%d)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<int32>(%d)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<int64>(%lld)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<uint8>(%u)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<uint16>(%u)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<uint32>(%u)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "cast<uint64>(%llu)",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_FLOAT: {
			char str[128];
			float val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "cast<float>(%f)",
				val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_DOUBLE: {
			char str[128];
			double val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "cast<double>(%lf)",
				val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_POINTER: {
			if (FE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_constexpr)) {
				FeParserConstexprString *conststr = FE_EXPR_AST_NODE_GET_CONSTSTR(node_constexpr);
				uint64_t ptr_val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);

				ResizableString rstr_string;
				rstr_init(&rstr_string);
				rstr_append_with_char(&rstr_string, '"');
				rstr_append_with_raw(&rstr_string, conststr->str, conststr->len);
				rstr_append_with_char(&rstr_string, '"');

				ResizableString target_type;
				rstr_init(&target_type);
				_ir_type(ctx, &target_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_constexpr));

				char str[1024];
				if (ptr_val == 0) {
					snprintf(
						str, sizeof(str), "cast<%s>(%s)",
						RSTR_CSTR(&target_type),
						RSTR_CSTR(&rstr_string)
					);
				} else {
					snprintf(
						str, sizeof(str), "cast<%s>(%s + %llu)",
						RSTR_CSTR(&target_type),
						RSTR_CSTR(&rstr_string),
						ptr_val
					);
				}

				rstr_append_with_cstr(rstr_val, str);

				rstr_free(&rstr_string);
				rstr_free(&target_type);
			} else {
				ResizableString target_type;
				rstr_init(&target_type);
				_ir_type(ctx, &target_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_constexpr));

				char str[1024];
				snprintf(
					str, sizeof(str), "cast<%s>(%llu)",
					RSTR_CSTR(&target_type),
					FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr)
				);
				rstr_append_with_cstr(rstr_val, str);

				rstr_free(&target_type);
			}
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _ir_constexpr_initializer(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr);
	assert(node_constexpr);
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	_ir_constexpr(ctx, NULL, rstr, node_constexpr);
}

static void _ir_var(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
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

		switch (FE_VAR_AST_NODE_GET_TYPE(node)) {
			case FE_VAR_TYPE_GLOBAL: {
				ResizableString rstr_id;
				rstr_init(&rstr_id);
				_ir_identifier_name(ctx, &rstr_id, node_identifier);

				ResizableString rstr;
				rstr_init(&rstr);

				if (align > 0) {
					rstr_appendf(&rstr, "align(%d) ", align);
				}

				rstr_append_with_cstr(&rstr, "var ");
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_id));
				rstr_append_with_char(&rstr, ':');

				ResizableString rstr_type;
				rstr_init(&rstr_type);
				_ir_type(ctx, &rstr_type, node_type);
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_type));
				rstr_free(&rstr_type);

				if (node_expr != NULL) {
					rstr_append_with_cstr(&rstr, " = ");
					_ir_constexpr_initializer(ctx, &rstr, node_expr);
				}

				rstr_append_with_cstr(&rstr, ";\n");

				rstr_append_with_cstr(ctx->body, RSTR_CSTR(&rstr));

				rstr_free(&rstr);

				// 更新全局变量符号在代码生成时的名字。
				ParserSymbol *symbol = FE_VAR_ITEM_AST_NODE_GET_SYMBOL(node_var_item);
				FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);
				rstr_init(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol));
				rstr_append_with_raw(
					FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol),
					RSTR_CSTR(&rstr_id),
					RSTR_LEN(&rstr_id)
				);
				
				rstr_free(&rstr_id);
				break;
			}
			case FE_VAR_TYPE_LOCAL: {

				break;
			}
			case FE_VAR_TYPE_STRUCT_MEMBER: {
				assert(node_expr == NULL);
				
				ResizableString rstr_id;
				rstr_init(&rstr_id);
				_ir_identifier_name(ctx, &rstr_id, node_identifier);

				ResizableString rstr;
				rstr_init(&rstr);

				rstr_append_with_cstr(&rstr, "var ");
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_id));
				rstr_append_with_char(&rstr, ':');

				ResizableString rstr_type;
				rstr_init(&rstr_type);
				_ir_type(ctx, &rstr_type, node_type);
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_type));
				rstr_free(&rstr_type);

				rstr_append_with_cstr(&rstr, ";\n");

				rstr_append_with_cstr(ctx->body, RSTR_CSTR(&rstr));

				rstr_free(&rstr);
				rstr_free(&rstr_id);
				break;
			}
			default: {
				assert(0);
			}
		}
	}
}

static void _ir_struct(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == FE_NODE_IDENTIFIER);
	ParserASTNode *node_struct_body = node->childs[1];

	if (node_struct_body->type == FE_NODE_DUMMY) {
		ResizableString rstr_id;
		rstr_init(&rstr_id);
		_ir_identifier_name(ctx, &rstr_id, node_identifier);
		rstr_append_with_cstr(ctx->body, "\nstruct ");
		rstr_append_with_rstr(ctx->body, &rstr_id);
		rstr_append_with_cstr(ctx->body, " dummy\n\n");
		rstr_free(&rstr_id);
	} else if (node_struct_body->type == FE_NODE_STRUCT_BODY) {
		ResizableString rstr_id;
		rstr_init(&rstr_id);
		_ir_identifier_name(ctx, &rstr_id, node_identifier);
		rstr_append_with_cstr(ctx->body, "\nstruct ");
		rstr_append_with_rstr(ctx->body, &rstr_id);
		rstr_append_with_cstr(ctx->body, " {\n");
		rstr_free(&rstr_id);

		for (int i = 0; i < node_struct_body->nchilds; i++) {
			ParserASTNode *node_var = node_struct_body->childs[i];
			assert(node_var->type == FE_NODE_VAR);

			_ir_var(ctx, node_var);
		}

		rstr_append_with_cstr(ctx->body, "}\n\n");
	} else {
		assert(0);
	}
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
