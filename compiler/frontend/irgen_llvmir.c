#include "irgen_llvmir.h"

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

#define	_LLVM_TYPE_INTxx(ctx)	\
	((ctx)->psrctx->arch == FE_ARCH_32 ? "i32" : "i64")

static const char * _primitive_llvm_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR: {
			return "i8";
		}
		case FE_TYPE_INT8: {
			return "i8";
		}
		case FE_TYPE_UINT8: {
			return "i8";
		}
		case FE_TYPE_INT16: {
			return "i16";
		}
		case FE_TYPE_UINT16: {
			return "i16";
		}
		case FE_TYPE_INT32: {
			return "i32";
		}
		case FE_TYPE_UINT32: {
			return "i32";
		}
		case FE_TYPE_INT64: {
			return "i64";
		}
		case FE_TYPE_UINT64: {
			return "i64";
		}
		case FE_TYPE_FLOAT: {
			return "float";
		}
		case FE_TYPE_DOUBLE: {
			return "double";
		}
		default: {
			assert(0);
			return "Oop";
		}
	}
}

typedef struct {
	ResizableString rstr_for_result;
	ResizableString rstr_for_result_ptr;

	ResizableString rstr_result;
	ResizableString rstr_result_type;
	ResizableString rstr_result_val;

	ResizableString rstr_result_ptr;
	ResizableString rstr_result_ptr_type;
	ResizableString rstr_result_ptr_val;
} _ExprResult;

static void _expr_result_init(
	_ExprResult *expr_result
) {
	assert(expr_result);

	rstr_init(&(expr_result->rstr_for_result));
	rstr_init(&(expr_result->rstr_for_result_ptr));

	rstr_init(&(expr_result->rstr_result));
	rstr_init(&(expr_result->rstr_result_type));
	rstr_init(&(expr_result->rstr_result_val));

	rstr_init(&(expr_result->rstr_result_ptr));
	rstr_init(&(expr_result->rstr_result_ptr_type));
	rstr_init(&(expr_result->rstr_result_ptr_val));
}

static void _expr_result_set_result(
	_ExprResult *result,
	const char *type,
	const char *val
) {
	assert(result);
	assert(type);
	assert(val);

	rstr_appendf(
		&(result->rstr_result),
		"%s %s",
		type,
		val
	);

	rstr_append_with_cstr(&(result->rstr_result_type), type);

	rstr_append_with_cstr(&(result->rstr_result_val), val);
}

static void _expr_result_set_func_result(
	_ExprResult *result,
	const char *type,
	const char *val
) {
	assert(result);
	assert(type);
	assert(val);

	rstr_appendf(
		&(result->rstr_result),
		"%s* %s",
		type,
		val
	);

	rstr_appendf(
		&(result->rstr_result_type),
		"%s*",
		type
	);

	rstr_append_with_cstr(&(result->rstr_result_val), val);
}

static void _expr_result_set_result_ptr(
	_ExprResult *result,
	const char *type,
	const char *val
) {
	assert(result);
	assert(type);
	assert(val);

	rstr_appendf(
		&(result->rstr_result_ptr),
		"%s* %s",
		type,
		val
	);

	rstr_appendf(
		&(result->rstr_result_ptr_type),
		"%s*",
		type
	);

	rstr_append_with_cstr(&(result->rstr_result_ptr_val), val);
}

static void _expr_result_set_result_ptr_raw(
	_ExprResult *result,
	const char *type,
	const char *val
) {
	assert(result);
	assert(type);
	assert(val);

	rstr_appendf(
		&(result->rstr_result_ptr),
		"%s %s",
		type,
		val
	);

	rstr_append_with_cstr(&(result->rstr_result_ptr_type), type);

	rstr_append_with_cstr(&(result->rstr_result_ptr_val), val);
}

static void _expr_result_free(
	_ExprResult *expr_result
) {
	assert(expr_result);

	rstr_free(&(expr_result->rstr_for_result));
	rstr_free(&(expr_result->rstr_for_result_ptr));

	rstr_free(&(expr_result->rstr_result));
	rstr_free(&(expr_result->rstr_result_type));
	rstr_free(&(expr_result->rstr_result_val));

	rstr_free(&(expr_result->rstr_result_ptr));
	rstr_free(&(expr_result->rstr_result_ptr_type));
	rstr_free(&(expr_result->rstr_result_ptr_val));
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




static void _error(
	const char * msg
) {
	assert(msg);
	fprintf(stderr, "IRGen(LLVMIR) Error: %s\n", msg);
	exit(1);
}




static size_t _next_no(
	IRGeneratorLlvmirContext *ctx
) {
	assert(ctx);

	return ctx->counter++;
}

static size_t _generate_func_tmp(
	IRGeneratorLlvmirContext *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);

	size_t no = FE_FUNC_SYMBOL_NEXT_NO(func_symbol);

	rstr_appendf(rstr, "%%.__tmp_%zu__", no);

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






static int _new_string(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *name,
	const char *str,
	size_t len
) {
	assert(ctx);
	assert(str);

	// @.str.1 = private unnamed_addr constant [9 x i8] c"Person02\00", align 1

	size_t no = _next_no(ctx);

	if (name != NULL) {
		rstr_appendf(name, "@.str.%zu", no);
	}
	
	ResizableString unescaped_str;
	rstr_init(&unescaped_str);

	fe_lexer_unescape_string(
		ctx->psrctx->lexctx,
		&unescaped_str,
		str, len
	);
	int unescaped_str_len = RSTR_LEN(&unescaped_str);

	rstr_appendf(
		ctx->head,
		"@.str.%zu = private unnamed_addr constant [%d x i8] c\"",
		no, unescaped_str_len + 1
	);

	for (int i = 0; i < unescaped_str_len; i++) {
		char chr = RSTR_CHAR(&unescaped_str, i);
		if (isprint(chr)) {
			rstr_append_with_char(ctx->head, chr);
		} else {
			rstr_appendf(ctx->head, "\\%02X", chr);
		}
	}

	rstr_append_with_cstr(ctx->head, "\\00\", align 1\n");

	rstr_free(&unescaped_str);

	return unescaped_str_len + 1;
}




static void _ir_type(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
);

static void _ir_expr(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
);

static void _ir_expr_wrapper(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
);

static void _ir_expr_wrapper_val(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
);

static void _ir_expr_wrapper_ptr(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
);




static void _expr_result_cast(
	IRGeneratorLlvmirContext *ctx,
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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	ResizableString rstr_target_type;
	rstr_init(&rstr_target_type);
	_ir_type(ctx, &rstr_target_type, target_type_node);

	#define	_A(cmd)	\
		rstr_appendf(	\
			&(target_result->rstr_for_result),	\
			"%s = "cmd" %s to %s\n",	\
			RSTR_CSTR(&rstr_tmp_label_val),	\
			RSTR_CSTR(&(source_result->rstr_result)),	\
			RSTR_CSTR(&rstr_target_type)	\
		);	\
		_expr_result_set_result(	\
			target_result,	\
			RSTR_CSTR(&rstr_target_type),	\
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
	_expr_result_set_result(
		target_result,
		RSTR_CSTR(&(source_result->rstr_result_type)),
		RSTR_CSTR(&(source_result->rstr_result_val))
	);
	_expr_result_set_result_ptr(
		target_result,
		RSTR_CSTR(&(source_result->rstr_result_ptr_type)),
		RSTR_CSTR(&(source_result->rstr_result_ptr_val))
	);

	#undef	_A
}




static void _ir_func_type_params(
	IRGeneratorLlvmirContext *ctx,
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
					ParserASTNode *node_t = node_param->childs[1];	\
						\
					ResizableString rstr_tmp;	\
					rstr_init(&rstr_tmp);	\
					_ir_type(ctx, &rstr_tmp, node_t);	\
					rstr_append_with_rstr(rstr, &rstr_tmp);	\
					rstr_free(&rstr_tmp);	\
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
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR: {
			rstr_append_with_cstr(rstr, "i8");
			break;
		}
		case FE_NODE_TYPE_INT8: {
			rstr_append_with_cstr(rstr, "i8");
			break;
		}
		case FE_NODE_TYPE_UINT8: {
			rstr_append_with_cstr(rstr, "i8");
			break;
		}
		case FE_NODE_TYPE_INT16: {
			rstr_append_with_cstr(rstr, "i16");
			break;
		}
		case FE_NODE_TYPE_UINT16: {
			rstr_append_with_cstr(rstr, "i16");
			break;
		}
		case FE_NODE_TYPE_INT32: {
			rstr_append_with_cstr(rstr, "i32");
			break;
		}
		case FE_NODE_TYPE_UINT32: {
			rstr_append_with_cstr(rstr, "i32");
			break;
		}
		case FE_NODE_TYPE_INT64: {
			rstr_append_with_cstr(rstr, "i64");
			break;
		}
		case FE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "i64");
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
		case FE_NODE_TYPE_STRUCT: {
			assert(node_type->nchilds == 1);

			ParserASTNode *node_id = node_type->childs[0];
			assert(node_id->type == FE_NODE_IDENTIFIER);

			rstr_append_with_cstr(rstr, "%struct.");
			rstr_append_with_raw(rstr, node_id->token->content, node_id->token->len);
			break;
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_dims = node_type->childs[0];
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);
			assert(node_array_dims->nchilds > 0);

			ParserASTNode *node_t = node_type->childs[1];

			_ir_type(ctx, rstr, node_t);

			for (int i = node_array_dims->nchilds - 1; i >= 0; i--) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == FE_NODE_LITERAL_UINT);

				rstr_prepend_with_cstr(rstr, " x ");
				rstr_prepend_with_raw(rstr, node_array_dim->token->content, node_array_dim->token->len);
				rstr_prepend_with_cstr(rstr, "[");

				rstr_append_with_cstr(rstr, "]");
			}

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

			if (node_t != NULL) {
				_ir_type(ctx, rstr, node_t);
			} else {
				rstr_append_with_cstr(rstr, "void");
			}

			rstr_append_with_cstr(rstr, " (");
			_ir_func_type_params(ctx, rstr, node_params);
			rstr_append_with_cstr(rstr, ")");
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			assert(node_type->nchilds == 1);
			ParserASTNode *node_t = node_type->childs[0];

			_ir_type(ctx, rstr, node_t);

			rstr_append_with_cstr(rstr, "*");
			break;
		}
		case FE_NODE_TYPE_VOID: {
			// LLVM IR不允许有void指针类型，需要把void指针类型转换为i8指针类型。
			// 但是在方法的返回值类型的地方，是允许为void类型。
			if (node_type->parent != NULL
					&& node_type->parent->type == FE_NODE_TYPE_POINTER) {
				rstr_prepend_with_cstr(rstr, "i8");
			} else {
				rstr_prepend_with_cstr(rstr, "void");
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}




static void _ir_zero_initializer(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR:
		case FE_NODE_TYPE_INT8:
		case FE_NODE_TYPE_UINT8:
		case FE_NODE_TYPE_INT16:
		case FE_NODE_TYPE_UINT16:
		case FE_NODE_TYPE_INT32:
		case FE_NODE_TYPE_UINT32:
		case FE_NODE_TYPE_INT64:
		case FE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		case FE_NODE_TYPE_FLOAT:
		case FE_NODE_TYPE_DOUBLE: {
			rstr_append_with_cstr(rstr, "0.0");
			break;
		}
		case FE_NODE_TYPE_STRUCT:
		case FE_NODE_TYPE_ARRAY: {
			rstr_append_with_cstr(rstr, "zeroinitializer");
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			rstr_append_with_cstr(rstr, "null");
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _ir_constexpr(
	IRGeneratorLlvmirContext *ctx,
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
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%lld",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%llu",
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_FLOAT: {
			char str[128];
			double val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_DOUBLE: {
			char str[128];
			double val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case FE_TYPE_POINTER: {
			if (FE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_constexpr)) {
				FeParserConstexprString *conststr = FE_EXPR_AST_NODE_GET_CONSTSTR(node_constexpr);
				uint64_t ptr_val = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);

				ResizableString conststr_name;
				rstr_init(&conststr_name);

				int len = _new_string(ctx, &conststr_name, conststr->str, conststr->len);

				ResizableString target_type;
				rstr_init(&target_type);

				_ir_type(ctx, &target_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_constexpr));

				char *offset_type = "";
				if (ctx->psrctx->arch == FE_ARCH_32) {
					offset_type = "i32";
				} else if (ctx->psrctx->arch == FE_ARCH_64) {
					offset_type = "i64";
				} else {
					assert(0);
				}

				if (ptr_val == 0) {
					rstr_appendf(
						rstr_val,
						"bitcast (i8* getelementptr inbounds ([%zu x i8], [%zu x i8]* %s, i32 0, %s 0) to %s)",
						len,
						len,
						RSTR_CSTR(&conststr_name),
						offset_type,
						RSTR_CSTR(&target_type)
					);
				} else {
					rstr_appendf(
						rstr_val,
						"bitcast (i8* getelementptr inbounds ([%zu x i8], [%zu x i8]* %s, i32 0, %s %u) to %s)",
						len,
						len,
						RSTR_CSTR(&conststr_name),
						offset_type,
						ptr_val,
						RSTR_CSTR(&target_type)
					);
				}

				rstr_free(&conststr_name);
				rstr_free(&target_type);
			} else {
				char *source_type = "";
				if (ctx->psrctx->arch == FE_ARCH_32) {
					source_type = "i32";
				} else if (ctx->psrctx->arch == FE_ARCH_64) {
					source_type = "i64";
				} else {
					assert(0);
				}

				ResizableString target_type;
				rstr_init(&target_type);
				_ir_type(ctx, &target_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_constexpr));

				char str[128];
				snprintf(
					str, sizeof(str), "inttoptr (%s %llu to %s)",
					source_type,
					FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr),
					RSTR_CSTR(&target_type)
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
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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
				// @c = common global i8 zeroinitializer, align 1
				ResizableString rstr;
				rstr_init(&rstr);

				rstr_append_with_cstr(&rstr, "@");
				rstr_append_with_raw(&rstr, node_identifier->token->content, node_identifier->token->len);
				
				if (node_expr == NULL) {
					rstr_append_with_cstr(&rstr, " = common global ");
				} else {
					rstr_append_with_cstr(&rstr, " = global ");
				}

				ResizableString rstr_type;
				rstr_init(&rstr_type);
				_ir_type(ctx, &rstr_type, node_type);
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_type));
				rstr_free(&rstr_type);

				rstr_append_with_cstr(&rstr, " ");

				if (node_expr == NULL) {
					_ir_zero_initializer(ctx, &rstr, node_type);
				} else {
					_ir_constexpr_initializer(ctx, &rstr, node_expr);
				}

				if (align > 0) {
					rstr_appendf(&rstr, ", align %d", align);
				}

				rstr_append_with_cstr(ctx->body, RSTR_CSTR(&rstr));

				rstr_free(&rstr);

				// 更新全局变量符号在代码生成时的名字。
				ParserSymbol *symbol = FE_VAR_ITEM_AST_NODE_GET_SYMBOL(node_var_item);
				FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);
				rstr_init(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol));
				rstr_append_with_char(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol), '@');
				rstr_append_with_raw(
					FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol),
					node_identifier->token->content,
					node_identifier->token->len
				);

				rstr_append_with_cstr(ctx->body, "\n");
				break;
			}
			case FE_VAR_TYPE_LOCAL: {
				// %3 = alloca i32, align 4
				ResizableString rstr;
				rstr_init(&rstr);

				size_t no = FE_FUNC_SYMBOL_NEXT_NO(ctx->func_symbol);
				ResizableString rstr_local_name;
				rstr_init_with_raw(
					&rstr_local_name,
					node_identifier->token->content,
					node_identifier->token->len
				);
				rstr_appendf(
					&rstr_local_name,
					"_%zu",
					no
				);

				rstr_append_with_cstr(&rstr, "%");
				rstr_append_with_rstr(&rstr, &rstr_local_name);
				rstr_append_with_cstr(&rstr, " = alloca ");

				ResizableString rstr_type;
				rstr_init(&rstr_type);
				_ir_type(ctx, &rstr_type, node_type);
				rstr_append_with_cstr(&rstr, RSTR_CSTR(&rstr_type));

				if (align > 0) {
					rstr_appendf(&rstr, ", align %d", align);
				}

				rstr_append_with_cstr(&rstr, "\n");

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
						"store %s, %s* %%",
						RSTR_CSTR(&(casted_result.rstr_result)),
						RSTR_CSTR(&rstr_type)
					);
					rstr_append_with_rstr(
						&rstr_expr,
						&rstr_local_name	
					);
					rstr_append_with_cstr(&rstr_expr, "\n");

					rstr_append_with_rstr(ctx->body, &rstr_expr);

					rstr_init(&rstr_expr);
					_expr_result_free(&result);
					_expr_result_free(&casted_result);
				}

				assert(ctx->local_var_defs);
				rstr_append_with_cstr(ctx->local_var_defs, RSTR_CSTR(&rstr));

				// 更新局部变量符号在代码生成时的名字。
				ParserSymbol *symbol = FE_VAR_ITEM_AST_NODE_GET_SYMBOL(node_var_item);
				FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);
				rstr_init(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol));
				rstr_append_with_char(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol), '%');
				rstr_append_with_rstr(
					FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol),
					&rstr_local_name
				);

				rstr_free(&rstr_type);
				rstr_free(&rstr);
				rstr_free(&rstr_local_name);
				break;
			}
			case FE_VAR_TYPE_STRUCT_MEMBER: {
				assert(0);
				break;
			}
			default: {
				assert(0);
			}
		}
	}
}

static void _ir_struct(IRGeneratorLlvmirContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);
	assert(node->nchilds == 2);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == FE_NODE_IDENTIFIER);
	ParserASTNode *node_struct_body = node->childs[1];
	if (node_struct_body->type == FE_NODE_DUMMY) {
		return;
	}
	assert(node_struct_body->type == FE_NODE_STRUCT_BODY);

	rstr_append_with_cstr(ctx->body, "%struct.");
	rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
	if (FE_STRUCT_AST_NODE_GET_PACKED(node)) {
		rstr_append_with_cstr(ctx->body, " = type <{");
	} else {
		rstr_append_with_cstr(ctx->body, " = type {");
	}

	for (int i = 0; i < node_struct_body->nchilds; i++) {
		ParserASTNode *node_var = node_struct_body->childs[i];
		assert(node_var->type == FE_NODE_VAR);

		for (int j = 0; j < node_var->nchilds; j++) {
			ParserASTNode *node_var_item = node_var->childs[j];
			assert(node_var_item->type == FE_NODE_VAR_ITEM);
			assert(node_var_item->nchilds >= 2);

			ParserASTNode *node_type = node_var_item->childs[1];

			ResizableString rstr_type;
			rstr_init(&rstr_type);

			_ir_type(ctx, &rstr_type, node_type);
			rstr_append_with_rstr(ctx->body, &rstr_type);

			rstr_free(&rstr_type);

			if (i + 1 < node_struct_body->nchilds
					|| j + 1 < node_var->nchilds) {
				rstr_append_with_cstr(ctx->body, ", ");
			}
		}
	}
	
	if (FE_STRUCT_AST_NODE_GET_PACKED(node)) {
		rstr_append_with_cstr(ctx->body, "}>\n");
	} else {
		rstr_append_with_cstr(ctx->body, "}\n");
	}
}




static void _ir_func_default_return_value(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR:
		case FE_NODE_TYPE_INT8:
		case FE_NODE_TYPE_UINT8:
		case FE_NODE_TYPE_INT16:
		case FE_NODE_TYPE_UINT16:
		case FE_NODE_TYPE_INT32:
		case FE_NODE_TYPE_UINT32:
		case FE_NODE_TYPE_INT64:
		case FE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		case FE_NODE_TYPE_FLOAT:
		case FE_NODE_TYPE_DOUBLE: {
			rstr_append_with_cstr(rstr, "0.0");
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			rstr_append_with_cstr(rstr, "null");
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}




static void _ir_stat_var(
	IRGeneratorLlvmirContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	_ir_var(ctx, node);
}

static void _ir_cond_expr_with_rstr(
	IRGeneratorLlvmirContext *ctx,
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

	uint8_t expr_type = FE_EXPR_AST_NODE_GET_TYPE(expr_node);

	_TMP_LABEL_DEF(ctx, cond_expr)

	if (fe_sem_is_integer_type(expr_type)) {
		rstr_appendf(
			rstr,
			"%s = icmp ne %s, 0\n",
			_TMP_LABEL_CSTR(cond_expr),
			RSTR_CSTR(&(expr_result->rstr_result))
		);
	} else if (fe_sem_is_pointer_type(expr_type)) {
		rstr_appendf(
			rstr,
			"%s = icmp ne %s, null\n",
			_TMP_LABEL_CSTR(cond_expr),
			RSTR_CSTR(&(expr_result->rstr_result))
		);
	} else {
		assert(0);
	}

	rstr_append_with_cstr(cond_expr_label, _TMP_LABEL_CSTR(cond_expr));

	_TMP_LABEL_FREE(cond_expr)
}

static void _ir_cond_expr(
	IRGeneratorLlvmirContext *ctx,
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

static void _ir_stats_block(
	IRGeneratorLlvmirContext *ctx,
	ParserASTNode *node_stats_block
);

static void _ir_stat_if(
	IRGeneratorLlvmirContext *ctx,
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
		"br i1 %s, label %s, label %s\n",
		RSTR_CSTR(&rstr_label_if_cond_expr),
		_TMP_LABEL_CSTR(if_true),
		_TMP_LABEL_CSTR(if_false)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(if_true),
		_TMP_LABEL_NAME_CSTR(if_true)
	);
	_ir_stats_block(ctx, node_if_body);
	rstr_appendf(
		ctx->body,
		"br label %s\n",
		_TMP_LABEL_CSTR(if_end)
	);
	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(if_false),
		_TMP_LABEL_NAME_CSTR(if_false)
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
				"br i1 %s, label %s, label %s\n",
				RSTR_CSTR(&rstr_label_elif_cond_expr),
				_TMP_LABEL_CSTR(elif_true),
				_TMP_LABEL_CSTR(elif_false)
			);

			rstr_appendf(
				ctx->body,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(elif_true),
				_TMP_LABEL_NAME_CSTR(elif_true)
			);
			_ir_stats_block(ctx, node_elif_body);
			rstr_appendf(
				ctx->body,
				"br label %s\n",
				_TMP_LABEL_CSTR(if_end)
			);
			rstr_appendf(
				ctx->body,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(elif_false),
				_TMP_LABEL_NAME_CSTR(elif_false)
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
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(if_end),
		_TMP_LABEL_NAME_CSTR(if_end)
	);

	_TMP_LABEL_FREE(if_end)

	_TMP_LABEL_FREE(if_true)
	_TMP_LABEL_FREE(if_false)
	
	_expr_result_free(&result_if_expr);

	rstr_free(&rstr_label_if_cond_expr);
}

static void _ir_stat_while(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(while_repeat),
		_TMP_LABEL_NAME_CSTR(while_repeat)
	);

	_ir_expr_wrapper_val(ctx, ctx->body, &result_while_expr, node_while_expr);

	ResizableString rstr_label_while_cond_expr;
	rstr_init(&rstr_label_while_cond_expr);
	_ir_cond_expr(ctx, &rstr_label_while_cond_expr, &result_while_expr, node_while_expr);

	rstr_appendf(
		ctx->body,
		"br i1 %s, label %s, label %s\n",
		RSTR_CSTR(&rstr_label_while_cond_expr),
		_TMP_LABEL_CSTR(while_true),
		_TMP_LABEL_CSTR(while_false)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(while_true),
		_TMP_LABEL_NAME_CSTR(while_true)
	);

	_ir_stats_block(ctx, node_while_body);

	rstr_appendf(
		ctx->body,
		"br label %s\n",
		_TMP_LABEL_CSTR(while_repeat)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(while_false),
		_TMP_LABEL_NAME_CSTR(while_false)
	);

	_TMP_LABEL_FREE(while_repeat)

	_TMP_LABEL_FREE(while_true)
	_TMP_LABEL_FREE(while_false)

	_expr_result_free(&result_while_expr);

	rstr_free(&rstr_label_while_cond_expr);
}

static void _ir_stat_do(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(do_true),
		_TMP_LABEL_NAME_CSTR(do_true)
	);

	_ir_stats_block(ctx, node_do_body);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(do_repeat),
		_TMP_LABEL_NAME_CSTR(do_repeat)
	);
	
	_ExprResult result_do_expr;
	_expr_result_init(&result_do_expr);

	_ir_expr_wrapper_val(ctx, ctx->body, &result_do_expr, node_do_expr);

	ResizableString rstr_label_do_cond_expr;
	rstr_init(&rstr_label_do_cond_expr);
	_ir_cond_expr(ctx, &rstr_label_do_cond_expr, &result_do_expr, node_do_expr);

	rstr_appendf(
		ctx->body,
		"br i1 %s, label %s, label %s\n",
		RSTR_CSTR(&rstr_label_do_cond_expr),
		_TMP_LABEL_CSTR(do_true),
		_TMP_LABEL_CSTR(do_false)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(do_false),
		_TMP_LABEL_NAME_CSTR(do_false)
	);

	_TMP_LABEL_FREE(do_repeat)

	_TMP_LABEL_FREE(do_true)
	_TMP_LABEL_FREE(do_false)

	_expr_result_free(&result_do_expr);

	rstr_free(&rstr_label_do_cond_expr);
}

static void _ir_stat_for(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %s\n",
		_TMP_LABEL_CSTR(for_skip_first_expr)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(for_repeat),
		_TMP_LABEL_NAME_CSTR(for_repeat)
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
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(for_skip_first_expr),
		_TMP_LABEL_NAME_CSTR(for_skip_first_expr)
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
			"br i1 %s, label %s, label %s\n",
			RSTR_CSTR(&rstr_label_for_expr_cond),
			_TMP_LABEL_CSTR(for_true),
			_TMP_LABEL_CSTR(for_false)
		);

		_expr_result_free(&result_for_expr_cond);
		rstr_free(&rstr_label_for_expr_cond);
	}

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(for_true),
		_TMP_LABEL_NAME_CSTR(for_true)
	);

	_ir_stats_block(ctx, node_for_body);

	rstr_appendf(
		ctx->body,
		"br label %s\n",
		_TMP_LABEL_CSTR(for_repeat)
	);

	rstr_appendf(
		ctx->body,
		"br label %s\n%s:\n",
		_TMP_LABEL_CSTR(for_false),
		_TMP_LABEL_NAME_CSTR(for_false)
	);

	_TMP_LABEL_FREE(for_repeat)
	_TMP_LABEL_FREE(for_skip_first_expr)

	_TMP_LABEL_FREE(for_true)
	_TMP_LABEL_FREE(for_false)
}

static void _ir_stat_break(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %s\n",
		RSTR_CSTR(FE_LOOP_AST_NODE_GET_LABEL_END(node_loop))
	);
}

static void _ir_stat_continue(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %s\n",
		RSTR_CSTR(FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node_loop))
	);
}

static void _ir_stat_return(
	IRGeneratorLlvmirContext *ctx,
	ParserASTNode *node_stat_return
) {
	assert(ctx);
	assert(node_stat_return);
	assert(node_stat_return->type == FE_NODE_STAT_RETURN);
	assert(node_stat_return->nchilds == 0 || node_stat_return->nchilds == 1);

	if (node_stat_return->nchilds == 0) {
		rstr_append_with_cstr(ctx->body, "ret void\n");
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
			"ret %s\n",
			RSTR_CSTR(&(casted_result.rstr_result))
		);

		_expr_result_free(&expr_result);
		_expr_result_free(&casted_result);
	} else {
		assert(0);
	}
}

static void _ir_stat_goto(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %%%s\n",
		RSTR_CSTR(&rstr_label)
	);

	rstr_free(&rstr_label);
}

static void _ir_stat_label(
	IRGeneratorLlvmirContext *ctx,
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
		"br label %%%s\n%s:\n",
		RSTR_CSTR(&rstr_label),
		RSTR_CSTR(&rstr_label)
	);

	rstr_free(&rstr_label);
}

static void _ir_stat_asm(
	IRGeneratorLlvmirContext *ctx,
	ParserASTNode *node_stat_asm
) {
	assert(ctx);
	assert(node_stat_asm);
	assert(node_stat_asm->type == FE_NODE_STAT_ASM);

	_error("asm is not supported.");
}




static bool _ir_expr_constexpr(
	IRGeneratorLlvmirContext *ctx,
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

	ResizableString rstr_type;
	ResizableString rstr_val;
	rstr_init(&rstr_type);
	rstr_init(&rstr_val);

	_ir_constexpr(ctx, &rstr_type, &rstr_val, node);

	_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_val));

	rstr_free(&rstr_type);
	rstr_free(&rstr_val);

	return true;
}

static bool _ir_expr_identifier(
	IRGeneratorLlvmirContext *ctx,
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

		ResizableString rstr_tmp_label;
		rstr_init(&rstr_tmp_label);
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label);

		ParserASTNode *node_type = FE_VAR_SYMBOL_GET_TYPE_NODE(symbol);
		ResizableString rstr_type;
		rstr_init(&rstr_type);
		_ir_type(ctx, &rstr_type, node_type);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = load %s, %s* %s\n",
			RSTR_CSTR(&rstr_tmp_label),
			RSTR_CSTR(&rstr_type),
			RSTR_CSTR(&rstr_type),
			RSTR_CSTR(code_gen_name)
		);
		_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_tmp_label));

		_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(code_gen_name));

		rstr_free(&rstr_tmp_label);
		rstr_free(&rstr_type);
	} else if (symbol->type == FE_SYM_FUNC) {
		ParserASTNode *node_type = FE_FUNC_SYMBOL_GET_FUNC_TYPE_NODE(symbol);
		ResizableString rstr_type;
		rstr_init(&rstr_type);
		_ir_type(ctx, &rstr_type, node_type);

		ResizableString rstr_func_name;
		rstr_init_with_char(&rstr_func_name, '@');
		rstr_append_with_raw(
			&rstr_func_name,
			symbol->token->content,
			symbol->token->len
		);

		_expr_result_set_func_result(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_func_name));

		_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_func_name));

		rstr_free(&rstr_type);
		rstr_free(&rstr_func_name);
	} else {
		assert(0);
	}

	return true;
}

static bool _ir_expr_member_arridx_fncall(
	IRGeneratorLlvmirContext *ctx,
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
			// %3 = getelementptr inbounds %struct.A, %struct.A* %1, i32 0, i32 2
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			
			_ExprResult result;
			_expr_result_init(&result);

			_ir_expr(ctx, rstr, &result, node_expr);
			rstr_append_with_rstr(rstr, &(result.rstr_for_result_ptr));

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			assert(node_type->type == FE_NODE_TYPE_STRUCT);
			ParserASTNode *node_struct_id = node_type->childs[0];
			assert(node_struct_id->type == FE_NODE_IDENTIFIER);
			ResizableString rstr_struct_name;
			rstr_init_with_raw(
				&rstr_struct_name, node_struct_id->token->content, node_struct_id->token->len
			);

			int index = fe_sem_get_struct_member_index(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);

			ParserASTNode *node_struct_member_type = fe_sem_get_struct_member_type_node(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);
			ResizableString rstr_struct_member_type;
			rstr_init(&rstr_struct_member_type);
			_ir_type(ctx, &rstr_struct_member_type, node_struct_member_type);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label0;
			rstr_init(&rstr_tmp_label0);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label0);
			rstr_appendf(
				&(expr_result->rstr_for_result_ptr),
				"%s = getelementptr inbounds %%struct.%s, %s, i32 0, i32 %d\n",
				RSTR_CSTR(&rstr_tmp_label0),
				RSTR_CSTR(&rstr_struct_name),
				RSTR_CSTR(&(result.rstr_result_ptr)),
				index
			);
			_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_struct_member_type), RSTR_CSTR(&rstr_tmp_label0));

			ResizableString rstr_tmp_label1;
			rstr_init(&rstr_tmp_label1);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label1);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = load %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label1),
				RSTR_CSTR(&rstr_struct_member_type),
				RSTR_CSTR(&(expr_result->rstr_result_ptr))
			);
			_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_struct_member_type), RSTR_CSTR(&rstr_tmp_label1));

			_expr_result_free(&result);

			rstr_free(&rstr_struct_name);
			rstr_free(&rstr_struct_member_type);
			rstr_free(&rstr_tmp_label0);
			rstr_free(&rstr_tmp_label1);

			return true;
		}
		case FE_NODE_EXPR_PMEMBER: {
			// %3 = load %struct.A*, %struct.A** %1, align 8
			// %4 = getelementptr inbounds %struct.A, %struct.A* %3, i32 0, i32 1
			assert(node->nchilds == 2);
			
			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			
			_ExprResult result;
			_expr_result_init(&result);

			_ir_expr(ctx, rstr, &result, node_expr);
			rstr_append_with_rstr(rstr, &(result.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result.rstr_for_result));

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			assert(node_type->type == FE_NODE_TYPE_POINTER);
			ParserASTNode *node_struct_type = node_type->childs[0];
			assert(node_struct_type->type == FE_NODE_TYPE_STRUCT);
			ParserASTNode *node_struct_id = node_struct_type->childs[0];
			assert(node_struct_id->type == FE_NODE_IDENTIFIER);
			ResizableString rstr_struct_name;
			rstr_init_with_raw(
				&rstr_struct_name, node_struct_id->token->content, node_struct_id->token->len
			);

			int index = fe_sem_get_struct_member_index(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);

			ParserASTNode *node_struct_member_type = fe_sem_get_struct_member_type_node(
				ctx->psrctx, node, node_struct_id->token, node_id->token
			);
			ResizableString rstr_struct_member_type;
			rstr_init(&rstr_struct_member_type);
			_ir_type(ctx, &rstr_struct_member_type, node_struct_member_type);

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label0;
			rstr_init(&rstr_tmp_label0);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label0);
			rstr_appendf(
				&(expr_result->rstr_for_result_ptr),
				"%s = getelementptr inbounds %%struct.%s, %s, i32 0, i32 %d\n",
				RSTR_CSTR(&rstr_tmp_label0),
				RSTR_CSTR(&rstr_struct_name),
				RSTR_CSTR(&(result.rstr_result)),
				index
			);
			_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_struct_member_type), RSTR_CSTR(&rstr_tmp_label0));

			ResizableString rstr_tmp_label1;
			rstr_init(&rstr_tmp_label1);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label1);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = load %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label1),
				RSTR_CSTR(&rstr_struct_member_type),
				RSTR_CSTR(&(expr_result->rstr_result_ptr))
			);
			_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_struct_member_type), RSTR_CSTR(&rstr_tmp_label1));

			_expr_result_free(&result);

			rstr_free(&rstr_struct_name);
			rstr_free(&rstr_struct_member_type);
			rstr_free(&rstr_tmp_label0);
			rstr_free(&rstr_tmp_label1);

			return true;
		}
		case FE_NODE_EXPR_ARRAY_INDEX: {
			// Array:
			//		%5 = getelementptr inbounds [10 x i8], [10 x i8]* %1, i64 0, i64 0
			// Pointer:
			//		%8 = getelementptr inbounds i8, i8* %7, i64 0
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

			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node));

			ResizableString rstr_base_type;
			rstr_init(&rstr_base_type);
			_ir_type(ctx, &rstr_base_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_base));

			ParserSymbol *func_symbol = ctx->func_symbol;
			assert(func_symbol);

			ResizableString rstr_tmp_label_ptr;
			rstr_init(&rstr_tmp_label_ptr);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_ptr);

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			if (node_base_type == FE_TYPE_ARRAY) {
				rstr_appendf(
					&(expr_result->rstr_for_result_ptr),
					"%s = getelementptr inbounds %s, %s, %s 0, %s\n",
					RSTR_CSTR(&rstr_tmp_label_ptr),
					RSTR_CSTR(&rstr_base_type),
					RSTR_CSTR(&(result_base.rstr_result_ptr)),
					_LLVM_TYPE_INTxx(ctx),
					RSTR_CSTR(&(result_index.rstr_result))
				);
				_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_tmp_label_ptr));

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = load %s, %s\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&rstr_type),
					RSTR_CSTR(&(expr_result->rstr_result_ptr))
				);
				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_tmp_label_val));
			} else if (node_base_type == FE_TYPE_POINTER) {
				rstr_appendf(
					&(expr_result->rstr_for_result_ptr),
					"%s = getelementptr inbounds %s, %s, %s\n",
					RSTR_CSTR(&rstr_tmp_label_ptr),
					RSTR_CSTR(&rstr_type),
					RSTR_CSTR(&(result_base.rstr_result)),
					RSTR_CSTR(&(result_index.rstr_result))
				);
				_expr_result_set_result_ptr(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_tmp_label_ptr));

				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = load %s, %s\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&rstr_type),
					RSTR_CSTR(&(expr_result->rstr_result_ptr))
				);
				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_type), RSTR_CSTR(&rstr_tmp_label_val));
			} else {
				assert(0);
			}

			_expr_result_free(&result_base);
			_expr_result_free(&result_index);
			rstr_free(&rstr_type);
			rstr_free(&rstr_base_type);
			rstr_free(&rstr_tmp_label_ptr);
			rstr_free(&rstr_tmp_label_val);

			return true;
		}
		case FE_NODE_EXPR_FUNC_CALL: {
			// call i64 (i8*, ...) @printf(i8* %tmp0, i32 %tmp1)
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

			ResizableString rstr_func_type;
			rstr_init(&rstr_func_type);
			_ir_type(ctx, &rstr_func_type, node_func_expr_func_type);

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
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			if (node_func_type_return_type != NULL) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = ",
					RSTR_CSTR(&rstr_tmp_label_val)
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
				"call %s %s(%s)\n",
				RSTR_CSTR(&rstr_func_type),
				RSTR_CSTR(&(result_func_expr.rstr_result_val)),
				RSTR_CSTR(&rstr_func_params)
			);

			if (node_func_type_return_type != NULL) {
				ResizableString rstr_return_type;
				rstr_init(&rstr_return_type);
				_ir_type(ctx, &rstr_return_type, node_func_type_return_type);

				_expr_result_set_result(expr_result, RSTR_CSTR(&rstr_return_type), RSTR_CSTR(&rstr_tmp_label_val));

				rstr_free(&rstr_return_type);
			}

			rstr_free(&rstr_func_type);
			_expr_result_free(&result_func_expr);
			rstr_free(&rstr_tmp_label_val);
			rstr_free(&rstr_func_params);

			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_unary(
	IRGeneratorLlvmirContext *ctx,
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
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			ResizableString rstr_tmp_label_val1;
			rstr_init(&rstr_tmp_label_val1);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val1);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = icmp eq %s, 0\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = zext i1 %s to %s\n",
				RSTR_CSTR(&rstr_tmp_label_val1),
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result_type))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&rstr_tmp_label_val1)
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);
			rstr_free(&rstr_tmp_label_val1);

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
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&(result_expr.rstr_result_val))
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
			uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node_expr);

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			if (fe_sem_is_signed_type(type)) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = sub nsw %s 0, %s\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&(result_expr.rstr_result_val))
				);
			} else if (fe_sem_is_unsigned_type(type)) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = sub %s 0, %s\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&(result_expr.rstr_result_val))
				);
			} else if (fe_sem_is_float_type(type)) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = fsub %s 0.0, %s\n",
					RSTR_CSTR(&rstr_tmp_label_val),
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&(result_expr.rstr_result_val))
				);
			} else {
				assert(0);
			}
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result_type)),
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
			uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node_expr);

			_ir_expr(ctx, rstr, &result_expr, node_expr);
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_expr.rstr_for_result));

			ResizableString rstr_tmp_label_val;
			rstr_init(&rstr_tmp_label_val);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = xor %s, -1\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);
			return true;
		}
		case FE_NODE_EXPR_INC_LEFT:
		case FE_NODE_EXPR_INC_RIGHT: {
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
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = add %s, 1\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"store %s %s, %s\n",
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result_ptr))
			);
			if (node->type == FE_NODE_EXPR_INC_LEFT) {
				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&rstr_tmp_label_val)
				);
			} else if (node->type == FE_NODE_EXPR_INC_RIGHT) {
				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&(result_expr.rstr_result_val))
				);
			} else {
				assert(0);
			}

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);
			return true;
		}
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
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = add %s, -1\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"store %s %s, %s\n",
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_expr.rstr_result_ptr))
			);
			if (node->type == FE_NODE_EXPR_DEC_LEFT) {
				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&rstr_tmp_label_val)
				);
			} else if (node->type == FE_NODE_EXPR_DEC_RIGHT) {
				_expr_result_set_result(
					expr_result,
					RSTR_CSTR(&(result_expr.rstr_result_type)),
					RSTR_CSTR(&(result_expr.rstr_result_val))
				);
			} else {
				assert(0);
			}

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
				RSTR_CSTR(&(casted_result.rstr_result_type)),
				RSTR_CSTR(&(casted_result.rstr_result_val))
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
				RSTR_CSTR(&(result_expr.rstr_result_ptr_type)),
				RSTR_CSTR(&(result_expr.rstr_result_ptr_val))
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
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node));

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = load %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&rstr_type),
				RSTR_CSTR(&(result_expr.rstr_result))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_type),
				RSTR_CSTR(&rstr_tmp_label_val)
			);

			_expr_result_set_result_ptr_raw(
				expr_result,
				RSTR_CSTR(&(result_expr.rstr_result_type)),
				RSTR_CSTR(&(result_expr.rstr_result_val))
			);

			_expr_result_free(&result_expr);
			rstr_free(&rstr_tmp_label_val);
			rstr_free(&rstr_type);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_mul(
	IRGeneratorLlvmirContext *ctx,
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
	uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node);

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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	if (fe_sem_is_signed_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = mul nsw %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_unsigned_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = mul %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_float_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = fmul %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else {
		assert(0);
	}
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_div(
	IRGeneratorLlvmirContext *ctx,
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
	uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node);

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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	if (fe_sem_is_signed_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = sdiv %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_unsigned_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = udiv %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_float_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = fdiv %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else {
		assert(0);
	}
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_mod(
	IRGeneratorLlvmirContext *ctx,
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
	uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node);

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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	if (fe_sem_is_signed_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = srem %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_unsigned_type(type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = urem %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else {
		assert(0);
	}
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_mul_div_mod(
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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

	if ((fe_sem_is_pointer_type(type1) && fe_sem_is_integer_type(type2))) {
		ParserASTNode *node_lhs = NULL;
		ParserASTNode *node_rhs = NULL;

		if (fe_sem_is_pointer_type(type1) && fe_sem_is_integer_type(type2)) {
			node_lhs = node->childs[0];
			node_rhs = node->childs[1];
		} else if (fe_sem_is_integer_type(type1) && fe_sem_is_pointer_type(type2)) {
			node_lhs = node->childs[1];
			node_rhs = node->childs[0];
		} else {
			assert(0);
		}

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

		ParserASTNode *node_lhs_ptr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
		assert(node_lhs_ptr_type->type == FE_NODE_TYPE_POINTER);
		ParserASTNode *node_lhs_type = node_lhs_ptr_type->childs[0];

		ResizableString rstr_lhs_type;
		rstr_init(&rstr_lhs_type);
		_ir_type(ctx, &rstr_lhs_type, node_lhs_type);

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = getelementptr inbounds %s, %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&rstr_lhs_type),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs.rstr_result))
		);
		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&(result_lhs.rstr_result_type)),
			RSTR_CSTR(&rstr_tmp_label_val)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		rstr_free(&rstr_lhs_type);
		rstr_free(&rstr_tmp_label_val);
	} else if (fe_sem_is_number_type(type1) && fe_sem_is_number_type(type2)) {
		ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);
		uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node);

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
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

		if (fe_sem_is_signed_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = add nsw %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else if (fe_sem_is_unsigned_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = add %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else if (fe_sem_is_float_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = fadd %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else {
			assert(0);
		}
		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
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
	IRGeneratorLlvmirContext *ctx,
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

		ParserASTNode *node_lhs_ptr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_lhs);
		assert(node_lhs_ptr_type->type == FE_NODE_TYPE_POINTER);
		ParserASTNode *node_lhs_type = node_lhs_ptr_type->childs[0];

		ResizableString rstr_lhs_type;
		rstr_init(&rstr_lhs_type);
		_ir_type(ctx, &rstr_lhs_type, node_lhs_type);

		ResizableString rstr_tmp_label_val;
		rstr_init(&rstr_tmp_label_val);
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

		ResizableString rstr_tmp_label_val1;
		rstr_init(&rstr_tmp_label_val1);
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val1);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = sub %s 0, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_rhs.rstr_result_type)),
			RSTR_CSTR(&(result_rhs.rstr_result_val))
		);
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = getelementptr inbounds %s, %s, %s %s\n",
			RSTR_CSTR(&rstr_tmp_label_val1),
			RSTR_CSTR(&rstr_lhs_type),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs.rstr_result_type)),
			RSTR_CSTR(&rstr_tmp_label_val)
		);
		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&(result_lhs.rstr_result_type)),
			RSTR_CSTR(&rstr_tmp_label_val1)
		);

		_expr_result_free(&result_lhs);
		_expr_result_free(&result_rhs);
		rstr_free(&rstr_lhs_type);
		rstr_free(&rstr_tmp_label_val);
		rstr_free(&rstr_tmp_label_val1);
	} else if (fe_sem_is_number_type(type1) && fe_sem_is_number_type(type2)) {
		ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);
		uint8_t type = FE_EXPR_AST_NODE_GET_TYPE(node);

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
		_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

		if (fe_sem_is_signed_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = sub nsw %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else if (fe_sem_is_unsigned_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = sub %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else if (fe_sem_is_float_type(type)) {
			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = fsub %s, %s\n",
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&(result_lhs_casted.rstr_result)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
			);
		} else {
			assert(0);
		}
		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
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
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	rstr_appendf(
		&(expr_result->rstr_for_result),
		"%s = shl %s, %s\n",
		RSTR_CSTR(&rstr_tmp_label_val),
		RSTR_CSTR(&(result_lhs.rstr_result)),
		RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
	);
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static void _ir_expr_shift_right(
	IRGeneratorLlvmirContext *ctx,
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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	uint8_t lhs_type = FE_EXPR_AST_NODE_GET_TYPE(node_lhs);
	if (fe_sem_is_signed_type(lhs_type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = ashr %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else if (fe_sem_is_unsigned_type(lhs_type)) {
		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = lshr %s, %s\n",
			RSTR_CSTR(&rstr_tmp_label_val),
			RSTR_CSTR(&(result_lhs.rstr_result)),
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
		);
	} else {
		assert(0);
	}
	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_shift(
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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

			ResizableString rstr_type;
			rstr_init(&rstr_type);

			_ir_type(ctx, &rstr_type, node_type);

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
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

			ResizableString rstr_tmp_label_val1;
			rstr_init(&rstr_tmp_label_val1);
			_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val1);

			#define	_A(cmd)	\
				rstr_appendf(	\
					&(expr_result->rstr_for_result),	\
					"%s = "cmd" %s, %s\n",	\
					RSTR_CSTR(&rstr_tmp_label_val),	\
					RSTR_CSTR(&(result_lhs_casted.rstr_result)),	\
					RSTR_CSTR(&(result_rhs_casted.rstr_result_val))	\
				);

			switch (node->type) {
				case FE_NODE_EXPR_LT: {
					if (fe_sem_is_signed_type(target_type)) {
						_A("icmp slt")
					} else if (fe_sem_is_unsigned_type(target_type)) {
						_A("icmp ult")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp olt")
					} else {
						assert(0);
					}
					break;
				}
				case FE_NODE_EXPR_LE: {
					if (fe_sem_is_signed_type(target_type)) {
						_A("icmp sle")
					} else if (fe_sem_is_unsigned_type(target_type)) {
						_A("icmp ule")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp ole")
					} else {
						assert(0);
					}
					break;
				}
				case FE_NODE_EXPR_GT: {
					if (fe_sem_is_signed_type(target_type)) {
						_A("icmp sgt")
					} else if (fe_sem_is_unsigned_type(target_type)) {
						_A("icmp ugt")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp ogt")
					} else {
						assert(0);
					}
					break;
				}
				case FE_NODE_EXPR_GE: {
					if (fe_sem_is_signed_type(target_type)) {
						_A("icmp sge")
					} else if (fe_sem_is_unsigned_type(target_type)) {
						_A("icmp uge")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp oge")
					} else {
						assert(0);
					}
					break;
				}
				case FE_NODE_EXPR_EQ: {
					if (fe_sem_is_integer_type(target_type)
							|| fe_sem_is_pointer_type(target_type)) {
						_A("icmp eq")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp oeq")
					} else {
						assert(0);
					}
					break;
				}
				case FE_NODE_EXPR_NEQ: {
					if (fe_sem_is_integer_type(target_type)
							|| fe_sem_is_pointer_type(target_type)) {
						_A("icmp ne")
					} else if (fe_sem_is_float_type(target_type)) {
						_A("fcmp one")
					} else {
						assert(0);
					}
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			#undef	_A

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = zext i1 %s to %s\n",
				RSTR_CSTR(&rstr_tmp_label_val1),
				RSTR_CSTR(&rstr_tmp_label_val),
				RSTR_CSTR(&rstr_type)
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_type),
				RSTR_CSTR(&rstr_tmp_label_val1)
			);

			rstr_free(&rstr_type);
			_expr_result_free(&result_lhs);
			_expr_result_free(&result_rhs);
			_expr_result_free(&result_lhs_casted);
			_expr_result_free(&result_rhs_casted);
			rstr_free(&rstr_tmp_label_val);
			rstr_free(&rstr_tmp_label_val1);

			return true;
		}
		default: {
			return false;
		}
	}
}

static void _ir_expr_bitwise(
	IRGeneratorLlvmirContext *ctx,
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
	_generate_func_tmp(ctx, func_symbol, &rstr_tmp_label_val);

	#define	_A(cmd)	\
		rstr_appendf(	\
			&(expr_result->rstr_for_result),	\
			"%s = "cmd" %s, %s\n",	\
			RSTR_CSTR(&rstr_tmp_label_val),	\
			RSTR_CSTR(&(result_lhs_casted.rstr_result)),	\
			RSTR_CSTR(&(result_rhs_casted.rstr_result_val))	\
		);

	switch (node->type) {
		case FE_NODE_EXPR_BAND:
		case FE_NODE_EXPR_BAND_ASSIGN: {
			_A("and")
			break;
		}
		case FE_NODE_EXPR_BXOR:
		case FE_NODE_EXPR_BXOR_ASSIGN: {
			_A("xor")
			break;
		}
		case FE_NODE_EXPR_BOR:
		case FE_NODE_EXPR_BOR_ASSIGN: {
			_A("or")
			break;
		}
		default: {
			assert(0);
		}
	}

	#undef	_A

	_expr_result_set_result(
		expr_result,
		RSTR_CSTR(&(result_lhs_casted.rstr_result_type)),
		RSTR_CSTR(&rstr_tmp_label_val)
	);

	_expr_result_free(&result_lhs);
	_expr_result_free(&result_rhs);
	_expr_result_free(&result_lhs_casted);
	_expr_result_free(&result_rhs_casted);
	rstr_free(&rstr_tmp_label_val);
}

static bool _ir_expr_band_bxor_bor(
	IRGeneratorLlvmirContext *ctx,
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

static bool _ir_expr_logic(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(expr_result);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_AND:
		case FE_NODE_EXPR_OR: {
			assert(node->nchilds == 2);

			ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);
			
			ResizableString rstr_type;
			rstr_init(&rstr_type);
			_ir_type(ctx, &rstr_type, node_type);

			_TMP_LABEL_DEF(ctx, lhs)
			_TMP_LABEL_DEF(ctx, rhs)
			_TMP_LABEL_DEF(ctx, rhs1)
			_TMP_LABEL_DEF(ctx, end)
			_TMP_LABEL_DEF(ctx, result)
			_TMP_LABEL_DEF(ctx, result_casted)

			ParserASTNode *node_lhs = node->childs[0];
			ParserASTNode *node_rhs = node->childs[1];

			_ExprResult result_lhs;
			_expr_result_init(&result_lhs);

			_ExprResult result_rhs;
			_expr_result_init(&result_rhs);

			_ir_expr(ctx, rstr, &result_lhs, node_lhs);
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_lhs.rstr_for_result));

			ResizableString rstr_lhs_cond;
			rstr_init(&rstr_lhs_cond);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_lhs_cond, &result_lhs, node_lhs);

			// 左表达式的Basic Block的标号。
			rstr_appendf(
				rstr,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(lhs),
				_TMP_LABEL_NAME_CSTR(lhs)
			);

			if (node->type == FE_NODE_EXPR_AND) {
				rstr_appendf(
					rstr,
					"br i1 %s, label %s, label %s\n",
					RSTR_CSTR(&rstr_lhs_cond),
					_TMP_LABEL_CSTR(rhs),
					_TMP_LABEL_CSTR(end)
				);
			} else if (node->type == FE_NODE_EXPR_OR) {
				rstr_appendf(
					rstr,
					"br i1 %s, label %s, label %s\n",
					RSTR_CSTR(&rstr_lhs_cond),
					_TMP_LABEL_CSTR(end),
					_TMP_LABEL_CSTR(rhs)
				);
			} else {
				assert(0);
			}

			// 右表达式的Basic Block的标号。
			rstr_appendf(
				rstr,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(rhs),
				_TMP_LABEL_NAME_CSTR(rhs)
			);

			_ir_expr(ctx, rstr, &result_rhs, node_rhs);
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result_ptr));
			rstr_append_with_rstr(rstr, &(result_rhs.rstr_for_result));

			ResizableString rstr_rhs_cond;
			rstr_init(&rstr_rhs_cond);
			_ir_cond_expr_with_rstr(ctx, rstr, &rstr_rhs_cond, &result_rhs, node_rhs);

			rstr_appendf(
				rstr,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(rhs1),
				_TMP_LABEL_NAME_CSTR(rhs1)
			);

			// 结束的Basic Block的标号。
			rstr_appendf(
				rstr,
				"br label %s\n%s:\n",
				_TMP_LABEL_CSTR(end),
				_TMP_LABEL_NAME_CSTR(end)
			);

			if (node->type == FE_NODE_EXPR_AND) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = phi i1 [ false, %s ], [ %s, %s ]\n",
					_TMP_LABEL_CSTR(result),
					_TMP_LABEL_CSTR(lhs),
					RSTR_CSTR(&rstr_rhs_cond),
					_TMP_LABEL_CSTR(rhs1)
				);
			} else if (node->type == FE_NODE_EXPR_OR) {
				rstr_appendf(
					&(expr_result->rstr_for_result),
					"%s = phi i1 [ true, %s ], [ %s, %s ]\n",
					_TMP_LABEL_CSTR(result),
					_TMP_LABEL_CSTR(lhs),
					RSTR_CSTR(&rstr_rhs_cond),
					_TMP_LABEL_CSTR(rhs1)
				);
			} else {
				assert(0);
			}

			rstr_appendf(
				&(expr_result->rstr_for_result),
				"%s = zext i1 %s to %s\n",
				_TMP_LABEL_CSTR(result_casted),
				_TMP_LABEL_CSTR(result),
				RSTR_CSTR(&rstr_type)
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&rstr_type),
				_TMP_LABEL_CSTR(result_casted)
			);

			rstr_free(&rstr_type);

			_TMP_LABEL_FREE(lhs)
			_TMP_LABEL_FREE(rhs)
			_TMP_LABEL_FREE(rhs1)
			_TMP_LABEL_FREE(end)
			_TMP_LABEL_FREE(result)
			_TMP_LABEL_FREE(result_casted)

			_expr_result_free(&result_lhs);
			_expr_result_free(&result_rhs);

			rstr_free(&rstr_lhs_cond);

			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _ir_expr_cond(
	IRGeneratorLlvmirContext *ctx,
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
		ResizableString rstr_target_type;
		rstr_init(&rstr_target_type);
		_ir_type(ctx, &rstr_target_type, node_target_type);

		_TMP_LABEL_DEF(ctx, expr_true)
		_TMP_LABEL_DEF(ctx, expr_true_end)
		_TMP_LABEL_DEF(ctx, expr_false)
		_TMP_LABEL_DEF(ctx, expr_false_end)
		_TMP_LABEL_DEF(ctx, end)
		_TMP_LABEL_DEF(ctx, result)

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
			"br i1 %s, label %s, label %s\n",
			RSTR_CSTR(&rstr_expr_cond),
			_TMP_LABEL_CSTR(expr_true),
			_TMP_LABEL_CSTR(expr_false)
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_NAME_CSTR(expr_true)
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
			"br label %s\n%s:\n",
			_TMP_LABEL_CSTR(expr_true_end),
			_TMP_LABEL_NAME_CSTR(expr_true_end)
		);
		rstr_appendf(
			rstr,
			"br label %s\n",
			_TMP_LABEL_CSTR(end)
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_NAME_CSTR(expr_false)
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
			"br label %s\n%s:\n",
			_TMP_LABEL_CSTR(expr_false_end),
			_TMP_LABEL_NAME_CSTR(expr_false_end)
		);
		rstr_appendf(
			rstr,
			"br label %s\n",
			_TMP_LABEL_CSTR(end)
		);

		rstr_appendf(
			rstr,
			"%s:\n",
			_TMP_LABEL_NAME_CSTR(end)
		);

		rstr_appendf(
			&(expr_result->rstr_for_result),
			"%s = phi %s [ %s, %s ], [ %s, %s ]\n",
			_TMP_LABEL_CSTR(result),
			RSTR_CSTR(&rstr_target_type),
			RSTR_CSTR(&(result_expr_true_casted.rstr_result_val)),
			_TMP_LABEL_CSTR(expr_true_end),
			RSTR_CSTR(&(result_expr_false_casted.rstr_result_val)),
			_TMP_LABEL_CSTR(expr_false_end)
		);
		_expr_result_set_result(
			expr_result,
			RSTR_CSTR(&rstr_target_type),
			_TMP_LABEL_CSTR(result)
		);

		_TMP_LABEL_FREE(expr_true)
		_TMP_LABEL_FREE(expr_true_end)
		_TMP_LABEL_FREE(expr_false)
		_TMP_LABEL_FREE(expr_false_end)
		_TMP_LABEL_FREE(end)
		_TMP_LABEL_FREE(result)

		_expr_result_free(&result_expr_cond);
		_expr_result_free(&result_expr_true);
		_expr_result_free(&result_expr_true_casted);
		_expr_result_free(&result_expr_false);
		_expr_result_free(&result_expr_false_casted);

		rstr_free(&rstr_target_type);
		rstr_free(&rstr_expr_cond);

		return true;
	} else {
		return false;
	}
}

static bool _ir_expr_assign(
	IRGeneratorLlvmirContext *ctx,
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
			"store %s, %s\n",	\
			RSTR_CSTR(&(result.rstr_result)),	\
			RSTR_CSTR(&rstr_result_lhs_ptr)	\
		);	\
		_expr_result_set_result(	\
			expr_result,	\
			RSTR_CSTR(&(result.rstr_result_type)),	\
			RSTR_CSTR(&(result.rstr_result_val))	\
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
				"store %s, %s\n",
				RSTR_CSTR(&(result_rhs_casted.rstr_result)),
				RSTR_CSTR(&(result_lhs.rstr_result_ptr))
			);
			_expr_result_set_result(
				expr_result,
				RSTR_CSTR(&(result_rhs_casted.rstr_result_type)),
				RSTR_CSTR(&(result_rhs_casted.rstr_result_val))
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
	IRGeneratorLlvmirContext *ctx,
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

	assert(0);

	#undef	_A
}

static void _ir_expr_wrapper(
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	_ir_expr_wrapper(ctx, rstr, expr_result, node);

	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result_ptr));
	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result));
}

static void _ir_expr_wrapper_ptr(
	IRGeneratorLlvmirContext *ctx,
	ResizableString *rstr,
	_ExprResult *expr_result,
	ParserASTNode *node
) {
	_ir_expr_wrapper(ctx, rstr, expr_result, node);

	rstr_append_with_rstr(rstr, &(expr_result->rstr_for_result_ptr));
}

static void _ir_stat_expr(
	IRGeneratorLlvmirContext *ctx,
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

static void _ir_stat(
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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
	IRGeneratorLlvmirContext *ctx,
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

	if (node_body->type == FE_NODE_STATS_BLOCK) {
		rstr_append_with_cstr(ctx->body, "define");
	} else if (node_body->type == FE_NODE_DUMMY) {
		// LLVM IR对于同一个函数不能同时出现define和declare。
		// 如果在ilang中，有一个函数声明，然后又有相应的函数定义，则不为函数声明生成相关的LLVM IR代码（）。
		ParserSymbol *symbol = FE_FUNC_AST_NODE_GET_SYMBOL(node);
		if (!FE_FUNC_SYMBOL_GET_DUMMY(symbol)) {
			return;
		}

		rstr_append_with_cstr(ctx->body, "declare");
	} else {
		assert(0);
	}

	rstr_append_with_char(ctx->body, ' ');

	ResizableString rstr_return_type;
	rstr_init(&rstr_return_type);
	if (node_return_type == NULL) {
		rstr_append_with_cstr(&rstr_return_type, "void");
	} else {
		_ir_type(ctx, &rstr_return_type, node_return_type);
	}
	rstr_append_with_rstr(ctx->body, &rstr_return_type);

	rstr_append_with_char(ctx->body, ' ');

	rstr_append_with_char(ctx->body, '@');
	rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);

	rstr_append_with_char(ctx->body, '(');
	for (int i = 0; i < node_func_params->nchilds; i++) {
		ParserASTNode *node_param = node_func_params->childs[i];

		switch (node_param->type) {
			case FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {
				rstr_append_with_cstr(ctx->body, "...");
				break;
			}
			case FE_NODE_FUNC_PARAMS_ITEM: {
				assert(node_param->nchilds == 2);
				ParserASTNode *node_t = node_param->childs[1];

				ResizableString rstr_func_params_item;
				rstr_init(&rstr_func_params_item);
				_ir_type(ctx, &rstr_func_params_item, node_t);

				rstr_append_with_rstr(ctx->body, &rstr_func_params_item);
				rstr_free(&rstr_func_params_item);
				break;
			}
			default: {
				assert(0);
				break;
			}
		}

		if (i + 1 < node_func_params->nchilds) {
			rstr_append_with_cstr(ctx->body, ", ");
		}
	}
	rstr_append_with_char(ctx->body, ')');

	if (node_body->type == FE_NODE_STATS_BLOCK) {
		rstr_append_with_cstr(ctx->body, " {\n");

		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_param = node_func_params->childs[i];
			if (node_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				continue;
			}

			ParserASTNode *node_param_type = node_param->childs[1];
			ParserSymbol *symbol = FE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_param);

			assert(!FE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol));

			size_t no = FE_FUNC_SYMBOL_NEXT_NO(ctx->func_symbol);
			FE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);	
			ResizableString *code_gen_name = FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);
			rstr_init_with_char(code_gen_name, '%');
			rstr_append_with_raw(
				code_gen_name,
				symbol->token->content,
				symbol->token->len
			);
			rstr_appendf(
				code_gen_name,
				"_%zu",
				no
			);

			ResizableString rstr_param_type;
			rstr_init(&rstr_param_type);
			_ir_type(ctx, &rstr_param_type, node_param_type);
			rstr_appendf(
				ctx->body,
				"%s = alloca %s\n",
				RSTR_CSTR(code_gen_name),
				RSTR_CSTR(&rstr_param_type)
			);
			rstr_appendf(
				ctx->body,
				"store %s %%%d, %s* %s\n",
				RSTR_CSTR(&rstr_param_type),
				i,
				RSTR_CSTR(&rstr_param_type),
				RSTR_CSTR(code_gen_name)
			);
			rstr_free(&rstr_param_type);
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

		rstr_append_with_cstr(ctx->body, "ret ");
		rstr_append_with_rstr(ctx->body, &rstr_return_type);
		if (node_return_type != NULL) {
			rstr_append_with_char(ctx->body, ' ');

			ResizableString rstr_func_default_return_value;
			rstr_init(&rstr_func_default_return_value);
			_ir_func_default_return_value(ctx, &rstr_func_default_return_value, node_return_type);
			rstr_append_with_rstr(ctx->body, &rstr_func_default_return_value);
			rstr_free(&rstr_func_default_return_value);
		}
		rstr_append_with_char(ctx->body, '\n');
		rstr_free(&rstr_return_type);

		rstr_append_with_cstr(ctx->body, "}");
	}
	rstr_append_with_char(ctx->body, '\n');
}









static void _ir_module_item(IRGeneratorLlvmirContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);

	ctx->func_symbol = NULL;

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

	ctx->func_symbol = NULL;
}

static void _ir_module(IRGeneratorLlvmirContext *ctx, ParserASTNode *node) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_MODULE);

	for (int i = 0; i < node->nchilds; i++) {
		_ir_module_item(ctx, node->childs[i]);
	}
}




void fe_irgen_llvmir_generate(IRGeneratorLlvmirContext *ctx) {
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




IRGeneratorLlvmirContext * fe_irgen_llvmir_new_context(ParserContext *psrctx, FILE *out) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	
	IRGeneratorLlvmirContext *ctx = (IRGeneratorLlvmirContext *)malloc(sizeof(IRGeneratorLlvmirContext));
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

void fe_irgen_llvmir_free_context(IRGeneratorLlvmirContext *ctx) {
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
