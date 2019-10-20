#include "irgen.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "parser.h"
#include "lexer.h"
#include "semantics.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#define	_OUT_CHAR(ctx, chr)	{ fputc((chr), (ctx)->out); }
#define	_OUT_STR(ctx, str, len) { for (int __i = 0; __i < (len); __i++) { _OUT_CHAR((ctx), (str)[__i]) } }
#define	_OUT_CSTR(ctx, str) { _OUT_STR((ctx), (str), strlen((str))) }
#define	_OUT_FMT(ctx, fmt, ...) { fprintf((ctx)->out, fmt, __VA_ARGS__); }




static void _error(const char * msg) {
	assert(msg);
	fprintf(stderr, "IRGen Error: %s\n", msg);
	exit(1);
}




static size_t _next_no(
	IRGeneratorContext *ctx
) {
	assert(ctx);

	return ctx->counter++;
}




typedef struct {
	ResizableString rstr_for_result;
	ResizableString rstr_for_result_ptr;

	ResizableString rstr_result;

	ResizableString rstr_result_ptr;
} _ExprResult;

static void _expr_result_init(
	_ExprResult *expr_result
) {
	assert(expr_result);

	rstr_init(&(expr_result->rstr_for_result));
	rstr_init(&(expr_result->rstr_for_result_ptr));

	rstr_init(&(expr_result->rstr_result));

	rstr_init(&(expr_result->rstr_result_ptr));
}

static void _expr_result_free(
	_ExprResult *expr_result
) {
	assert(expr_result);

	rstr_free(&(expr_result->rstr_for_result));
	rstr_free(&(expr_result->rstr_for_result_ptr));

	rstr_free(&(expr_result->rstr_result));

	rstr_free(&(expr_result->rstr_result_ptr));
}

static void _expr_result_copy(
	_ExprResult *target,
	_ExprResult *source
) {
	assert(target);
	assert(source);

	rstr_append_with_rstr(&(target->rstr_for_result), &(source->rstr_for_result));
	rstr_append_with_rstr(&(target->rstr_for_result_ptr), &(source->rstr_for_result_ptr));
	rstr_append_with_rstr(&(target->rstr_result), &(source->rstr_result));
	rstr_append_with_rstr(&(target->rstr_result_ptr), &(source->rstr_result_ptr));
}

static void _expr_result_set_result(
	_ExprResult *result,
	const char *val
) {
	assert(result);
	assert(val);

	rstr_append_with_cstr(&(result->rstr_result), val);
}

static void _expr_result_set_result_ptr(
	_ExprResult *result,
	const char *val
) {
	assert(result);
	assert(val);

	rstr_append_with_cstr(&(result->rstr_result_ptr), val);
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

static void _ir_identifier_name_tk(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	LexerToken *tk_identifier
) {
	assert(ctx);
	assert(rstr);
	assert(tk_identifier);

	rstr_append_with_char(rstr, '_');
	rstr_append_with_raw(rstr, tk_identifier->content, tk_identifier->len);
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
					_ir_identifier_name_tk(ctx, rstr, node_id->token);	\
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

static size_t _generate_func_tmp(
	IRGeneratorContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);

	size_t no = FE_FUNC_SYMBOL_NEXT_NO(func_symbol);

	rstr_appendf(rstr, "__tmp.%zu__", no);

	return no;
}

#define	_TMP_LABEL(name)			__rstr_label_##name
#define	_TMP_LABEL_CSTR(name)		(RSTR_CSTR(&_TMP_LABEL(name)))

#define	_TMP_LABEL_DEF(ctx, name)	\
	ResizableString _TMP_LABEL(name);	\
	rstr_init(&_TMP_LABEL(name));	\
	_generate_func_tmp((ctx), (ctx)->func_symbol, &_TMP_LABEL(name));

#define	_TMP_LABEL_FREE(name)	\
	rstr_free(&_TMP_LABEL(name));

static size_t _generate_func_tmp_var_cstr(
	IRGeneratorContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr,
	const char *type
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);
	assert(type);

	size_t no = _generate_func_tmp(ctx, func_symbol, rstr);

	rstr_appendf(
		ctx->local_var_defs,
		"var %s:%s;\n",
		RSTR_CSTR(rstr),
		type
	);

	return no;
}

static size_t _generate_func_tmp_ptr_var_cstr(
	IRGeneratorContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr,
	const char *type
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);
	assert(type);

	size_t no = _generate_func_tmp(ctx, func_symbol, rstr);

	rstr_appendf(
		ctx->local_var_defs,
		"var %s:*%s;\n",
		RSTR_CSTR(rstr),
		type
	);

	return no;
}

static size_t _generate_func_tmp_var(
	IRGeneratorContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);
	assert(node_type);

	ResizableString rstr_type;
	rstr_init(&rstr_type);
	_ir_type(ctx, &rstr_type, node_type);

	size_t no = _generate_func_tmp_var_cstr(ctx, func_symbol, rstr, RSTR_CSTR(&rstr_type));

	rstr_free(&rstr_type);

	return no;
}

static size_t _generate_func_tmp_ptr_var(
	IRGeneratorContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);
	assert(node_type);

	ResizableString rstr_type;
	rstr_init(&rstr_type);
	_ir_type(ctx, &rstr_type, node_type);

	size_t no = _generate_func_tmp_ptr_var_cstr(ctx, func_symbol, rstr, RSTR_CSTR(&rstr_type));

	rstr_free(&rstr_type);

	return no;
}

static void _const_int(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ParserASTNode *type,
	int v
) {
	assert(ctx);
	assert(rstr);
	assert(type);

	ResizableString rstr_type;
	rstr_init(&rstr_type);
	_ir_type(ctx, &rstr_type, type);

	rstr_appendf(
		rstr,
		"cast<%s>(%d)",
		RSTR_CSTR(&rstr_type),
		v
	);

	rstr_free(&rstr_type);
}











static void _expr_result_cast(
	IRGeneratorContext *ctx,
	_ExprResult *target_result,
	ParserASTNode *target_type_node,
	_ExprResult *source_result,
	ParserASTNode *source_type_node
) {
	assert(ctx);
	assert(target_result);
	assert(target_type_node);
	assert(source_result);
	assert(source_type_node);

	if (fe_sem_is_compatible_type(ctx->psrctx, target_type_node, source_type_node, true)) {
		goto not_need_to_cast;
	}

	uint8_t target_type = fe_sem_get_type_by_type_node(ctx->psrctx, target_type_node);
	uint8_t source_type = fe_sem_get_type_by_type_node(ctx->psrctx, source_type_node);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(ctx, func_symbol, &rstr_tmp_label_val, target_type_node);

	ResizableString rstr_target_type;
	rstr_init(&rstr_target_type);
	_ir_type(ctx, &rstr_target_type, target_type_node);

	#define	_A(cmd)	\
		rstr_appendf(	\
			&(target_result->rstr_for_result),	\
			cmd" %s, %s;\n",	\
			RSTR_CSTR(&rstr_tmp_label_val),	\
			RSTR_CSTR(&(source_result->rstr_result))	\
		);	\
		_expr_result_set_result(	\
			target_result,	\
			RSTR_CSTR(&rstr_tmp_label_val)	\
		);

	if (fe_sem_is_signed_type(source_type)) {
		if (fe_sem_is_signed_type(target_type)) {
			size_t sz_target_type = fe_sem_get_primitive_type_size(target_type);
			size_t sz_source_type = fe_sem_get_primitive_type_size(source_type);

			if (sz_target_type < sz_source_type) {
				// 小的有符号整数 <- 大的有符号整数。
				_A("trunc")
			} else if (sz_target_type > sz_source_type) {
				// 大的有符号整数 <- 小的有符号整数。
				_A("sext")
			} else {
				_A("bitcast")
			}
		} else if (fe_sem_is_unsigned_type(target_type)) {
			size_t sz_target_type = fe_sem_get_primitive_type_size(target_type);
			size_t sz_source_type = fe_sem_get_primitive_type_size(source_type);

			if (sz_target_type < sz_source_type) {
				// 小的无符号整数 <- 大的有符号整数。
				_A("trunc")
			} else if (sz_target_type > sz_source_type) {
				// 大的无符号整数 <- 小的有符号整数。
				_A("sext")
			} else {
				_A("bitcast")
			}
		} else if (target_type == FE_TYPE_FLOAT) {
			// float <- 有符号整数。
			_A("sitofp")
		} else if (target_type == FE_TYPE_DOUBLE) {
			// double <- 有符号整数。
			_A("sitofp")
		} else if (target_type == FE_TYPE_POINTER) {
			// T* <- 有符号整数。
			_A("inttoptr")
		} else {
			assert(0);
		}
	} else if (fe_sem_is_unsigned_type(source_type)) {
		if (fe_sem_is_signed_type(target_type)) {
			size_t sz_target_type = fe_sem_get_primitive_type_size(target_type);
			size_t sz_source_type = fe_sem_get_primitive_type_size(source_type);

			if (sz_target_type < sz_source_type) {
				// 小的有符号整数 <- 大的无符号整数。
				_A("trunc")
			} else if (sz_target_type > sz_source_type) {
				// 大的有符号整数 <- 小的无符号整数。
				_A("zext")
			} else {
				_A("bitcast")
			}
		} else if (fe_sem_is_unsigned_type(target_type)) {
			size_t sz_target_type = fe_sem_get_primitive_type_size(target_type);
			size_t sz_source_type = fe_sem_get_primitive_type_size(source_type);

			if (sz_target_type < sz_source_type) {
				// 小的无符号整数 <- 大的无符号整数。
				_A("trunc")
			} else if (sz_target_type > sz_source_type) {
				// 大的无符号整数 <- 小的无符号整数。
				_A("zext")
			} else {
				_A("bitcast")
			}
		} else if (target_type == FE_TYPE_FLOAT) {
			// float <- 无符号整数。
			_A("uitofp")
		} else if (target_type == FE_TYPE_DOUBLE) {
			// double <- 无符号整数。
			_A("uitofp")
		} else if (target_type == FE_TYPE_POINTER) {
			// T* <- 无符号整数。
			_A("inttoptr")
		} else {
			assert(0);
		}
	} else if (source_type == FE_TYPE_FLOAT) {
		if (fe_sem_is_signed_type(target_type)) {
			// 有符号整数 <- float。
			_A("fptosi")
		} else if (fe_sem_is_unsigned_type(target_type)) {
			// 无符号整数 <- float。
			_A("fptoui")
		} else if (target_type == FE_TYPE_FLOAT) {
			// float <- float，不存在的。
			assert(0);
		} else if (target_type == FE_TYPE_DOUBLE) {
			// double <- float。
			_A("fpext")
		} else if (target_type == FE_TYPE_POINTER) {
			// T* <- float，不存在的。
			assert(0);
		} else {
			assert(0);
		}
	} else if (source_type == FE_TYPE_DOUBLE) {
		if (fe_sem_is_signed_type(target_type)) {
			// 有符号整数 <- double。
			_A("fptosi")
		} else if (fe_sem_is_unsigned_type(target_type)) {
			// 无符号整数 <- double。
			_A("fptoui")
		} else if (target_type == FE_TYPE_FLOAT) {
			// float <- double。
			_A("fptrunc")
		} else if (target_type == FE_TYPE_DOUBLE) {
			// double <- double，不存在的。
			assert(0);
		} else if (target_type == FE_TYPE_POINTER) {
			// T* <- double，不存在的。
			assert(0);
		} else {
			assert(0);
		}
	} else if (source_type == FE_TYPE_POINTER) {
		if (fe_sem_is_signed_type(target_type)) {
			// 有符号整数 <- T*。
			_A("ptrtoint")
		} else if (fe_sem_is_unsigned_type(target_type)) {
			// 无符号整数 <- T*。
			_A("ptrtoint")
		} else if (target_type == FE_TYPE_FLOAT) {
			// float <- T*，不存在的。
			assert(0);
		} else if (target_type == FE_TYPE_DOUBLE) {
			// double <- T*，不存在的。
			assert(0);
		} else if (target_type == FE_TYPE_POINTER) {
			// T* <- T*。
			_A("bitcast")
		} else {
			assert(0);
		}
	} else {
		assert(0);
	}

	rstr_free(&rstr_tmp_label_val);
	rstr_free(&rstr_target_type);
	return;

not_need_to_cast:
	_expr_result_set_result(target_result, RSTR_CSTR(&(source_result->rstr_result)));
	_expr_result_set_result_ptr(target_result, RSTR_CSTR(&(source_result->rstr_result_ptr)));

	#undef	_A
}




static void _ir_constexpr(
	IRGeneratorContext *ctx,
	ResizableString *rstr_val,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr_val);
	assert(node_constexpr);
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

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

static void _ir_expr(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
);

static bool _ir_expr_constexpr(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	if (!FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
		return false;
	}

	ResizableString rstr_val;
	rstr_init(&rstr_val);

	_ir_constexpr(ctx, &rstr_val, node);

	_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_val));

	rstr_free(&rstr_val);

	return true;
}

static bool _ir_expr_identifier(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	if (node->type != FE_NODE_EXPR_ATOM) {
		return false;
	}

	ParserASTNode *child = node->childs[0];
	if (child->type != FE_NODE_IDENTIFIER) {
		return false;
	}

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserSymbol *symbol = parser_get_symbol_from_node(ctx->psrctx, node, 0, child->token);
	assert(symbol);

	if (symbol->type == FE_SYM_VAR) {
		assert(FE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol));
		ResizableString *code_gen_name = FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);

		_expr_result_set_result(expr_result, RSTR_CSTR(code_gen_name));

		ParserASTNode *node_type = FE_VAR_SYMBOL_GET_TYPE_NODE(symbol);

		ResizableString rstr_tmp_label;
		rstr_init(&rstr_tmp_label);
		_generate_func_tmp_ptr_var(ctx, func_symbol, &rstr_tmp_label, node_type);

		rstr_appendf(
			&(expr_result->rstr_for_result_ptr),
			"ref %s, %s;\n",
			RSTR_CSTR(&rstr_tmp_label),
			RSTR_CSTR(code_gen_name)
		);

		_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_tmp_label));

		rstr_free(&rstr_tmp_label);
	} else if (symbol->type == FE_SYM_FUNC) {
		ResizableString rstr_id;
		rstr_init(&rstr_id);
		_ir_identifier_name(ctx, &rstr_id, child);

		_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_id));
		_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_id));

		rstr_free(&rstr_id);
	} else {
		assert(0);
	}

	return true;
}

static bool _ir_expr_member_arridx_fncall(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_MEMBER: {
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			
			_ExprResult result;
			_expr_result_init(&result);

			_ir_expr(ctx, rstr, &result, node_expr);
			rstr_append_with_rstr(rstr, &(result.rstr_for_result_ptr));

			ResizableString rstr_id;
			rstr_init(&rstr_id);
			_ir_identifier_name(ctx, &rstr_id, node_id);

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			assert(node_type->type == FE_NODE_TYPE_STRUCT);
			ParserASTNode *node_struct_id = node_type->childs[0];
			assert(node_struct_id->type == FE_NODE_IDENTIFIER);
			ResizableString rstr_struct_name;
			rstr_init_with_raw(
				&rstr_struct_name, node_struct_id->token->content, node_struct_id->token->len
			);

			ParserASTNode *node_struct_member_type = fe_sem_get_struct_member_type_node(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label0;
			rstr_init(&rstr_tmp_label0);
			_generate_func_tmp_ptr_var(
				ctx,
				func_symbol,
				&rstr_tmp_label0,
				node_struct_member_type
			);
			rstr_appendf(
				&(expr_result->rstr_for_result_ptr),
				"mbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label0),
				RSTR_CSTR(&(result.rstr_result_ptr)),
				RSTR_CSTR(&rstr_id)
			);
			_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_tmp_label0));

			ResizableString rstr_tmp_label1;
			rstr_init(&rstr_tmp_label1);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label1,
				node_struct_member_type
			);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"load %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label1),
				RSTR_CSTR(&(expr_result->rstr_result_ptr))
			);
			_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_tmp_label1));

			_expr_result_free(&result);

			rstr_free(&rstr_id);

			rstr_free(&rstr_struct_name);
			rstr_free(&rstr_tmp_label0);
			rstr_free(&rstr_tmp_label1);

			return true;
		}
		case FE_NODE_EXPR_PMEMBER: {
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			
			_ExprResult result;
			_expr_result_init(&result);

			_ir_expr(ctx, rstr, &result, node_expr);
			rstr_append_with_rstr(rstr, &(result.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result.rstr_for_result));

			ResizableString rstr_id;
			rstr_init(&rstr_id);
			_ir_identifier_name(ctx, &rstr_id, node_id);

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			assert(node_type->type == FE_NODE_TYPE_POINTER);
			assert(node_type->childs[0]->type == FE_NODE_TYPE_STRUCT);
			ParserASTNode *node_struct_id = node_type->childs[0]->childs[0];
			assert(node_struct_id->type == FE_NODE_IDENTIFIER);
			ResizableString rstr_struct_name;
			rstr_init_with_raw(
				&rstr_struct_name, node_struct_id->token->content, node_struct_id->token->len
			);

			ParserASTNode *node_struct_member_type = fe_sem_get_struct_member_type_node(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label0;
			rstr_init(&rstr_tmp_label0);
			_generate_func_tmp_ptr_var(
				ctx,
				func_symbol,
				&rstr_tmp_label0,
				node_struct_member_type
			);
			rstr_appendf(
				&(expr_result->rstr_for_result_ptr),
				"mbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label0),
				RSTR_CSTR(&(result.rstr_result)),
				RSTR_CSTR(&rstr_id)
			);
			_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_tmp_label0));

			ResizableString rstr_tmp_label1;
			rstr_init(&rstr_tmp_label1);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label1,
				node_struct_member_type
			);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"load %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label1),
				RSTR_CSTR(&(expr_result->rstr_result_ptr))
			);
			_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_tmp_label1));

			_expr_result_free(&result);

			rstr_free(&rstr_id);

			rstr_free(&rstr_struct_name);
			rstr_free(&rstr_tmp_label0);
			rstr_free(&rstr_tmp_label1);

			return true;
		}
		case FE_NODE_EXPR_ARRAY_INDEX: {
			assert(node->nchilds == 2);

			ParserASTNode *node_base = node->childs[0];
			uint8_t node_base_type = FE_EXPR_AST_NODE_GET_TYPE(node_base);
			assert(node_base_type == FE_TYPE_ARRAY
					|| node_base_type == FE_TYPE_POINTER);
			
			ParserASTNode *node_index = node->childs[1];

			_ExprResult result_base;
			_expr_result_init(&result_base);
			_ir_expr(ctx, rstr, &result_base, node_base);
			rstr_append_with_rstr(rstr, &(result_base.rstr_for_result_ptr));
			if (node_base_type == FE_TYPE_POINTER) {
				rstr_append_with_rstr(rstr, &(result_base.rstr_for_result));
			}

			_ExprResult result_index;
			_expr_result_init(&result_index);
			_ir_expr(ctx, rstr, &result_index, node_index);
			rstr_append_with_rstr(rstr, &(result_index.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_index.rstr_for_result));

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label_ptr;
			rstr_init(&rstr_tmp_label_ptr);
			_generate_func_tmp_ptr_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_ptr,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			if (node_base_type == FE_TYPE_ARRAY) {
				rstr_appendf(
					&(expr_result->rstr_for_result_ptr),
					"idx %s, %s, %s;\n",
					RSTR_CSTR(&rstr_tmp_label_ptr),
					RSTR_CSTR(&(result_base.rstr_result_ptr)),
					RSTR_CSTR(&(result_index.rstr_result))
				);
				_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_tmp_label_ptr));

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"load %s, %s;\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&(expr_result->rstr_result_ptr))
				);
				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_tmp_label_val));
			} else if (node_base_type == FE_TYPE_POINTER) {
				rstr_appendf(
					&(expr_result->rstr_for_result_ptr),
					"add %s, %s, %s;\n",
					RSTR_CSTR(&rstr_tmp_label_ptr),
					RSTR_CSTR(&(result_base.rstr_result)),
					RSTR_CSTR(&(result_index.rstr_result))
				);
				_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_tmp_label_ptr));

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"load %s, %s;\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&(expr_result->rstr_result_ptr))
				);
				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_tmp_label_val));
			} else {
				assert(0);
			}

			_expr_result_free(&result_base);
			_expr_result_free(&result_index);
			rstr_free(&rstr_tmp_label_ptr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_FUNC_CALL: {
			assert(node->nchilds == 2);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);
			
			_ExprResult result_func_expr;
			_expr_result_init(&result_func_expr);

			ParserASTNode *node_func_expr = node->childs[0];
			ParserASTNode *node_func_expr_func_ptr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_func_expr);
			assert(node_func_expr_func_ptr_type->type == FE_NODE_TYPE_POINTER);
			ParserASTNode *node_func_expr_func_type = node_func_expr_func_ptr_type->childs[0];
			assert(node_func_expr_func_type->type == FE_NODE_TYPE_FUNC);

			_ir_expr(ctx, rstr, &result_func_expr, node_func_expr);
			rstr_append_with_rstr(rstr, &(result_func_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_func_expr.rstr_for_result));

			ParserASTNode *node_func_type_params = node_func_expr_func_type->childs[0];
			ParserASTNode *node_func_type_return_type = NULL;
			if (node_func_expr_func_type->nchilds == 2) {
				node_func_type_return_type = node_func_expr_func_type->childs[1];
			}

			ParserASTNode *node_func_params = node->childs[1];

			ResizableString rstr_tmp_label_val;

			if (node_func_type_return_type != NULL) {
				rstr_init(&rstr_tmp_label_val);
				_generate_func_tmp_var(
					ctx,
					func_symbol,
					&rstr_tmp_label_val,
					node_func_type_return_type
				);

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"call %s, ",
					RSTR_CSTR(&rstr_tmp_label_val)
				);
			} else {
				rstr_append_with_cstr(
					&(expr_result->rstr_for_result),
					"vcall "
				);
			}

			ResizableString rstr_func_params;
			rstr_init(&rstr_func_params);
			for (int i = 0; i < node_func_params->nchilds; i++) {
				ParserASTNode *node_func_param = node_func_params->childs[i];

				_ExprResult result_func_param;
				_expr_result_init(&result_func_param);

				_ir_expr(ctx, rstr, &result_func_param, node_func_param);
				rstr_append_with_rstr(rstr, &(result_func_param.rstr_for_result_ptr));
				rstr_append_with_rstr(rstr, &(result_func_param.rstr_for_result));

				if (i < node_func_type_params->nchilds
						&& node_func_type_params->childs[i]->type == FE_NODE_FUNC_PARAMS_ITEM) {
					ParserASTNode *node_func_type_param = node_func_type_params->childs[i];
					ParserASTNode *node_func_type_param_type = node_func_type_param->childs[1];

					_ExprResult result_casted_func_param;
					_expr_result_init(&result_casted_func_param);

					_expr_result_cast(
						ctx,
						&result_casted_func_param,
						node_func_type_param_type,
						&result_func_param,
						FE_EXPR_AST_NODE_GET_TYPE_NODE(node_func_param)
					);
					rstr_append_with_rstr(rstr, &(result_casted_func_param.rstr_for_result_ptr));
					rstr_append_with_rstr(rstr, &(result_casted_func_param.rstr_for_result));

					rstr_append_with_rstr(&rstr_func_params, &(result_casted_func_param.rstr_result));

					_expr_result_free(&result_casted_func_param);
				} else {
					rstr_append_with_rstr(&rstr_func_params, &(result_func_param.rstr_result));
				}

				_expr_result_free(&result_func_param);

				if (i + 1 < node_func_params->nchilds) {
					rstr_append_with_cstr(&rstr_func_params, ", ");
				}
			}

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s(%s);\n",
				RSTR_CSTR(&(result_func_expr.rstr_result)),
				RSTR_CSTR(&rstr_func_params)
			);

			if (node_func_type_return_type != NULL) {
				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_tmp_label_val));

				rstr_free(&rstr_tmp_label_val);
			}

			_expr_result_free(&result_func_expr);
			rstr_free(&rstr_func_params);

			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_unary(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_NOT: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			ParserASTNode *node_expr = node->childs[0];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"not %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_POSITIVE: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			ParserASTNode *node_expr = node->childs[0];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result))
			);

			_expr_result_free(&result_expr);

			return true;
		}
		case FE_NODE_EXPR_NEGATIVE: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			ParserASTNode *node_expr = node->childs[0];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"neg %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_BNOT: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			ParserASTNode *node_expr = node->childs[0];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"bnot %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_INC_LEFT:
		case FE_NODE_EXPR_INC_RIGHT:
		case FE_NODE_EXPR_DEC_LEFT:
		case FE_NODE_EXPR_DEC_RIGHT: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			ParserASTNode *node_expr = node->childs[0];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(
				ctx,
				&rstr_type,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			if (node->type == FE_NODE_EXPR_INC_LEFT
					|| node->type == FE_NODE_EXPR_DEC_LEFT) {
				if (node->type == FE_NODE_EXPR_INC_LEFT) {
					rstr_appendf(
						&(expr_result->rstr_for_result),
						"add %s, %s, cast<%s>(1);\n",
						RSTR_CSTR(&rstr_tmp_label_val),
						RSTR_CSTR(&(result_expr.rstr_result)),
						RSTR_CSTR(&rstr_type)
					);
				} else {
					rstr_appendf(
						&(expr_result->rstr_for_result),
						"sub %s, %s, cast<%s>(1);\n",
						RSTR_CSTR(&rstr_tmp_label_val),
						RSTR_CSTR(&(result_expr.rstr_result)),
						RSTR_CSTR(&rstr_type)
					);
				}

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"store %s, %s;\n",
					RSTR_CSTR(&(result_expr.rstr_result_ptr)),
					RSTR_CSTR(&rstr_tmp_label_val)
				);

				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&rstr_tmp_label_val)
				);
			} else if (node->type == FE_NODE_EXPR_INC_RIGHT
					|| node->type == FE_NODE_EXPR_DEC_RIGHT) {
				ResizableString rstr_tmp_label_old_val;
				rstr_init(&rstr_tmp_label_old_val);
				_generate_func_tmp_var(
					ctx,
					func_symbol,
					&rstr_tmp_label_old_val,
					FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
				);

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"assign %s, %s;\n",
					RSTR_CSTR(&rstr_tmp_label_old_val),
					RSTR_CSTR(&(result_expr.rstr_result))
				);

				if (node->type == FE_NODE_EXPR_INC_RIGHT) {
					rstr_appendf(
						&(expr_result->rstr_for_result),
						"add %s, %s, cast<%s>(1);\n",
						RSTR_CSTR(&rstr_tmp_label_val),
						RSTR_CSTR(&(result_expr.rstr_result)),
						RSTR_CSTR(&rstr_type)
					);
				} else {
					rstr_appendf(
						&(expr_result->rstr_for_result),
						"sub %s, %s, cast<%s>(1);\n",
						RSTR_CSTR(&rstr_tmp_label_val),
						RSTR_CSTR(&(result_expr.rstr_result)),
						RSTR_CSTR(&rstr_type)
					);
				}

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"store %s, %s;\n",
					RSTR_CSTR(&(result_expr.rstr_result_ptr)),
					RSTR_CSTR(&rstr_tmp_label_val)
				);

				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&rstr_tmp_label_old_val)
				);

				rstr_free(&rstr_tmp_label_old_val);
			} else {
				assert(0);
			}

			rstr_free(&rstr_type);
			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_SIZEOF: {
			assert(0);

			return true;
		}
		case FE_NODE_EXPR_CAST: {
			assert(node->nchilds == 2);

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			_ExprResult casted_result;
			_expr_result_init(&casted_result);

			ParserASTNode *node_type = node->childs[0];
			ParserASTNode *node_expr = node->childs[1];

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			_expr_result_cast(
				ctx,
				&casted_result,
				node_type,
				&result_expr,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr)
			);
			rstr_append_with_rstr(rstr, &(casted_result.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(casted_result.rstr_for_result));

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(casted_result.rstr_result))
			);

			_expr_result_free(&result_expr);
			_expr_result_free(&casted_result);

			return true;
		}
		case FE_NODE_EXPR_REF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_expr = node->childs[0];

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result_ptr))
			);

			_expr_result_free(&result_expr);

			return true;
		}
		case FE_NODE_EXPR_DEREF: {
			assert(node->nchilds == 1);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ParserASTNode *node_expr = node->childs[0];

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
			);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"load %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_set_result_ptr(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result))
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_VA_ARG: {
			assert(node->nchilds == 2);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_type = node->childs[1];

			_ExprResult result_expr;
			_expr_result_init(&result_expr);

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, node_type);

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				node_type
			);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"__va_arg %s, %s, %s;\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&rstr_type),
				RSTR_CSTR(&(result_expr.rstr_result_ptr))
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_expr);

			rstr_free(&rstr_type);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_mul(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_MUL
			|| node->type == FE_NODE_EXPR_MUL_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_lhs_casted;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_lhs_casted);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_lhs_casted,
		node_type,
		&result_lhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs)
	);
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_type,
		&result_rhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs)
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
	);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"mul %s, %s, %s;\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs_casted.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result))
	);
	
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_div(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_DIV
			|| node->type == FE_NODE_EXPR_DIV_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_lhs_casted;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_lhs_casted);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_lhs_casted,
		node_type,
		&result_lhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs)
	);
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_type,
		&result_rhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs)
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
	);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"div %s, %s, %s;\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs_casted.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result))
	);
	
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_mod(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_MOD
			|| node->type == FE_NODE_EXPR_MOD_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_lhs_casted;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_lhs_casted);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_lhs_casted,
		node_type,
		&result_lhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs)
	);
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_type,
		&result_rhs,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs)
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
	);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"rem %s, %s, %s;\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs_casted.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result))
	);
	
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_mul_div_mod(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_MUL: {
			assert(node->nchilds == 2);

			_ir_expr_mul(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		case FE_NODE_EXPR_DIV: {
			assert(node->nchilds == 2);

			_ir_expr_div(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		case FE_NODE_EXPR_MOD: {
			assert(node->nchilds == 2);

			_ir_expr_mod(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_add(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_ADD
			|| node->type == FE_NODE_EXPR_ADD_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	uint8_t type1 = FE_EXPR_AST_NODE_GET_TYPE(node->childs[0]);
	uint8_t type2 = FE_EXPR_AST_NODE_GET_TYPE(node->childs[1]);

	if (fe_sem_is_pointer_type(type1) && fe_sem_is_integer_type(type2)) {
		ParserASTNode *node_lhs = node->childs[0];
		ParserASTNode *node_rhs = node->childs[1];

		_ExprResult result_lhs;
		_ExprResult result_rhs;

		_expr_result_init(&result_lhs);
		_expr_result_init(&result_rhs);

		_ir_expr(ctx, rstr, &result_lhs, node_lhs);
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

		if (rstr_result_lhs_ptr != NULL) {
			rstr_append_with_rstr(
				rstr_result_lhs_ptr,
				&(result_lhs.rstr_result_ptr)
			);
		}

		_ir_expr(ctx, rstr, &result_rhs, node_rhs);
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp_var(
			ctx,
			func_symbol,
			&rstr_tmp_label_val,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
		);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"add %s, %s, %s;\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs.rstr_result))
		);

		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_tmp_label_val)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		rstr_free(&rstr_tmp_label_val);
	} else if (fe_sem_is_number_type(type1) && fe_sem_is_number_type(type2)) {
		ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

		ParserASTNode *node_lhs = node->childs[0];
		ParserASTNode *node_rhs = node->childs[1];

		_ExprResult result_lhs;
		_ExprResult result_rhs;
		_ExprResult result_lhs_casted;
		_ExprResult result_rhs_casted;

		_expr_result_init(&result_lhs);
		_expr_result_init(&result_rhs);
		_expr_result_init(&result_lhs_casted);
		_expr_result_init(&result_rhs_casted);

		_ir_expr(ctx, rstr, &result_lhs, node_lhs);
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

		if (rstr_result_lhs_ptr != NULL) {
			rstr_append_with_rstr(
				rstr_result_lhs_ptr,
				&(result_lhs.rstr_result_ptr)
			);
		}

		_ir_expr(ctx, rstr, &result_rhs, node_rhs);
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

		_expr_result_cast(
			ctx,
			&result_lhs_casted,
			node_type,
			&result_lhs,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs)
		);
		rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

		_expr_result_cast(
			ctx,
			&result_rhs_casted,
			node_type,
			&result_rhs,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs)
		);
		rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp_var(
			ctx,
			func_symbol,
			&rstr_tmp_label_val,
			node_type
		);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"add %s, %s, %s;\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result))
		);

		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_tmp_label_val)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		_expr_result_free(&result_lhs_casted);
		_expr_result_free(&result_rhs_casted);
		rstr_free(&rstr_tmp_label_val);
	} else {
		assert(0);
	}
}

static void _ir_expr_sub(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_SUB
			|| node->type == FE_NODE_EXPR_SUB_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	uint8_t type1 = FE_EXPR_AST_NODE_GET_TYPE(node->childs[0]);
	uint8_t type2 = FE_EXPR_AST_NODE_GET_TYPE(node->childs[1]);

	if (fe_sem_is_pointer_type(type1) && fe_sem_is_integer_type(type2)) {
		ParserASTNode *node_lhs = node->childs[0];
		ParserASTNode *node_rhs = node->childs[1];

		_ExprResult result_lhs;
		_ExprResult result_rhs;

		_expr_result_init(&result_lhs);
		_expr_result_init(&result_rhs);

		_ir_expr(ctx, rstr, &result_lhs, node_lhs);
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

		if (rstr_result_lhs_ptr != NULL) {
			rstr_append_with_rstr(
				rstr_result_lhs_ptr,
				&(result_lhs.rstr_result_ptr)
			);
		}

		_ir_expr(ctx, rstr, &result_rhs, node_rhs);
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp_var(
			ctx,
			func_symbol,
			&rstr_tmp_label_val,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
		);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"sub %s, %s, %s;\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs.rstr_result))
		);

		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_tmp_label_val)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		rstr_free(&rstr_tmp_label_val);
	} else if (fe_sem_is_number_type(type1) && fe_sem_is_number_type(type2)) {
		ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

		ParserASTNode *node_lhs = node->childs[0];
		ParserASTNode *node_rhs = node->childs[1];

		_ExprResult result_lhs;
		_ExprResult result_rhs;
		_ExprResult result_lhs_casted;
		_ExprResult result_rhs_casted;

		_expr_result_init(&result_lhs);
		_expr_result_init(&result_rhs);
		_expr_result_init(&result_lhs_casted);
		_expr_result_init(&result_rhs_casted);

		_ir_expr(ctx, rstr, &result_lhs, node_lhs);
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

		if (rstr_result_lhs_ptr != NULL) {
			rstr_append_with_rstr(
				rstr_result_lhs_ptr,
				&(result_lhs.rstr_result_ptr)
			);
		}

		_ir_expr(ctx, rstr, &result_rhs, node_rhs);
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

		_expr_result_cast(
			ctx,
			&result_lhs_casted,
			node_type,
			&result_lhs,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs)
		);
		rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

		_expr_result_cast(
			ctx,
			&result_rhs_casted,
			node_type,
			&result_rhs,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs)
		);
		rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp_var(
			ctx,
			func_symbol,
			&rstr_tmp_label_val,
			node_type
		);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"sub %s, %s, %s;\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result))
		);

		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_tmp_label_val)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		_expr_result_free(&result_lhs_casted);
		_expr_result_free(&result_rhs_casted);
		rstr_free(&rstr_tmp_label_val);
	} else {
		assert(0);
	}
}

static bool _ir_expr_add_sub(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_ADD: {
			assert(node->nchilds == 2);

			_ir_expr_add(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		case FE_NODE_EXPR_SUB: {
			assert(node->nchilds == 2);

			_ir_expr_sub(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_shift_left(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_SHIFT_LEFT
			|| node->type == FE_NODE_EXPR_SHIFT_LEFT_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	ParserASTNode *node_lhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
	ParserASTNode *node_rhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs);

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_lhs_type,
		&result_rhs,
		node_rhs_type
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
	);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"shl %s, %s, %s;\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result))
	);

	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_shift_right(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_SHIFT_RIGHT
			|| node->type == FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	ParserASTNode *node_lhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
	ParserASTNode *node_rhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs);

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_lhs_type,
		&result_rhs,
		node_rhs_type
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node)
	);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"shr %s, %s, %s;\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result))
	);

	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_shift(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_SHIFT_LEFT: {
			assert(node->nchilds == 2);

			_ir_expr_shift_left(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		case FE_NODE_EXPR_SHIFT_RIGHT: {
			assert(node->nchilds == 2);

			_ir_expr_shift_right(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_comparison(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_LT:
		case FE_NODE_EXPR_LE:
		case FE_NODE_EXPR_GT:
		case FE_NODE_EXPR_GE:
		case FE_NODE_EXPR_EQ:
		case FE_NODE_EXPR_NEQ: {
			assert(node->nchilds == 2);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ParserASTNode *node_lhs = node->childs[0];
			ParserASTNode *node_rhs = node->childs[1];

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

			ParserASTNode *node_lhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
			ParserASTNode *node_rhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs);
			
			uint8_t lhs_type = FE_EXPR_AST_NODE_GET_TYPE(node_lhs);
			uint8_t rhs_type = FE_EXPR_AST_NODE_GET_TYPE(node_rhs);
			
			ParserASTNode *node_target_type = NULL;
			uint8_t target_type;
			if (lhs_type == FE_TYPE_POINTER) {
				node_target_type = node_lhs_type;
				target_type = lhs_type;
			} else if (rhs_type == FE_TYPE_POINTER) {
				node_target_type = node_rhs_type;
				target_type = rhs_type;
			} else {
				int32_t lhs_type_weight = fe_sem_get_primitive_type_weight(lhs_type);
				int32_t rhs_type_weight = fe_sem_get_primitive_type_weight(rhs_type); 
				if (lhs_type_weight > rhs_type_weight) {
					node_target_type = node_lhs_type;
				} else {
					node_target_type = node_rhs_type;
				}
				target_type = fe_sem_get_type_by_type_node(ctx->psrctx, node_target_type);
			}

			_ExprResult result_lhs;
			_ExprResult result_rhs;
			_ExprResult result_lhs_casted;
			_ExprResult result_rhs_casted;

			_expr_result_init(&result_lhs);
			_expr_result_init(&result_rhs);
			_expr_result_init(&result_lhs_casted);
			_expr_result_init(&result_rhs_casted);

			_ir_expr(ctx, rstr, &result_lhs, node_lhs);
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

			_ir_expr(ctx, rstr, &result_rhs, node_rhs);
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

			_expr_result_cast(
				ctx,
				&result_lhs_casted,
				node_target_type,
				&result_lhs,
				node_lhs_type
			);
			rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

			_expr_result_cast(
				ctx,
				&result_rhs_casted,
				node_target_type,
				&result_rhs,
				node_rhs_type
			);
			rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp_var(
				ctx,
				func_symbol,
				&rstr_tmp_label_val,
				node_type
			);

			#define	_A(cmd)	\
				rstr_appendf(	\
					&(expr_result->rstr_for_result),	\
					cmd" %s, %s, %s;\n",	\
					RSTR_CSTR(&rstr_tmp_label_val),	\
					RSTR_CSTR(&(result_lhs_casted.rstr_result)),	\
					RSTR_CSTR(&(result_rhs_casted.rstr_result))	\
				);

			switch (node->type) {
				case FE_NODE_EXPR_LT: {
					_A("lt")
					break;
				}
				case FE_NODE_EXPR_LE: {
					_A("le")
					break;
				}
				case FE_NODE_EXPR_GT: {
					_A("gt")
					break;
				}
				case FE_NODE_EXPR_GE: {
					_A("ge")
					break;
				}
				case FE_NODE_EXPR_EQ: {
					_A("eq")
					break;
				}
				case FE_NODE_EXPR_NEQ: {
					_A("neq")
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			#undef	_A

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_lhs);
			_expr_result_free(&result_rhs);
			_expr_result_free(&result_lhs_casted);
			_expr_result_free(&result_rhs_casted);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_bitwise(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *rstr_result_lhs_ptr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR_BAND
			|| node->type == FE_NODE_EXPR_BAND_ASSIGN
			|| node->type == FE_NODE_EXPR_BXOR
			|| node->type == FE_NODE_EXPR_BXOR_ASSIGN
			|| node->type == FE_NODE_EXPR_BOR
			|| node->type == FE_NODE_EXPR_BOR_ASSIGN);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);

	ParserASTNode *node_lhs = node->childs[0];
	ParserASTNode *node_rhs = node->childs[1];

	ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

	ParserASTNode *node_lhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
	ParserASTNode *node_rhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs);

	_ExprResult result_lhs;
	_ExprResult result_rhs;
	_ExprResult result_lhs_casted;
	_ExprResult result_rhs_casted;

	_expr_result_init(&result_lhs);
	_expr_result_init(&result_rhs);
	_expr_result_init(&result_lhs_casted);
	_expr_result_init(&result_rhs_casted);

	_ir_expr(ctx, rstr, &result_lhs, node_lhs);
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

	if (rstr_result_lhs_ptr != NULL) {
		rstr_append_with_rstr(
			rstr_result_lhs_ptr,
			&(result_lhs.rstr_result_ptr)
		);
	}

	_ir_expr(ctx, rstr, &result_rhs, node_rhs);
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_lhs_casted,
		node_type,
		&result_lhs,
		node_lhs_type
	);
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_lhs_casted.rstr_for_result));

	_expr_result_cast(
		ctx,
		&result_rhs_casted,
		node_type,
		&result_rhs,
		node_rhs_type
	);
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

	ResizableString rstr_tmp_label_val;
	rstr_init(&rstr_tmp_label_val);
	_generate_func_tmp_var(
		ctx,
		func_symbol,
		&rstr_tmp_label_val,
		node_type
	);

	#define	_A(cmd)	\
		rstr_appendf(	\
			&(expr_result->rstr_for_result),	\
			cmd" %s, %s, %s;\n",	\
			RSTR_CSTR(&rstr_tmp_label_val),	\
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),	\
			RSTR_CSTR(&(result_rhs_casted.rstr_result))	\
		);

	switch (node->type) {
		case FE_NODE_EXPR_BAND:
		case FE_NODE_EXPR_BAND_ASSIGN: {
			_A("band")
			break;
		}
		case FE_NODE_EXPR_BXOR:
		case FE_NODE_EXPR_BXOR_ASSIGN: {
			_A("bxor")
			break;
		}
		case FE_NODE_EXPR_BOR:
		case FE_NODE_EXPR_BOR_ASSIGN: {
			_A("bor")
			break;
		}
		default: {
			assert(0);
		}
	}

	#undef	_A

	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_band_bxor_bor(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_BAND:
		case FE_NODE_EXPR_BXOR:
		case FE_NODE_EXPR_BOR: {
			assert(node->nchilds == 2);

			_ir_expr_bitwise(ctx, rstr, NULL, expr_result, node);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_cond_expr_with_rstr(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	ResizableString *cond_expr_label,
	_ExprResult *expr_result,
	ParserASTNode *expr_node
) {
	assert(ctx);
	assert(rstr);
	assert(cond_expr_label);
	assert(expr_result);
	assert(expr_node);

	ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(expr_node);

	ResizableString rstr_type;
	rstr_init(&rstr_type);
	_ir_type(ctx, &rstr_type, node_type);

	ResizableString rstr_tmp;
	rstr_init(&rstr_tmp);
	_generate_func_tmp_var(
		ctx,
		ctx->func_symbol,
		&rstr_tmp,
		FE_SEM_NODE_TYPE_INT64
	);

	rstr_appendf(
		rstr,
		"neq %s, %s, cast<%s>(0);\n",
		RSTR_CSTR(&rstr_tmp),
		RSTR_CSTR(&(expr_result->rstr_result)),
		RSTR_CSTR(&rstr_type)
	);

	rstr_append_with_cstr(cond_expr_label, RSTR_CSTR(&rstr_tmp));

	rstr_free(&rstr_type);
	rstr_free(&rstr_tmp);
}

static void _ir_cond_expr(
	IRGeneratorContext *ctx,
	ResizableString *cond_expr_label,
	_ExprResult *expr_result,
	ParserASTNode *expr_node
) {
	return _ir_cond_expr_with_rstr(
		ctx,
		ctx->body,
		cond_expr_label,
		expr_result,
		expr_node
	);
}

static bool _ir_expr_logic(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_AND: {
			assert(node->nchilds == 2);

			ParserASTNode *node_lhs = node->childs[0];
			ParserASTNode *node_rhs = node->childs[1];

			ParserASTNode *node_target_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

			ResizableString rstr_zero;
			rstr_init(&rstr_zero);
			_const_int(ctx, &rstr_zero, node_target_type, 0);

			ResizableString rstr_one;
			rstr_init(&rstr_one);
			_const_int(ctx, &rstr_one, node_target_type, 1);

			ResizableString rstr_result;
			rstr_init(&rstr_result);
			_generate_func_tmp_var(
				ctx,
				ctx->func_symbol,
				&rstr_result,
				node_target_type
			);

			rstr_appendf(
				rstr,
				"assign %s, %s;\n",
				RSTR_CSTR(&rstr_result),
				RSTR_CSTR(&rstr_zero)
			);

			_ExprResult result_lhs;
			_expr_result_init(&result_lhs);

			_ir_expr(ctx, rstr, &result_lhs, node_lhs);
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

			ResizableString rstr_cond_expr_lhs;
			rstr_init(&rstr_cond_expr_lhs);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_cond_expr_lhs, &result_lhs, node_lhs);

			_TMP_LABEL_DEF(ctx, lhs_true)
			_TMP_LABEL_DEF(ctx, rhs_true)
			_TMP_LABEL_DEF(ctx, end)

			rstr_appendf(
				rstr,
				"cbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_cond_expr_lhs),
				_TMP_LABEL_CSTR(lhs_true),
				_TMP_LABEL_CSTR(end)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(lhs_true)
			);

			_ExprResult result_rhs;
			_expr_result_init(&result_rhs);

			_ir_expr(ctx, rstr, &result_rhs, node_rhs);
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

			ResizableString rstr_cond_expr_rhs;
			rstr_init(&rstr_cond_expr_rhs);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_cond_expr_rhs, &result_rhs, node_rhs);

			rstr_appendf(
				rstr,
				"cbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_cond_expr_rhs),
				_TMP_LABEL_CSTR(rhs_true),
				_TMP_LABEL_CSTR(end)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(rhs_true)
			);

			rstr_appendf(
				rstr,
				"assign %s, %s;\n",
				RSTR_CSTR(&rstr_result),
				RSTR_CSTR(&rstr_one)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(end)
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_result)
			);

			rstr_free(&rstr_zero);
			rstr_free(&rstr_one);
			rstr_free(&rstr_result);

			_expr_result_free(&result_lhs);
			rstr_free(&rstr_cond_expr_lhs);

			_TMP_LABEL_FREE(lhs_true)
			_TMP_LABEL_FREE(rhs_true)
			_TMP_LABEL_FREE(end)

			_expr_result_free(&result_rhs);
			rstr_free(&rstr_cond_expr_rhs);

			return true;
		}
		case FE_NODE_EXPR_OR: {
			assert(node->nchilds == 2);

			ParserASTNode *node_lhs = node->childs[0];
			ParserASTNode *node_rhs = node->childs[1];

			ParserASTNode *node_target_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

			ResizableString rstr_zero;
			rstr_init(&rstr_zero);
			_const_int(ctx, &rstr_zero, node_target_type, 0);

			ResizableString rstr_one;
			rstr_init(&rstr_one);
			_const_int(ctx, &rstr_one, node_target_type, 1);

			ResizableString rstr_result;
			rstr_init(&rstr_result);
			_generate_func_tmp_var(
				ctx,
				ctx->func_symbol,
				&rstr_result,
				node_target_type
			);

			rstr_appendf(
				rstr,
				"assign %s, %s;\n",
				RSTR_CSTR(&rstr_result),
				RSTR_CSTR(&rstr_zero)
			);

			_ExprResult result_lhs;
			_expr_result_init(&result_lhs);

			_ir_expr(ctx, rstr, &result_lhs, node_lhs);
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

			ResizableString rstr_cond_expr_lhs;
			rstr_init(&rstr_cond_expr_lhs);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_cond_expr_lhs, &result_lhs, node_lhs);

			_TMP_LABEL_DEF(ctx, lhs_true)
			_TMP_LABEL_DEF(ctx, lhs_false)
			_TMP_LABEL_DEF(ctx, rhs_true)
			_TMP_LABEL_DEF(ctx, end)

			rstr_appendf(
				rstr,
				"cbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_cond_expr_lhs),
				_TMP_LABEL_CSTR(lhs_true),
				_TMP_LABEL_CSTR(lhs_false)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(lhs_true)
			);

			rstr_appendf(
				rstr,
				"assign %s, %s;\n",
				RSTR_CSTR(&rstr_result),
				RSTR_CSTR(&rstr_one)
			);

			rstr_appendf(
				rstr,
				"br %s;\n",
				_TMP_LABEL_CSTR(end)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(lhs_false)
			);

			_ExprResult result_rhs;
			_expr_result_init(&result_rhs);

			_ir_expr(ctx, rstr, &result_rhs, node_rhs);
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

			ResizableString rstr_cond_expr_rhs;
			rstr_init(&rstr_cond_expr_rhs);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_cond_expr_rhs, &result_rhs, node_rhs);

			rstr_appendf(
				rstr,
				"cbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_cond_expr_rhs),
				_TMP_LABEL_CSTR(rhs_true),
				_TMP_LABEL_CSTR(end)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(rhs_true)
			);

			rstr_appendf(
				rstr,
				"assign %s, %s;\n",
				RSTR_CSTR(&rstr_result),
				RSTR_CSTR(&rstr_one)
			);

			rstr_appendf(
				rstr,
				"%s:\n",
				_TMP_LABEL_CSTR(end)
			);

			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_result)
			);

			rstr_free(&rstr_zero);
			rstr_free(&rstr_one);
			rstr_free(&rstr_result);

			_expr_result_free(&result_lhs);
			rstr_free(&rstr_cond_expr_lhs);

			_TMP_LABEL_FREE(lhs_true)
			_TMP_LABEL_FREE(lhs_false)
			_TMP_LABEL_FREE(rhs_true)
			_TMP_LABEL_FREE(end)

			_expr_result_free(&result_rhs);
			rstr_free(&rstr_cond_expr_rhs);

			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_cond(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	if (node->type == FE_NODE_EXPR_COND) {
		assert(node->nchilds == 3);

		ParserASTNode *node_expr_cond = node->childs[0];
		ParserASTNode *node_expr_true = node->childs[1];
		ParserASTNode *node_expr_false = node->childs[2];

		ParserASTNode *node_target_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);

		_TMP_LABEL_DEF(ctx, expr_true)
		_TMP_LABEL_DEF(ctx, expr_false)
		_TMP_LABEL_DEF(ctx, end)

		ResizableString rstr_result;
		rstr_init(&rstr_result);
		_generate_func_tmp_var(
			ctx,
			ctx->func_symbol,
			&rstr_result,
			node_target_type
		);

		_ExprResult result_expr_cond;
		_expr_result_init(&result_expr_cond);

		_ir_expr(ctx, rstr, &result_expr_cond, node_expr_cond);
		rstr_append_with_rstr(rstr, &(result_expr_cond.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_expr_cond.rstr_for_result));

		ResizableString rstr_expr_cond;
		rstr_init(&rstr_expr_cond);
		_ir_cond_expr_with_rstr(ctx, rstr, &rstr_expr_cond, &result_expr_cond, node_expr_cond);

		// 根据条件表达式选择执行expr_true块还是expr_false块。
		rstr_appendf(
			rstr,
			"cbr %s, %s, %s;\n",
			RSTR_CSTR(&rstr_expr_cond),
			_TMP_LABEL_CSTR(expr_true),
			_TMP_LABEL_CSTR(expr_false)
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_CSTR(expr_true)
		);

		// expr_true块的代码。
		_ExprResult result_expr_true;
		_expr_result_init(&result_expr_true);

		_ir_expr(ctx, rstr, &result_expr_true, node_expr_true);
		rstr_append_with_rstr(rstr, &(result_expr_true.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_expr_true.rstr_for_result));

		_ExprResult result_expr_true_casted;
		_expr_result_init(&result_expr_true_casted);

		_expr_result_cast(
			ctx,
			&result_expr_true_casted,
			node_target_type,
			&result_expr_true,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_true)
		);
		rstr_append_with_rstr(rstr, &(result_expr_true_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_expr_true_casted.rstr_for_result));

		rstr_appendf(
			rstr,
			"assign %s, %s;\n",
			RSTR_CSTR(&rstr_result),
			RSTR_CSTR(&(result_expr_true_casted.rstr_result))
		);

		rstr_appendf(
			rstr,
			"br %s;\n",
			_TMP_LABEL_CSTR(end)
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_CSTR(expr_false)
		);

		// expr_false块的代码。
		_ExprResult result_expr_false;
		_expr_result_init(&result_expr_false);

		_ir_expr(ctx, rstr, &result_expr_false, node_expr_false);
		rstr_append_with_rstr(rstr, &(result_expr_false.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_expr_false.rstr_for_result));

		_ExprResult result_expr_false_casted;
		_expr_result_init(&result_expr_false_casted);

		_expr_result_cast(
			ctx,
			&result_expr_false_casted,
			node_target_type,
			&result_expr_false,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_false)
		);
		rstr_append_with_rstr(rstr, &(result_expr_false_casted.rstr_for_result_ptr));
		rstr_append_with_rstr(rstr, &(result_expr_false_casted.rstr_for_result));

		rstr_appendf(
			rstr,
			"assign %s, %s;\n",
			RSTR_CSTR(&rstr_result),
			RSTR_CSTR(&(result_expr_false_casted.rstr_result))
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_CSTR(end)
		);

		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_result)
		);

		_TMP_LABEL_FREE(expr_true)
		_TMP_LABEL_FREE(expr_false)
		_TMP_LABEL_FREE(end)

		rstr_free(&rstr_result);

		_expr_result_free(&result_expr_cond);
		_expr_result_free(&result_expr_true);
		_expr_result_free(&result_expr_true_casted);
		_expr_result_free(&result_expr_false);
		_expr_result_free(&result_expr_false_casted);

		rstr_free(&rstr_expr_cond);

		return true;
	} else {
		return false;
	}
}

static bool _ir_expr_assign(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	#define	_A(func)	\
		assert(node->nchilds == 2);	\
			\
		ResizableString rstr_result_lhs_ptr;	\
		rstr_init(&rstr_result_lhs_ptr);	\
			\
		_ExprResult result;	\
		_expr_result_init(&result);	\
			\
		(func)(ctx, rstr, &rstr_result_lhs_ptr, &result, node);	\
		rstr_append_with_rstr(rstr, &(result.rstr_for_result_ptr));	\
		rstr_append_with_rstr(rstr, &(result.rstr_for_result));	\
			\
		rstr_appendf(	\
			&(expr_result->rstr_for_result),	\
			"store %s, %s;\n",	\
			RSTR_CSTR(&rstr_result_lhs_ptr),	\
			RSTR_CSTR(&(result.rstr_result))	\
		);	\
		_expr_result_set_result(	\
			expr_result,	\
			RSTR_CSTR(&(result.rstr_result))	\
		);	\
			\
		rstr_free(&rstr_result_lhs_ptr);	\
			\
		_expr_result_free(&result);

	switch (node->type) {
		case FE_NODE_EXPR_ASSIGN: {
			assert(node->nchilds == 2);

			ParserASTNode *node_lhs = node->childs[0];
			ParserASTNode *node_rhs = node->childs[1];

			ParserASTNode *node_lhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
			ParserASTNode *ndoe_rhs_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_rhs);

			_ExprResult result_lhs;
			_ExprResult result_rhs;
			_ExprResult result_rhs_casted;

			_expr_result_init(&result_lhs);
			_expr_result_init(&result_rhs);
			_expr_result_init(&result_rhs_casted);

			_ir_expr(ctx, rstr, &result_lhs, node_lhs);
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));

			_ir_expr(ctx, rstr, &result_rhs, node_rhs);
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

			_expr_result_cast(
				ctx,
				&result_rhs_casted,
				node_lhs_type,
				&result_rhs,
				ndoe_rhs_type
			);
			rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs_casted.rstr_for_result));

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"store %s, %s;\n",
				RSTR_CSTR(&(result_lhs.rstr_result_ptr)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_rhs_casted.rstr_result))
			);

			_expr_result_free(&result_lhs);
			_expr_result_free(&result_rhs);
			_expr_result_free(&result_rhs_casted);

			return true;
		}
		case FE_NODE_EXPR_ADD_ASSIGN: {

			_A(_ir_expr_add)

			return true;
		}
		case FE_NODE_EXPR_SUB_ASSIGN: {

			_A(_ir_expr_sub)

			return true;
		}
		case FE_NODE_EXPR_MUL_ASSIGN: {

			_A(_ir_expr_mul)

			return true;
		}
		case FE_NODE_EXPR_DIV_ASSIGN: {
			
			_A(_ir_expr_div)
			
			return true;
		}
		case FE_NODE_EXPR_MOD_ASSIGN: {

			_A(_ir_expr_mod)
			
			return true;
		}
		case FE_NODE_EXPR_BAND_ASSIGN: {
			
			_A(_ir_expr_bitwise)

			return true;
		}
		case FE_NODE_EXPR_BOR_ASSIGN: {

			_A(_ir_expr_bitwise)
			
			return true;
		}
		case FE_NODE_EXPR_BXOR_ASSIGN: {

			_A(_ir_expr_bitwise)
			
			return true;
		}
		case FE_NODE_EXPR_SHIFT_LEFT_ASSIGN: {
			
			_A(_ir_expr_shift_left)

			return true;
		}
		case FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN: {
			
			_A(_ir_expr_shift_right)

			return true;
		}
		default: {
			return false;
		}
	}

	#undef	_A
}

static void _ir_expr(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	#define	_A(fn)	\
		if (fn(ctx, rstr, expr_result, node)) {	\
			return;	\
		}

	_A(_ir_expr_constexpr)
	_A(_ir_expr_identifier)
	_A(_ir_expr_member_arridx_fncall)
	_A(_ir_expr_unary)
	_A(_ir_expr_mul_div_mod)
	_A(_ir_expr_add_sub)
	_A(_ir_expr_shift)
	_A(_ir_expr_comparison)
	_A(_ir_expr_band_bxor_bor)
	_A(_ir_expr_logic)
	_A(_ir_expr_cond)
	_A(_ir_expr_assign)

	printf("TYPE: %x, %s\n", node->type, node->type_name);
	assert(0);

	#undef	_A
}

static void _ir_expr_wrapper(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);
	assert(node->type == FE_NODE_EXPR);
	assert(node->nchilds == 1);

	ParserASTNode *child = node->childs[0];
	_ir_expr(ctx, rstr, expr_result, child);
}

static void _ir_expr_wrapper_val(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	_ir_expr_wrapper(ctx, rstr, expr_result, node);

	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result));
}

static void _ir_expr_wrapper_ptr(
	IRGeneratorContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	_ir_expr_wrapper(ctx, rstr, expr_result, node);

	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result_ptr));
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

	_ir_constexpr(ctx, rstr, node_constexpr);
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
				ResizableString rstr_id;
				rstr_init(&rstr_id);
				_ir_identifier_name(ctx, &rstr_id, node_identifier);
				size_t no = FE_FUNC_SYMBOL_NEXT_NO(ctx->func_symbol);
				rstr_appendf(&rstr_id, ".%zu__", no);

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

				rstr_append_with_cstr(&rstr, ";\n");

				rstr_append_with_cstr(ctx->body, RSTR_CSTR(&rstr));

				rstr_free(&rstr);

				if (node_expr != NULL) {
					ResizableString rstr_expr;
					rstr_init(&rstr_expr);

					_ExprResult result;
					_expr_result_init(&result);

					_ExprResult casted_result;
					_expr_result_init(&casted_result);

					_ir_expr_wrapper_val(ctx, &rstr_expr, &result, node_expr);
					_expr_result_cast(
						ctx,
						&casted_result,
						node_type,
						&result,
						FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr)
					);
					rstr_append_with_rstr(&rstr_expr, &(casted_result.rstr_for_result_ptr));
					rstr_append_with_rstr(&rstr_expr, &(casted_result.rstr_for_result));

					rstr_appendf(
						&rstr_expr,
						"assign %s, %s;\n",
						RSTR_CSTR(&rstr_id),
						RSTR_CSTR(&(casted_result.rstr_result))
					);

					rstr_append_with_rstr(ctx->body, &rstr_expr);

					rstr_init(&rstr_expr);
					_expr_result_free(&result);
					_expr_result_free(&casted_result);
				}

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

static void _ir_stat_var(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	_ir_var(ctx, node);
}

static void _ir_stats_block(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stats_block
);

static void _ir_stat_if(
	IRGeneratorContext *ctx,
	ParserASTNode *node_if
) {
	assert(ctx);
	assert(node_if);
	assert(node_if->type == FE_NODE_STAT_IF);

	_TMP_LABEL_DEF(ctx, if_end)

	_TMP_LABEL_DEF(ctx, if_true)
	_TMP_LABEL_DEF(ctx, if_false)

	ParserASTNode *node_if_expr = node_if->childs[0];
	ParserASTNode *node_if_body = node_if->childs[1];

	_ExprResult result_if_expr;
	_expr_result_init(&result_if_expr);

	_ir_expr_wrapper_val(ctx, ctx->body, &result_if_expr, node_if_expr);

	ResizableString rstr_label_if_cond_expr;
	rstr_init(&rstr_label_if_cond_expr);
	_ir_cond_expr(ctx, &rstr_label_if_cond_expr, &result_if_expr, node_if_expr);
	rstr_appendf(
		ctx->body,
		"cbr %s, %s, %s;\n",
		RSTR_CSTR(&rstr_label_if_cond_expr),
		_TMP_LABEL_CSTR(if_true),
		_TMP_LABEL_CSTR(if_false)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(if_true)
	);
	_ir_stats_block(ctx, node_if_body);
	rstr_appendf(
		ctx->body,
		"br %s;\n",
		_TMP_LABEL_CSTR(if_end)
	);
	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(if_false)
	);

	for (int i = 2; i < node_if->nchilds; i++) {
		ParserASTNode *node = node_if->childs[i];
		if (node->type == FE_NODE_STAT_ELIF) {
			ParserASTNode *node_elif_expr = node->childs[0];
			ParserASTNode *node_elif_body = node->childs[1];

			_TMP_LABEL_DEF(ctx, elif_true)
			_TMP_LABEL_DEF(ctx, elif_false)

			_ExprResult result_elif_expr;
			_expr_result_init(&result_elif_expr);

			_ir_expr_wrapper_val(ctx, ctx->body, &result_elif_expr, node_elif_expr);

			ResizableString rstr_label_elif_cond_expr;
			rstr_init(&rstr_label_elif_cond_expr);
			_ir_cond_expr(ctx, &rstr_label_elif_cond_expr, &result_elif_expr, node_elif_expr);
			rstr_appendf(
				ctx->body,
				"cbr %s, %s, %s;\n",
				RSTR_CSTR(&rstr_label_elif_cond_expr),
				_TMP_LABEL_CSTR(elif_true),
				_TMP_LABEL_CSTR(elif_false)
			);

			rstr_appendf(
				ctx->body,
				"%s:\n",
				_TMP_LABEL_CSTR(elif_true)
			);
			_ir_stats_block(ctx, node_elif_body);
			rstr_appendf(
				ctx->body,
				"br %s;\n",
				_TMP_LABEL_CSTR(if_end)
			);
			rstr_appendf(
				ctx->body,
				"%s:\n",
				_TMP_LABEL_CSTR(elif_false)
			);

			_TMP_LABEL_FREE(elif_true)
			_TMP_LABEL_FREE(elif_false)
		} else if (node->type == FE_NODE_STAT_ELSE) {
			ParserASTNode *node_else_body = node->childs[0];
			_ir_stats_block(ctx, node_else_body);
		} else {
			assert(0);
		}
	}

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(if_end)
	);

	_TMP_LABEL_FREE(if_end)

	_TMP_LABEL_FREE(if_true)
	_TMP_LABEL_FREE(if_false)
	
	_expr_result_free(&result_if_expr);

	rstr_free(&rstr_label_if_cond_expr);
}

static void _ir_stat_while(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_while
) {
	assert(ctx);
	assert(node_stat_while);
	assert(node_stat_while->type == FE_NODE_STAT_WHILE);

	_TMP_LABEL_DEF(ctx, while_repeat)

	_TMP_LABEL_DEF(ctx, while_true)
	_TMP_LABEL_DEF(ctx, while_false)

	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node_stat_while),
		&_TMP_LABEL(while_repeat)
	);
	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_END(node_stat_while),
		&_TMP_LABEL(while_false)
	);

	ParserASTNode *node_while_expr = node_stat_while->childs[0];
	ParserASTNode *node_while_body = node_stat_while->childs[1];

	_ExprResult result_while_expr;
	_expr_result_init(&result_while_expr);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(while_repeat)
	);

	_ir_expr_wrapper_val(ctx, ctx->body, &result_while_expr, node_while_expr);

	ResizableString rstr_label_while_cond_expr;
	rstr_init(&rstr_label_while_cond_expr);
	_ir_cond_expr(ctx, &rstr_label_while_cond_expr, &result_while_expr, node_while_expr);

	rstr_appendf(
		ctx->body,
		"cbr %s, %s, %s;\n",
		RSTR_CSTR(&rstr_label_while_cond_expr),
		_TMP_LABEL_CSTR(while_true),
		_TMP_LABEL_CSTR(while_false)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(while_true)
	);

	_ir_stats_block(ctx, node_while_body);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		_TMP_LABEL_CSTR(while_repeat)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(while_false)
	);

	_TMP_LABEL_FREE(while_repeat)

	_TMP_LABEL_FREE(while_true)
	_TMP_LABEL_FREE(while_false)

	_expr_result_free(&result_while_expr);

	rstr_free(&rstr_label_while_cond_expr);
}

static void _ir_stat_do(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_do
) {
	assert(ctx);
	assert(node_stat_do);
	assert(node_stat_do->type == FE_NODE_STAT_DO);

	_TMP_LABEL_DEF(ctx, do_repeat)

	_TMP_LABEL_DEF(ctx, do_true)
	_TMP_LABEL_DEF(ctx, do_false)

	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node_stat_do),
		&_TMP_LABEL(do_repeat)
	);
	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_END(node_stat_do),
		&_TMP_LABEL(do_false)
	);

	ParserASTNode *node_do_body = node_stat_do->childs[0];
	ParserASTNode *node_do_expr = node_stat_do->childs[1];

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(do_true)
	);

	_ir_stats_block(ctx, node_do_body);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(do_repeat)
	);
	
	_ExprResult result_do_expr;
	_expr_result_init(&result_do_expr);

	_ir_expr_wrapper_val(ctx, ctx->body, &result_do_expr, node_do_expr);

	ResizableString rstr_label_do_cond_expr;
	rstr_init(&rstr_label_do_cond_expr);
	_ir_cond_expr(ctx, &rstr_label_do_cond_expr, &result_do_expr, node_do_expr);

	rstr_appendf(
		ctx->body,
		"cbr %s, %s, %s;\n",
		RSTR_CSTR(&rstr_label_do_cond_expr),
		_TMP_LABEL_CSTR(do_true),
		_TMP_LABEL_CSTR(do_false)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(do_false)
	);

	_TMP_LABEL_FREE(do_repeat)

	_TMP_LABEL_FREE(do_true)
	_TMP_LABEL_FREE(do_false)

	_expr_result_free(&result_do_expr);

	rstr_free(&rstr_label_do_cond_expr);
}

static void _ir_stat_for(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_for
) {
	assert(ctx);
	assert(node_stat_for);
	assert(node_stat_for->type == FE_NODE_STAT_FOR);
	assert(node_stat_for->nchilds == 4);

	ParserASTNode *node_for_init = node_stat_for->childs[0];
	assert(node_for_init->type == FE_NODE_STAT_FOR_INIT);
	assert(node_for_init->nchilds == 0
			|| node_for_init->nchilds == 1);

	ParserASTNode *node_for_expr_cond = node_stat_for->childs[1];
	assert(node_for_expr_cond->type == FE_NODE_STAT_FOR_EXPR_COND);
	assert(node_for_expr_cond->nchilds == 0
			|| node_for_expr_cond->nchilds == 1);

	ParserASTNode *node_for_expr = node_stat_for->childs[2];
	assert(node_for_expr->type == FE_NODE_STAT_FOR_EXPR);
	assert(node_for_expr->nchilds == 0
			|| node_for_expr->nchilds == 1);

	ParserASTNode *node_for_body = node_stat_for->childs[3];
	assert(node_for_body->type == FE_NODE_STATS_BLOCK);

	if (node_for_init->nchilds == 1) {
		ParserASTNode *node_for_init_var_def = node_for_init->childs[0];
		assert(node_for_init_var_def->type == FE_NODE_VAR);

		_ir_var(ctx, node_for_init_var_def);
	}

	_TMP_LABEL_DEF(ctx, for_repeat)
	_TMP_LABEL_DEF(ctx, for_skip_first_expr);

	_TMP_LABEL_DEF(ctx, for_true)
	_TMP_LABEL_DEF(ctx, for_false)

	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node_stat_for),
		&_TMP_LABEL(for_repeat)
	);
	rstr_append_with_rstr(
		FE_LOOP_AST_NODE_GET_LABEL_END(node_stat_for),
		&_TMP_LABEL(for_false)
	);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		_TMP_LABEL_CSTR(for_skip_first_expr)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(for_repeat)
	);

	if (node_for_expr->nchilds == 1) {
		ParserASTNode *node_expr = node_for_expr->childs[0];
		assert(node_expr->type == FE_NODE_EXPR);

		_ExprResult result_for_expr;
		_expr_result_init(&result_for_expr);

		_ir_expr_wrapper_val(ctx, ctx->body, &result_for_expr, node_expr);

		_expr_result_free(&result_for_expr);
	}

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(for_skip_first_expr)
	);

	if (node_for_expr_cond->nchilds == 1) {
		ParserASTNode *node_expr = node_for_expr_cond->childs[0];
		assert(node_expr->type == FE_NODE_EXPR);

		_ExprResult result_for_expr_cond;
		_expr_result_init(&result_for_expr_cond);

		_ir_expr_wrapper_val(ctx, ctx->body, &result_for_expr_cond, node_expr);

		ResizableString rstr_label_for_expr_cond;
		rstr_init(&rstr_label_for_expr_cond);
		_ir_cond_expr(ctx, &rstr_label_for_expr_cond, &result_for_expr_cond, node_expr);

		rstr_appendf(
			ctx->body,
			"cbr %s, %s, %s;\n",
			RSTR_CSTR(&rstr_label_for_expr_cond),
			_TMP_LABEL_CSTR(for_true),
			_TMP_LABEL_CSTR(for_false)
		);

		_expr_result_free(&result_for_expr_cond);
		rstr_free(&rstr_label_for_expr_cond);
	}

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(for_true)
	);

	_ir_stats_block(ctx, node_for_body);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		_TMP_LABEL_CSTR(for_repeat)
	);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		_TMP_LABEL_CSTR(for_false)
	);

	_TMP_LABEL_FREE(for_repeat)
	_TMP_LABEL_FREE(for_skip_first_expr)

	_TMP_LABEL_FREE(for_true)
	_TMP_LABEL_FREE(for_false)
}

static void _ir_stat_break(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_break
) {
	assert(ctx);
	assert(node_stat_break);
	assert(node_stat_break->type == FE_NODE_STAT_BREAK);
	assert(node_stat_break->nchilds == 0);

	ParserASTNode *node_loop = fe_sem_search_node_along_parent(
		node_stat_break->parent,
		FE_NODE_STAT_WHILE, FE_NODE_STAT_DO, FE_NODE_STAT_FOR, 0
	);
	assert(node_loop);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		RSTR_CSTR(FE_LOOP_AST_NODE_GET_LABEL_END(node_loop))
	);
}

static void _ir_stat_continue(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_continue
) {
	assert(ctx);
	assert(node_stat_continue);
	assert(node_stat_continue->type == FE_NODE_STAT_CONTINUE);
	assert(node_stat_continue->nchilds == 0);

	ParserASTNode *node_loop = fe_sem_search_node_along_parent(
		node_stat_continue->parent,
		FE_NODE_STAT_WHILE, FE_NODE_STAT_DO, FE_NODE_STAT_FOR, 0
	);
	assert(node_loop);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		RSTR_CSTR(FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node_loop))
	);
}

static void _ir_stat_return(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_return
) {
	assert(ctx);
	assert(node_stat_return);
	assert(node_stat_return->type == FE_NODE_STAT_RETURN);
	assert(node_stat_return->nchilds == 0 || node_stat_return->nchilds == 1);

	if (node_stat_return->nchilds == 0) {
		rstr_append_with_cstr(ctx->body, "return;\n");
	} else if (node_stat_return->nchilds == 1) {
		ParserSymbol *func_symbol = ctx->func_symbol;
		assert(func_symbol);
		ParserASTNode *node_func_return_type = FE_FUNC_SYMBOL_GET_RETURN_TYPE_NODE(func_symbol);
		assert(node_func_return_type);

		ParserASTNode *node_expr = node_stat_return->childs[0];

		_ExprResult expr_result;
		_expr_result_init(&expr_result);

		_ExprResult casted_result;
		_expr_result_init(&casted_result);

		_ir_expr_wrapper_val(ctx, ctx->body, &expr_result, node_expr);

		_expr_result_cast(
			ctx,
			&casted_result,
			node_func_return_type,
			&expr_result,
			FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr)
		);
		rstr_append_with_rstr(ctx->body, &(casted_result.rstr_for_result_ptr));
		rstr_append_with_rstr(ctx->body, &(casted_result.rstr_for_result));

		rstr_appendf(
			ctx->body,
			"return %s;\n",
			RSTR_CSTR(&(casted_result.rstr_result))
		);

		_expr_result_free(&expr_result);
		_expr_result_free(&casted_result);
	} else {
		assert(0);
	}
}

static void _ir_stat_goto(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_goto
) {
	assert(ctx);
	assert(node_stat_goto);
	assert(node_stat_goto->type == FE_NODE_STAT_GOTO);
	assert(node_stat_goto->nchilds == 1);

	ParserASTNode *node_id = node_stat_goto->childs[0];

	ResizableString rstr_label;
	rstr_init_with_raw(&rstr_label, node_id->token->content, node_id->token->len);

	rstr_appendf(
		ctx->body,
		"br %s;\n",
		RSTR_CSTR(&rstr_label)
	);

	rstr_free(&rstr_label);
}

static void _ir_stat_label(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_label
) {
	assert(ctx);
	assert(node_stat_label);
	assert(node_stat_label->type == FE_NODE_STAT_LABEL);
	assert(node_stat_label->nchilds == 1);

	ParserASTNode *node_id = node_stat_label->childs[0];

	ResizableString rstr_label;
	rstr_init_with_raw(&rstr_label, node_id->token->content, node_id->token->len);

	rstr_appendf(
		ctx->body,
		"%s:\n",
		RSTR_CSTR(&rstr_label)
	);

	rstr_free(&rstr_label);
}

static void _ir_stat_asm(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat_asm
) {
	assert(ctx);
	assert(node_stat_asm);
	assert(node_stat_asm->type == FE_NODE_STAT_ASM);
	assert(node_stat_asm->nchilds >= 1);

	ParserASTNode *node_asm_code = node_stat_asm->childs[0];
	assert(node_asm_code->type == FE_NODE_LITERAL_STRING);

	ResizableString rstr_set_reg;
	rstr_init(&rstr_set_reg);

	ResizableString rstr_get_reg;
	rstr_init(&rstr_get_reg);

	for (int i = 1; i < node_stat_asm->nchilds; i++) {
		ParserASTNode *node_asm_option = node_stat_asm->childs[i];
		assert(node_asm_option->type == FE_NODE_STAT_ASM_OPTION);
		assert(node_asm_option->nchilds == 2);

		ParserASTNode *node_expr = node_asm_option->childs[1];
		assert(node_expr->type == FE_NODE_EXPR);
		_ExprResult expr_result;
		_expr_result_init(&expr_result);
		_ir_expr_wrapper_val(ctx, ctx->body, &expr_result, node_expr);

		ParserASTNode *node_specifier = node_asm_option->childs[0];
		assert(node_specifier->type == FE_NODE_LITERAL_STRING);
		LexerToken *tk_specifier = node_specifier->token;
		if (tk_specifier->len >= 4) {
			if (strncmp("set#", tk_specifier->content + 1, 4) == 0) {
				ResizableString rstr_reg;
				rstr_init_with_raw(
					&rstr_reg,
					tk_specifier->content + 5,
					tk_specifier->len - 6
				);
				rstr_appendf(
					&rstr_set_reg,
					"asm_set_reg \"%s\", %s;\n",
					RSTR_CSTR(&rstr_reg),
					RSTR_CSTR(&(expr_result.rstr_result))
				);
				rstr_free(&rstr_reg);
			} else if (strncmp("get#", tk_specifier->content + 1, 4) == 0) {
				ResizableString rstr_reg;
				rstr_init_with_raw(
					&rstr_reg,
					tk_specifier->content + 5,
					tk_specifier->len - 6
				);
				rstr_appendf(
					&rstr_get_reg,
					"asm_get_reg %s, \"%s\";\n",
					RSTR_CSTR(&(expr_result.rstr_result)),
					RSTR_CSTR(&rstr_reg)
				);
				rstr_free(&rstr_reg);
			} else {
				assert(0);
			}
		} else {
			assert(0);
		}

		_expr_result_free(&expr_result);
	}

	rstr_append_with_rstr(
		ctx->body,
		&rstr_set_reg
	);

	ResizableString rstr_asm_code;
	rstr_init_with_raw(
		&rstr_asm_code,
		node_asm_code->token->content,
		node_asm_code->token->len
	);
	rstr_appendf(
		ctx->body,
		"asm %s;\n",
		RSTR_CSTR(&rstr_asm_code)
	);
	rstr_free(&rstr_asm_code);

	rstr_append_with_rstr(
		ctx->body,
		&rstr_get_reg
	);

	rstr_free(&rstr_set_reg);
	rstr_free(&rstr_get_reg);
}

static void _ir_stat_expr(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_EXPR);

	_ExprResult expr_result;
	_expr_result_init(&expr_result);

	ResizableString rstr;
	rstr_init(&rstr);

	_ir_expr_wrapper_val(ctx, &rstr, &expr_result, node);

	rstr_append_with_rstr(ctx->body, &rstr);

	_expr_result_free(&expr_result);
	rstr_free(&rstr);
}

static void _ir_stat_va_start(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_START);

	ParserASTNode *node_expr = node->childs[0];

	_ExprResult result_expr;
	_expr_result_init(&result_expr);

	_ir_expr_wrapper_ptr(ctx, ctx->body, &result_expr, node_expr);

	rstr_appendf(
		ctx->body,
		"__va_start %s;\n",
		RSTR_CSTR(&(result_expr.rstr_result_ptr))
	);

	_expr_result_free(&result_expr);
}

static void _ir_stat_va_end(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_END);

	ParserASTNode *node_expr = node->childs[0];

	_ExprResult result_expr;
	_expr_result_init(&result_expr);

	_ir_expr_wrapper_ptr(ctx, ctx->body, &result_expr, node_expr);

	rstr_appendf(
		ctx->body,
		"__va_end %s;\n",
		RSTR_CSTR(&(result_expr.rstr_result_ptr))
	);

	_expr_result_free(&result_expr);
}

static void _ir_stat_va_copy(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_COPY);

	ParserASTNode *node_target = node->childs[0];
	ParserASTNode *node_source = node->childs[1];

	_ExprResult result_target;
	_expr_result_init(&result_target);
	_ir_expr_wrapper_ptr(ctx, ctx->body, &result_target, node_target);

	_ExprResult result_source;
	_expr_result_init(&result_source);
	_ir_expr_wrapper_ptr(ctx, ctx->body, &result_source, node_source);

	rstr_appendf(
		ctx->body,
		"__va_copy %s, %s;\n",
		RSTR_CSTR(&(result_target.rstr_result_ptr)),
		RSTR_CSTR(&(result_source.rstr_result_ptr))
	);

	_expr_result_free(&result_target);
	_expr_result_free(&result_source);
}





static void _ir_stat(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stat
) {
	assert(ctx);
	assert(node_stat);

	switch (node_stat->type) {
		case FE_NODE_VAR: {
			_ir_stat_var(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_IF: {
			_ir_stat_if(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_WHILE: {
			_ir_stat_while(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_DO: {
			_ir_stat_do(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_FOR: {
			_ir_stat_for(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_BREAK: {
			_ir_stat_break(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_CONTINUE: {
			_ir_stat_continue(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_RETURN: {
			_ir_stat_return(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_GOTO: {
			_ir_stat_goto(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_LABEL: {
			_ir_stat_label(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_ASM: {
			_ir_stat_asm(ctx, node_stat);
			break;
		}
		case FE_NODE_EXPR: {
			_ir_stat_expr(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_DUMMY: {
			// 什么也不做。
			break;
		}

		case FE_NODE_STAT_VA_START: {
			_ir_stat_va_start(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_VA_END: {
			_ir_stat_va_end(ctx, node_stat);
			break;
		}
		case FE_NODE_STAT_VA_COPY: {
			_ir_stat_va_copy(ctx, node_stat);
			break;
		}
		

		case FE_NODE_STATS_BLOCK: {
			_ir_stats_block(ctx, node_stat);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _ir_stats_block(
	IRGeneratorContext *ctx,
	ParserASTNode *node_stats_block
) {
	assert(ctx);
	assert(node_stats_block);
	assert(node_stats_block->type == FE_NODE_STATS_BLOCK);

	for (int i = 0; i < node_stats_block->nchilds; i++) {
		ParserASTNode *node_stat = node_stats_block->childs[i];
		_ir_stat(ctx, node_stat);
	}
}

static void _ir_func(
	IRGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_FUNC);

	ctx->func_symbol = FE_FUNC_AST_NODE_GET_SYMBOL(node);
	assert(ctx->func_symbol);

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
	if (node_func_params->nchilds > 0) {
		assert(node_func_params->childs[0]->type == FE_NODE_FUNC_PARAMS_ITEM);
	}

	rstr_append_with_char(ctx->body, '\n');

	rstr_append_with_cstr(ctx->body, "function");

	rstr_append_with_char(ctx->body, ' ');

	ResizableString rstr_id;
	rstr_init(&rstr_id);
	_ir_identifier_name(ctx, &rstr_id, node_identifier);
	rstr_append_with_rstr(ctx->body, &rstr_id);
	rstr_free(&rstr_id);

	rstr_append_with_cstr(ctx->body, "(");
	_ir_func_type_params(ctx, ctx->body, node_func_params);
	rstr_append_with_cstr(ctx->body, ")");

	if (node_return_type != NULL) {
		rstr_append_with_char(ctx->body, ':');
		ResizableString rstr_return_type;
		rstr_init(&rstr_return_type);
		_ir_type(ctx, &rstr_return_type, node_return_type);
		rstr_append_with_rstr(ctx->body, &rstr_return_type);
		rstr_free(&rstr_return_type);
	}

	rstr_append_with_char(ctx->body, ' ');

	if (node_body->type == FE_NODE_STATS_BLOCK) {
		rstr_append_with_cstr(ctx->body, " {\n");

		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_param = node_func_params->childs[i];
			if (node_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				continue;
			}

			ParserSymbol *symbol = FE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_param);

			assert(!FE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol));

			FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);	
			ResizableString *code_gen_name = FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);
			rstr_init(code_gen_name);
			_ir_identifier_name_tk(ctx, code_gen_name, symbol->token);
		}

		ResizableString *body = ctx->body;
		ctx->body = rstr_new();
		ctx->local_var_defs = rstr_new();

		_ir_stats_block(ctx, node_body);

		rstr_append_with_rstr(body, ctx->local_var_defs);
		rstr_append_with_rstr(body, ctx->body);
	
		rstr_free(ctx->body);
		ctx->body = body;
		rstr_free(ctx->local_var_defs);
		ctx->local_var_defs = NULL;

		rstr_append_with_cstr(ctx->body, "}");
	} else if (node_body->type == FE_NODE_DUMMY) {
		rstr_append_with_cstr(ctx->body, "dummy");
	} else {
		assert(0);
	}

	rstr_append_with_cstr(ctx->body, "\n\n");
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
