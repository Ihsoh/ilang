#include "irgen_c.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

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
	fprintf(stderr, "IRGen(C) Error: %s\n", msg);
	exit(1);
}

static void _ir_func_params(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_func_params
);

static void _ir_type(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR: {
			rstr_prepend_with_cstr(rstr, "char ");
			break;
		}
		case FE_NODE_TYPE_INT8: {
			rstr_prepend_with_cstr(rstr, "int8_t ");
			break;
		}
		case FE_NODE_TYPE_UINT8: {
			rstr_prepend_with_cstr(rstr, "uint8_t ");
			break;
		}
		case FE_NODE_TYPE_INT16: {
			rstr_prepend_with_cstr(rstr, "int16_t ");
			break;
		}
		case FE_NODE_TYPE_UINT16: {
			rstr_prepend_with_cstr(rstr, "uint16_t ");
			break;
		}
		case FE_NODE_TYPE_INT32: {
			rstr_prepend_with_cstr(rstr, "int32_t ");
			break;
		}
		case FE_NODE_TYPE_UINT32: {
			rstr_prepend_with_cstr(rstr, "uint32_t ");
			break;
		}
		case FE_NODE_TYPE_INT64: {
			rstr_prepend_with_cstr(rstr, "int64_t ");
			break;
		}
		case FE_NODE_TYPE_UINT64: {
			rstr_prepend_with_cstr(rstr, "uint64_t ");
			break;
		}
		case FE_NODE_TYPE_FLOAT: {
			rstr_prepend_with_cstr(rstr, "float ");
			break;
		}
		case FE_NODE_TYPE_DOUBLE: {
			rstr_prepend_with_cstr(rstr, "double ");
			break;
		}
		case FE_NODE_TYPE_STRUCT: {
			assert(node_type->nchilds == 1);

			ParserASTNode *node_id = node_type->childs[0];
			assert(node_id->type == FE_NODE_IDENTIFIER);

			rstr_prepend_with_cstr(rstr, " ");
			rstr_prepend_with_raw(rstr, node_id->token->content, node_id->token->len);
			rstr_prepend_with_cstr(rstr, "struct ");
			break;
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);
			ParserASTNode *node_array_dims = node_type->childs[0];
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);
			ParserASTNode *node_t = node_type->childs[1];

			rstr_prepend_with_cstr(rstr, "(");
			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == FE_NODE_LITERAL_UINT);
				rstr_append_with_cstr(rstr, "[");
				rstr_append_with_raw(rstr, node_array_dim->token->content, node_array_dim->token->len);
				rstr_append_with_cstr(rstr, "]");
			}
			rstr_append_with_cstr(rstr, ")");

			_ir_type(ctx, rstr, node_t);
			break;
		}
		case FE_NODE_TYPE_FUNC: {
			assert(node_type->nchilds == 1 || node_type->nchilds == 2);
			ParserASTNode *node_params = NULL;
			ParserASTNode *node_t = NULL;

			node_params = node_type->childs[0];
			if (node_type->nchilds == 2) {
				node_t = node_type->childs[1];
			}

			rstr_prepend_with_cstr(rstr, "(");
			rstr_append_with_cstr(rstr, ")(");
			_ir_func_params(ctx, rstr, node_params);
			rstr_append_with_cstr(rstr, ")");

			if (node_t != NULL) {
				_ir_type(ctx, rstr, node_t);
			} else {
				rstr_prepend_with_cstr(rstr, "void(");
				rstr_append_with_cstr(rstr, ")");
			}
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			assert(node_type->nchilds == 1);
			ParserASTNode *node_t = node_type->childs[0];
			
			rstr_prepend_with_cstr(rstr, "(*");
			rstr_append_with_cstr(rstr, ")");

			_ir_type(ctx, rstr, node_t);
			break;
		}
		case FE_NODE_TYPE_VOID: {
			rstr_prepend_with_cstr(rstr, "void ");
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _ir_func_params(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_func_params
) {
	assert(ctx);
	assert(rstr);
	assert(node_func_params);
	assert(node_func_params->type == FE_NODE_FUNC_PARAMS);

	if (node_func_params->nchilds > 0) {
		ParserASTNode *node_param = node_func_params->childs[0];
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
					assert(node_id->type == FE_NODE_IDENTIFIER);	\
					ParserASTNode *node_t = node_param->childs[1];	\
						\
					ResizableString rstr_func_params_item;	\
					rstr_init(&rstr_func_params_item);	\
					rstr_append_with_raw(&rstr_func_params_item, node_id->token->content, node_id->token->len);	\
						\
					_ir_type(ctx, &rstr_func_params_item, node_t);	\
						\
					rstr_append_with_rstr(rstr, &rstr_func_params_item);	\
					rstr_free(&rstr_func_params_item);	\
					break;	\
				}	\
				default: {	\
					assert(0);	\
					break;	\
				}	\
			}

		_NODE_PARAM

		for (int i = 1; i < node_func_params->nchilds; i++) {
			node_param = node_func_params->childs[i];
			assert(node_param->type == FE_NODE_FUNC_PARAMS_ITEM
					|| node_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM);

			rstr_append_with_cstr(rstr, ", ");

			_NODE_PARAM
		}

		#undef	_NODE_PARAM
	}
}

#define	_IR_EXPR_AUTO_CAST_BEGIN	\
	ResizableString __rstr_type;	\
	rstr_init(&__rstr_type);	\
	_ir_type(ctx, &__rstr_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr));	\
	rstr_appendf(rstr, "((%s)", RSTR_CSTR(&__rstr_type));

#define	_IR_EXPR_AUTO_CAST_END	\
	rstr_free(&__rstr_type);	\
	rstr_append_with_char(rstr, ')');

static void _ir_expr(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
);

static bool _ir_expr_constexpr(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);
	
	if (!FE_EXPR_AST_NODE_GET_CONSTANT(node_expr)) {
		return false;
	}

	_IR_EXPR_AUTO_CAST_BEGIN

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node_expr)) {
		case FE_TYPE_CHAR: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%lld",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%llu",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_FLOAT: {
			char str[128];
			snprintf(
				str, sizeof(str), "%f",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_DOUBLE: {
			char str[128];
			snprintf(
				str, sizeof(str), "%lf",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_expr)
			);
			rstr_append_with_cstr(rstr, str);
			break;
		}
		case FE_TYPE_POINTER: {
			rstr_append_with_char(rstr, '(');

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, node_type);
			rstr_appendf(rstr, "(%s) ", RSTR_CSTR(&rstr_type));
			rstr_free(&rstr_type);

			if (FE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_expr)) {
				FeParserConstexprString *conststr = FE_EXPR_AST_NODE_GET_CONSTSTR(node_expr);
				rstr_append_with_char(rstr, '\"');
				rstr_append_with_raw(rstr, conststr->str, conststr->len);
				rstr_append_with_char(rstr, '\"');

				uint64_t ptr_val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_expr);

				if (ptr_val > 0) {
					rstr_append_with_cstr(rstr, " + ");

					char str[128];
					snprintf(
						str, sizeof(str), "%llu",
						ptr_val
					);
					rstr_append_with_cstr(rstr, str);
				}
			} else {
				char str[128];
				snprintf(
					str, sizeof(str), "%llu",
					FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_expr)
				);
				rstr_append_with_cstr(rstr, str);
			}

			rstr_append_with_char(rstr, ')');
			break;
		}
		default: {
			assert(0);
			return false;
		}
	}

	_IR_EXPR_AUTO_CAST_END

	return true;
}

static void _ir_expr_identifier(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);
	assert(node_expr->type == FE_NODE_IDENTIFIER);
	assert(node_expr->nchilds == 0);

	rstr_append_with_raw(rstr, node_expr->token->content, node_expr->token->len);
}

static bool _ir_expr_atom(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	if (node_expr->type != FE_NODE_EXPR_ATOM) {
		return false;
	}

	ParserASTNode *node = node_expr->childs[0];
	
	switch (node->type) {
		case FE_NODE_LITERAL_UINT: {
			_IR_EXPR_AUTO_CAST_BEGIN

			uint64_t val = fe_parser_get_uint_val(ctx->psrctx, node);
			char str[128];
			snprintf(str, sizeof(str), "%llu", val);
			rstr_append_with_cstr(rstr, str);

			_IR_EXPR_AUTO_CAST_END

			return true;
		}
		case FE_NODE_LITERAL_REAL: {
			_IR_EXPR_AUTO_CAST_BEGIN

			double val = fe_parser_get_double_val(ctx->psrctx, node);
			char str[128];
			snprintf(str, sizeof(str), "%lf", val);
			rstr_append_with_cstr(rstr, str);

			_IR_EXPR_AUTO_CAST_END

			return true;
		}
		case FE_NODE_LITERAL_CHAR:
		case FE_NODE_LITERAL_STRING: {
			rstr_append_with_raw(rstr, node->token->content, node->token->len);
			return true;
		}
		case FE_NODE_IDENTIFIER: {
			_ir_expr_identifier(ctx, rstr, node);
			return true;
		}
		default: {
			assert(0);
		}
	}
}

static void _ir_expr_func_call_params(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);
	assert(node_expr->type == FE_NODE_EXPR_FUNC_CALL_PARAMS);

	if (node_expr->nchilds > 0) {
		_ir_expr(ctx, rstr, node_expr->childs[0]);
		for (int i = 1; i < node_expr->nchilds; i++) {
			rstr_append_with_cstr(rstr, ", ");
			_ir_expr(ctx, rstr, node_expr->childs[i]);
		}
	}
}

static bool _ir_expr_member_arridx_fncall(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	switch (node_expr->type) {
		case FE_NODE_EXPR_MEMBER: {
			assert(node_expr->nchilds == 2);

			rstr_append_with_cstr(rstr, "(");
			_ir_expr(ctx, rstr, node_expr->childs[0]);
			rstr_append_with_cstr(rstr, ".");
			_ir_expr_identifier(ctx, rstr, node_expr->childs[1]);
			rstr_append_with_cstr(rstr, ")");

			return true;
		}
		case FE_NODE_EXPR_PMEMBER: {
			assert(node_expr->nchilds == 2);

			rstr_append_with_cstr(rstr, "(");
			_ir_expr(ctx, rstr, node_expr->childs[0]);
			rstr_append_with_cstr(rstr, "->");
			_ir_expr_identifier(ctx, rstr, node_expr->childs[1]);
			rstr_append_with_cstr(rstr, ")");

			return true;
		}
		case FE_NODE_EXPR_ARRAY_INDEX: {
			assert(node_expr->nchilds == 2);

			rstr_append_with_cstr(rstr, "(");
			_ir_expr(ctx, rstr, node_expr->childs[0]);
			rstr_append_with_cstr(rstr, "[");
			_ir_expr(ctx, rstr, node_expr->childs[1]);
			rstr_append_with_cstr(rstr, "]");
			rstr_append_with_cstr(rstr, ")");

			return true;
		}
		case FE_NODE_EXPR_FUNC_CALL: {
			assert(node_expr->nchilds == 2);

			rstr_append_with_cstr(rstr, "(");
			_ir_expr(ctx, rstr, node_expr->childs[0]);
			rstr_append_with_cstr(rstr, "(");
			_ir_expr_func_call_params(ctx, rstr, node_expr->childs[1]);
			rstr_append_with_cstr(rstr, ")");
			rstr_append_with_cstr(rstr, ")");
			
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_unary(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_UNARY(str_opt)	\
	{	\
		assert(node_expr->nchilds == 1);	\
		rstr_append_with_cstr(rstr, "(");	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	#define	_IR_EXPR_UNARY_POSTFIX(str_opt)	\
	{	\
		assert(node_expr->nchilds == 1);	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_NOT: {
			_IR_EXPR_UNARY("!")
			return true;
		}
		case FE_NODE_EXPR_POSITIVE: {
			_IR_EXPR_UNARY("+")
			return true;
		}
		case FE_NODE_EXPR_NEGATIVE: {
			_IR_EXPR_UNARY("-")
			return true;
		}
		case FE_NODE_EXPR_BNOT: {
			_IR_EXPR_UNARY("~")
			return true;
		}
		case FE_NODE_EXPR_INC_LEFT: {
			_IR_EXPR_UNARY("++")
			return true;
		}
		case FE_NODE_EXPR_DEC_LEFT: {
			_IR_EXPR_UNARY("--")
			return true;
		}
		case FE_NODE_EXPR_INC_RIGHT: {
			_IR_EXPR_UNARY_POSTFIX("++")
			return true;
		}
		case FE_NODE_EXPR_DEC_RIGHT: {
			_IR_EXPR_UNARY_POSTFIX("--")
			return true;
		}
		case FE_NODE_EXPR_SIZEOF: {
			assert(node_expr->nchilds == 1);

			_IR_EXPR_AUTO_CAST_BEGIN

			rstr_append_with_cstr(rstr, "(sizeof(");
			
			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, node_expr->childs[0]);
			rstr_append_with_rstr(rstr, &rstr_type);
			rstr_free(&rstr_type);
			
			rstr_append_with_cstr(rstr, "))");

			_IR_EXPR_AUTO_CAST_END
			
			return true;
		}
		case FE_NODE_EXPR_CAST: {
			assert(node_expr->nchilds == 2);

			rstr_append_with_cstr(rstr, "((");

			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, node_expr->childs[0]);
			rstr_append_with_rstr(rstr, &rstr_type);
			rstr_free(&rstr_type);

			rstr_append_with_cstr(rstr, ")");
			_ir_expr(ctx, rstr, node_expr->childs[1]);
			rstr_append_with_cstr(rstr, ")");

			return true;
		}
		case FE_NODE_EXPR_REF: {
			_IR_EXPR_UNARY("&")
			return true;
		}
		case FE_NODE_EXPR_DEREF: {
			_IR_EXPR_UNARY("*")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_UNARY
	#undef	_IR_EXPR_UNARY_POSTFIX
}

static bool _ir_expr_mul(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_MUL(opt_str)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (opt_str));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_MUL: {
			_IR_EXPR_MUL(" * ")
			return true;
		}
		case FE_NODE_EXPR_DIV: {
			_IR_EXPR_MUL(" / ")
			return true;
		}
		case FE_NODE_EXPR_MOD: {
			_IR_EXPR_MUL(" % ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_MUL
}

static bool _ir_expr_add(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_ADD(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_ADD: {
			_IR_EXPR_ADD(" + ")
			return true;
		}
		case FE_NODE_EXPR_SUB: {
			_IR_EXPR_ADD(" - ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_ADD
}

static bool _ir_expr_shift(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_SHIFT(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_SHIFT_LEFT: {
			_IR_EXPR_SHIFT(" << ")
			return true;
		}
		case FE_NODE_EXPR_SHIFT_RIGHT: {
			_IR_EXPR_SHIFT(" >> ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_SHIFT
}

static bool _ir_expr_lt(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_LT(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_LT: {
			_IR_EXPR_LT(" < ")
			return true;
		}
		case FE_NODE_EXPR_LE: {
			_IR_EXPR_LT(" <= ")
			return true;
		}
		case FE_NODE_EXPR_GT: {
			_IR_EXPR_LT(" > ")
			return true;
		}
		case FE_NODE_EXPR_GE: {
			_IR_EXPR_LT(" >= ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_LT
}

static bool _ir_expr_eq(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_EQ(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_EQ: {
			_IR_EXPR_EQ(" == ")
			return true;
		}
		case FE_NODE_EXPR_NEQ: {
			_IR_EXPR_EQ(" != ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_EQ
}

static bool _ir_expr_band(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_BAND(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_BAND: {
			_IR_EXPR_BAND(" & ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_BAND
}

static bool _ir_expr_bxor(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_BXOR(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_BXOR: {
			_IR_EXPR_BXOR(" ^ ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_BXOR
}

static bool _ir_expr_bor(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_BOR(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		_IR_EXPR_AUTO_CAST_BEGIN	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
		_IR_EXPR_AUTO_CAST_END	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_BOR: {
			_IR_EXPR_BOR(" | ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_BOR
}

static bool _ir_expr_and(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_AND(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_AND: {
			_IR_EXPR_AND(" && ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_AND
}

static bool _ir_expr_or(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_OR(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_OR: {
			_IR_EXPR_OR(" || ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_OR
}

static bool _ir_expr_assign(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	#define	_IR_EXPR_ASSIGN(str_opt)	\
	{	\
		assert(node_expr->nchilds == 2);	\
		rstr_append_with_cstr(rstr, "(");	\
		_ir_expr(ctx, rstr, node_expr->childs[0]);	\
		rstr_append_with_cstr(rstr, (str_opt));	\
		_ir_expr(ctx, rstr, node_expr->childs[1]);	\
		rstr_append_with_cstr(rstr, ")");	\
	}

	switch (node_expr->type) {
		case FE_NODE_EXPR_ASSIGN: {
			_IR_EXPR_ASSIGN(" = ")
			return true;
		}
		case FE_NODE_EXPR_ADD_ASSIGN: {
			_IR_EXPR_ASSIGN(" += ")
			return true;
		}
		case FE_NODE_EXPR_SUB_ASSIGN: {
			_IR_EXPR_ASSIGN(" -= ")
			return true;
		}
		case FE_NODE_EXPR_MUL_ASSIGN: {
			_IR_EXPR_ASSIGN(" *= ")
			return true;
		}
		case FE_NODE_EXPR_DIV_ASSIGN: {
			_IR_EXPR_ASSIGN(" /= ")
			return true;
		}
		case FE_NODE_EXPR_MOD_ASSIGN: {
			_IR_EXPR_ASSIGN(" %= ")
			return true;
		}
		case FE_NODE_EXPR_BAND_ASSIGN: {
			_IR_EXPR_ASSIGN(" &= ")
			return true;
		}
		case FE_NODE_EXPR_BOR_ASSIGN: {
			_IR_EXPR_ASSIGN(" |= ")
			return true;
		}
		case FE_NODE_EXPR_BXOR_ASSIGN: {
			_IR_EXPR_ASSIGN(" ^= ")
			return true;
		}
		case FE_NODE_EXPR_SHIFT_LEFT_ASSIGN: {
			_IR_EXPR_ASSIGN(" <<= ")
			return true;
		}
		case FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN: {
			_IR_EXPR_ASSIGN(" >>= ")
			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_IR_EXPR_ASSIGN
}

static bool _ir_expr_cond(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	if (node_expr->type == FE_NODE_EXPR_COND) {
		assert(node_expr->nchilds == 3);
		rstr_append_with_cstr(rstr, "((");
		_ir_expr(ctx, rstr, node_expr->childs[0]);
		rstr_append_with_cstr(rstr, ")?(");
		_ir_expr(ctx, rstr, node_expr->childs[1]);
		rstr_append_with_cstr(rstr, "):(");
		_ir_expr(ctx, rstr, node_expr->childs[2]);
		rstr_append_with_cstr(rstr, "))");

		return true;
	} else {
		return false;
	}
}

static void _ir_expr(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);

	if (_ir_expr_constexpr(ctx, rstr, node_expr)) {
		return;
	}

	#define	_A(fn)	\
		if (fn(ctx, rstr, node_expr)) {	\
			return;	\
		}

	_A(_ir_expr_atom)
	_A(_ir_expr_member_arridx_fncall)
	_A(_ir_expr_unary)
	_A(_ir_expr_mul)
	_A(_ir_expr_add)
	_A(_ir_expr_shift)
	_A(_ir_expr_lt)
	_A(_ir_expr_eq)
	_A(_ir_expr_band)
	_A(_ir_expr_bxor)
	_A(_ir_expr_bor)
	_A(_ir_expr_and)
	_A(_ir_expr_or)
	_A(_ir_expr_cond)
	_A(_ir_expr_assign)

	assert(0);

	#undef	_A
}

static void _ir_expr_wrapper(
	IRGeneratorCContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_expr
) {
	assert(ctx);
	assert(rstr);
	assert(node_expr);
	assert(node_expr->type == FE_NODE_EXPR);
	assert(node_expr->nchilds == 1);

	if (_ir_expr_constexpr(ctx, rstr, node_expr)) {
		return;
	}

	ParserASTNode *child = node_expr->childs[0];
	_ir_expr(ctx, rstr, child);
}

static void _ir_var(IRGeneratorCContext *ctx, ParserASTNode *node) {
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

		ResizableString rstr_type;
		rstr_init(&rstr_type);

		if (node_var_item->nchilds == 2) {
			rstr_append_with_raw(&rstr_type, node_identifier->token->content, node_identifier->token->len);

			_ir_type(ctx, &rstr_type, node_type);

			if (align > 0) {
				rstr_prependf(&rstr_type, "__attribute__((aligned(%d)))", align);
			}

			_OUT_STR(ctx, rstr_type.buffer, rstr_type.len);
		} else {
			ParserASTNode *node_expr = node_var_item->childs[2];
			
			rstr_append_with_raw(&rstr_type, node_identifier->token->content, node_identifier->token->len);

			_ir_type(ctx, &rstr_type, node_type);

			if (align > 0) {
				rstr_prependf(&rstr_type, "__attribute__((aligned(%d)))", align);
			}

			_OUT_STR(ctx, rstr_type.buffer, rstr_type.len);

			_OUT_CSTR(ctx, " = ");

			ResizableString rstr_expr;
			rstr_init(&rstr_expr);
			_ir_expr_wrapper(ctx, &rstr_expr, node_expr);
			_OUT_STR(ctx, rstr_expr.buffer, rstr_expr.len);
			rstr_free(&rstr_expr);
		}

		rstr_free(&rstr_type);

		_OUT_CSTR(ctx, "; ");
	}
	_OUT_CSTR(ctx, "\n");
}

static void _ir_struct(IRGeneratorCContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);
	assert(node->nchilds == 2);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == FE_NODE_IDENTIFIER);
	ParserASTNode *node_struct_body = node->childs[1];

	_OUT_CSTR(ctx, "\nstruct ");
	_OUT_STR(ctx, node_identifier->token->content, node_identifier->token->len);
	if (node_struct_body->type == FE_NODE_STRUCT_BODY) {
		_OUT_CSTR(ctx, "\n{\n");
		for (int i = 0; i < node_struct_body->nchilds; i++) {
			_OUT_CSTR(ctx, "\t");
			ParserASTNode *node_var = node_struct_body->childs[i];
			_ir_var(ctx, node_var);
		}
		if (FE_STRUCT_AST_NODE_GET_PACKED(node)) {
			_OUT_CSTR(ctx, "} __attribute__((packed));\n");
		} else {
			_OUT_CSTR(ctx, "};\n");
		}
	} else if (node_struct_body->type == FE_NODE_DUMMY) {
		_OUT_CSTR(ctx, ";\n");
	} else {
		assert(0);
	}
}

static void _out_tab(IRGeneratorCContext *ctx, int indentLevel) {
	for (int i = 0; i < indentLevel; i++) {
		_OUT_CHAR(ctx, '\t');
	}
}

#define	_OUT_INDENT(ctx, lv)	{ _out_tab((ctx), (lv)); }

static void _ir_stats_block(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stats_block
);

static void _ir_stat_var(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	_OUT_INDENT(ctx, indentLevel);
	_ir_var(ctx, node);
}

static void _ir_stat_if(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_if
) {
	assert(ctx);
	assert(node_if);
	assert(node_if->type == FE_NODE_STAT_IF);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "if (");
	ResizableString rstr_if_expr;
	rstr_init(&rstr_if_expr);
	_ir_expr_wrapper(ctx, &rstr_if_expr, node_if->childs[0]);
	_OUT_STR(ctx, rstr_if_expr.buffer, rstr_if_expr.len);
	rstr_free(&rstr_if_expr);
	_OUT_CSTR(ctx, ")\n");
	_ir_stats_block(ctx, indentLevel, node_if->childs[1]);

	for (int i = 2; i < node_if->nchilds; i++) {
		ParserASTNode *node = node_if->childs[i];
		if (node->type == FE_NODE_STAT_ELIF) {
			_OUT_INDENT(ctx, indentLevel);
			_OUT_CSTR(ctx, "else if (");
			ResizableString rstr_elif_expr;
			rstr_init(&rstr_elif_expr);
			_ir_expr_wrapper(ctx, &rstr_elif_expr, node->childs[0]);
			_OUT_STR(ctx, rstr_elif_expr.buffer, rstr_elif_expr.len);
			rstr_free(&rstr_elif_expr);
			_OUT_CSTR(ctx, ")\n");
			_ir_stats_block(ctx, indentLevel, node->childs[1]);
		} else if (node->type == FE_NODE_STAT_ELSE) {
			_OUT_INDENT(ctx, indentLevel);
			_OUT_CSTR(ctx, "else\n");
			_ir_stats_block(ctx, indentLevel, node->childs[0]);
		} else {
			assert(0);
		}
	}
}

static void _ir_stat_while(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_while
) {
	assert(ctx);
	assert(node_stat_while);
	assert(node_stat_while->type == FE_NODE_STAT_WHILE);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "while (");
	ResizableString rstr_while_expr;
	rstr_init(&rstr_while_expr);
	_ir_expr_wrapper(ctx, &rstr_while_expr, node_stat_while->childs[0]);
	_OUT_STR(ctx, rstr_while_expr.buffer, rstr_while_expr.len);
	rstr_free(&rstr_while_expr);
	_OUT_CSTR(ctx, ")\n");
	_ir_stats_block(ctx, indentLevel, node_stat_while->childs[1]);
}

static void _ir_stat_do(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_do
) {
	assert(ctx);
	assert(node_stat_do);
	assert(node_stat_do->type == FE_NODE_STAT_DO);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "do\n");
	_ir_stats_block(ctx, indentLevel, node_stat_do->childs[0]);
	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "while (");
	ResizableString rstr_do_expr;
	rstr_init(&rstr_do_expr);
	_ir_expr_wrapper(ctx, &rstr_do_expr, node_stat_do->childs[1]);
	_OUT_STR(ctx, rstr_do_expr.buffer, rstr_do_expr.len);
	rstr_free(&rstr_do_expr);
	_OUT_CSTR(ctx, ");\n");
}

static void _ir_stat_for(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_for
) {
	assert(ctx);
	assert(node_stat_for);
	assert(node_stat_for->type == FE_NODE_STAT_FOR);
	assert(node_stat_for->nchilds == 4);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "{\n");

	ParserASTNode *node_stat_for_init = node_stat_for->childs[0];
	assert(node_stat_for_init->type == FE_NODE_STAT_FOR_INIT);
	if (node_stat_for_init->nchilds > 0) {
		_OUT_INDENT(ctx, indentLevel + 1);
		_ir_var(ctx, node_stat_for_init->childs[0]);
	}

	_OUT_INDENT(ctx, indentLevel + 1);
	_OUT_CSTR(ctx, "for (;");

	ParserASTNode *node_stat_for_expr_cond = node_stat_for->childs[1];
	assert(node_stat_for_expr_cond->type == FE_NODE_STAT_FOR_EXPR_COND);
	if (node_stat_for_expr_cond->nchilds > 0) {
		ResizableString rstr_for_expr_cond;
		rstr_init(&rstr_for_expr_cond);
		_ir_expr_wrapper(ctx, &rstr_for_expr_cond, node_stat_for_expr_cond->childs[0]);
		_OUT_STR(ctx, rstr_for_expr_cond.buffer, rstr_for_expr_cond.len);
		rstr_free(&rstr_for_expr_cond);
	}

	_OUT_CSTR(ctx, ";");

	ParserASTNode *node_stat_for_expr = node_stat_for->childs[2];
	assert(node_stat_for_expr->type == FE_NODE_STAT_FOR_EXPR);
	if (node_stat_for_expr->nchilds > 0) {
		ResizableString rstr_for_expr;
		rstr_init(&rstr_for_expr);
		_ir_expr_wrapper(ctx, &rstr_for_expr, node_stat_for_expr->childs[0]);
		_OUT_STR(ctx, rstr_for_expr.buffer, rstr_for_expr.len);
		rstr_free(&rstr_for_expr);
	}

	_OUT_CSTR(ctx, ")\n");

	ParserASTNode *node_stat_for_block = node_stat_for->childs[3];
	_ir_stats_block(ctx, indentLevel + 1, node_stat_for_block);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "}\n");
}

static void _ir_stat_break(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_break
) {
	assert(ctx);
	assert(node_stat_break);
	assert(node_stat_break->type == FE_NODE_STAT_BREAK);
	assert(node_stat_break->nchilds == 0);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "break;\n");
}

static void _ir_stat_continue(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_continue
) {
	assert(ctx);
	assert(node_stat_continue);
	assert(node_stat_continue->type == FE_NODE_STAT_CONTINUE);
	assert(node_stat_continue->nchilds == 0);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "continue;\n");
}

static void _ir_stat_return(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_return
) {
	assert(ctx);
	assert(node_stat_return);
	assert(node_stat_return->type == FE_NODE_STAT_RETURN);
	assert(node_stat_return->nchilds == 0 || node_stat_return->nchilds == 1);

	_OUT_INDENT(ctx, indentLevel);

	int nchilds = node_stat_return->nchilds;
	if (nchilds == 0) {
		_OUT_CSTR(ctx, "return");
	} else if (nchilds == 1) {
		_OUT_CSTR(ctx, "return ");
		ResizableString rstr_expr;
		rstr_init(&rstr_expr);
		_ir_expr_wrapper(ctx, &rstr_expr, node_stat_return->childs[0]);
		_OUT_STR(ctx, rstr_expr.buffer, rstr_expr.len);
		rstr_free(&rstr_expr);
	} else {
		assert(0);
	}

	_OUT_CSTR(ctx, ";\n");
}

static void _ir_stat_goto(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_goto
) {
	assert(ctx);
	assert(node_stat_goto);
	assert(node_stat_goto->type == FE_NODE_STAT_GOTO);
	assert(node_stat_goto->nchilds == 1);

	ParserASTNode *node_label = node_stat_goto->childs[0];
	assert(node_label->type == FE_NODE_IDENTIFIER);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "goto ");
	_OUT_STR(ctx, node_label->token->content, node_label->token->len);
	_OUT_CSTR(ctx, ";\n");
}

static void _ir_stat_label(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_label
) {
	assert(ctx);
	assert(node_stat_label);
	assert(node_stat_label->type == FE_NODE_STAT_LABEL);
	assert(node_stat_label->nchilds == 1);

	ParserASTNode *node_label = node_stat_label->childs[0];
	assert(node_label->type == FE_NODE_IDENTIFIER);

	_OUT_STR(ctx, node_label->token->content, node_label->token->len);
	_OUT_CSTR(ctx, ":;\n");
}

static void _ir_stat_asm(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat_asm
) {
	assert(ctx);
	assert(node_stat_asm);
	assert(node_stat_asm->type == FE_NODE_STAT_ASM);

	_error("asm is not supported.");
}

static void _ir_stat_expr(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_EXPR);

	_OUT_INDENT(ctx, indentLevel);

	ResizableString rstr_expr;
	rstr_init(&rstr_expr);
	_ir_expr_wrapper(ctx, &rstr_expr, node);
	_OUT_STR(ctx, rstr_expr.buffer, rstr_expr.len);
	rstr_free(&rstr_expr);

	_OUT_CSTR(ctx, ";\n");
}

static void _ir_stat_va_start(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_START);
	assert(node->nchilds == 1);

	

	_OUT_INDENT(ctx, indentLevel);

	_OUT_CSTR(ctx, "va_start(");

	_OUT_CSTR(ctx, ");\n");
}

static void _ir_stat(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stat
) {
	assert(ctx);
	assert(node_stat);

	switch (node_stat->type) {
		case FE_NODE_VAR: {
			_ir_stat_var(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_IF: {
			_ir_stat_if(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_WHILE: {
			_ir_stat_while(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_DO: {
			_ir_stat_do(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_FOR: {
			_ir_stat_for(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_BREAK: {
			_ir_stat_break(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_CONTINUE: {
			_ir_stat_continue(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_RETURN: {
			_ir_stat_return(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_GOTO: {
			_ir_stat_goto(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_LABEL: {
			_ir_stat_label(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_ASM: {
			_ir_stat_asm(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_EXPR: {
			_ir_stat_expr(ctx, indentLevel, node_stat);
			break;
		}
		case FE_NODE_STAT_DUMMY: {
			
			break;
		}
		case FE_NODE_STATS_BLOCK: {
			_ir_stats_block(ctx, indentLevel, node_stat);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _ir_stats_block(
	IRGeneratorCContext *ctx,
	int indentLevel,
	ParserASTNode *node_stats_block
) {
	assert(ctx);
	assert(node_stats_block);
	assert(node_stats_block->type == FE_NODE_STATS_BLOCK);

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "{\n");

	for (int i = 0; i < node_stats_block->nchilds; i++) {
		ParserASTNode *node_stat = node_stats_block->childs[i];
		_ir_stat(ctx, indentLevel + 1, node_stat);
	}

	_OUT_INDENT(ctx, indentLevel);
	_OUT_CSTR(ctx, "}\n");
}

static void _ir_func(IRGeneratorCContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_FUNC);

	ParserASTNode *node_identifier = NULL;
	ParserASTNode *node_func_params = NULL;
	ParserASTNode *node_return_type = NULL;
	ParserASTNode *node_body = NULL;

	int nchilds = node->nchilds;
	if (nchilds == 3) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_body = node->childs[2];
	} else if (nchilds == 4) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_return_type = node->childs[2];
		node_body = node->childs[3];
	} else {
		assert(0);
	}
	assert(node_identifier->type == FE_NODE_IDENTIFIER);
	assert(node_func_params->type == FE_NODE_FUNC_PARAMS);

	_OUT_CSTR(ctx, "\n");

	if (node_return_type == NULL) {
		_OUT_CSTR(ctx, "void ");

		_OUT_STR(ctx, node_identifier->token->content, node_identifier->token->len);
		_OUT_CSTR(ctx, " (");
		ResizableString rstr_func_params;
		rstr_init(&rstr_func_params);
		_ir_func_params(ctx, &rstr_func_params, node_func_params);
		_OUT_STR(ctx, rstr_func_params.buffer, rstr_func_params.len);
		rstr_free(&rstr_func_params);
		_OUT_CSTR(ctx, ")");
	} else {
		ResizableString rstr_func_sign;
		rstr_init(&rstr_func_sign);

		rstr_append_with_raw(&rstr_func_sign, node_identifier->token->content, node_identifier->token->len);
		rstr_append_with_cstr(&rstr_func_sign, " (");
		ResizableString rstr_func_params;
		rstr_init(&rstr_func_params);
		_ir_func_params(ctx, &rstr_func_params, node_func_params);
		rstr_append_with_rstr(&rstr_func_sign, &rstr_func_params);
		rstr_free(&rstr_func_params);
		rstr_append_with_cstr(&rstr_func_sign, ")");

		_ir_type(ctx, &rstr_func_sign, node_return_type);
		_OUT_STR(ctx, rstr_func_sign.buffer, rstr_func_sign.len);
		_OUT_CSTR(ctx, " ");
		rstr_free(&rstr_func_sign);
	}

	if (node_body->type == FE_NODE_DUMMY) {
		_OUT_CSTR(ctx, ";");
	} else {
		_OUT_CSTR(ctx, "\n");
		_ir_stats_block(ctx, 0, node_body);
	}
	_OUT_CSTR(ctx, "\n");
}

static void _ir_module_item(IRGeneratorCContext *ctx, ParserASTNode *node) {
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

static void _ir_module(IRGeneratorCContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_MODULE);

	_OUT_CSTR(ctx, "#include <inttypes.h>\n");
	_OUT_CSTR(ctx, "#include <stdarg.h>\n");
	_OUT_CSTR(ctx, "\n");

	_OUT_STR(ctx, ctx->global.buffer, ctx->global.len);

	for (int i = 0; i < node->nchilds; i++) {
		_ir_module_item(ctx, node->childs[i]);
	}
}




void fe_irgen_c_generate(IRGeneratorCContext *ctx) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == FE_NODE_MODULE);
	assert(ctx->out);

	_ir_module(ctx, ctx->psrctx->ast);
}

IRGeneratorCContext * fe_irgen_c_new_context(ParserContext *psrctx, FILE *out) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	
	IRGeneratorCContext *ctx = (IRGeneratorCContext *)malloc(sizeof(IRGeneratorCContext));
	if (ctx == NULL) {
		_error("cannot allocate memory for a new context!");
	}

	ctx->psrctx = psrctx;
	ctx->out = out;

	rstr_init(&(ctx->global));

	return ctx;
}

void fe_irgen_c_free_context(IRGeneratorCContext *ctx) {
	assert(ctx);

	rstr_free(&(ctx->global));

	free(ctx);
}
