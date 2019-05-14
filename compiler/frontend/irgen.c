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
#define	_TMP_LABEL_NAME_CSTR(name)	(RSTR_CSTR(&_TMP_LABEL(name)) + 1)

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

	if (fe_sem_is_integer_type(target_type)
			&& fe_sem_is_integer_type(source_type)
			&& fe_sem_get_primitive_type_size(target_type) == fe_sem_get_primitive_type_size(source_type)) {
		goto not_need_to_cast;
	}

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
			cmd" %s, %s\n",	\
			RSTR_CSTR(&rstr_tmp_label_val),	\
			RSTR_CSTR(&(source_result->rstr_result))	\
		);	\
		_expr_result_set_result(	\
			target_result,	\
			RSTR_CSTR(&rstr_tmp_label_val)	\
		);

	if (fe_sem_is_signed_type(source_type)) {
		if (fe_sem_is_signed_type(target_type)) {
			if (fe_sem_get_primitive_type_size(target_type) < fe_sem_get_primitive_type_size(source_type)) {
				// 小的有符号整数 <- 大的有符号整数。
				_A("trunc")
			} else {
				// 大的有符号整数 <- 小的有符号整数。
				_A("sext")
			}
		} else if (fe_sem_is_unsigned_type(target_type)) {
			if (fe_sem_get_primitive_type_size(target_type) < fe_sem_get_primitive_type_size(source_type)) {
				// 小的无符号整数 <- 大的有符号整数。
				_A("trunc")
			} else {
				// 大的无符号整数 <- 小的有符号整数。
				_A("sext")
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
			if (fe_sem_get_primitive_type_size(target_type) < fe_sem_get_primitive_type_size(source_type)) {
				// 小的有符号整数 <- 大的无符号整数。
				_A("trunc")
			} else {
				// 大的有符号整数 <- 小的无符号整数。
				_A("zext")
			}
		} else if (fe_sem_is_unsigned_type(target_type)) {
			if (fe_sem_get_primitive_type_size(target_type) < fe_sem_get_primitive_type_size(source_type)) {
				// 小的无符号整数 <- 大的无符号整数。
				_A("trunc")
			} else {
				// 大的无符号整数 <- 小的无符号整数。
				_A("zext")
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
		default: {
			return false;
		}
	}
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
