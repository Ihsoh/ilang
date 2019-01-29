#include "semantics.h"

#include "../../parser.h"
#include "../../util.h"
#include "../../common.h"

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>




#define	_NODE_TYPE(t, node_type)	\
	static ParserASTNode _node_type_##t = {	\
		.type = node_type,	\
		.type_name = #node_type,	\
			\
		.token = NULL,	\
		.file = "SEMANTICS",	\
		.line = 0,	\
		.column = 0,	\
			\
		.next = NULL,	\
		.parent = NULL,	\
			\
		.symtbl = NULL,	\
			\
		.szchilds = 0,	\
		.nchilds = 0,	\
		.childs = NULL,	\
			\
		.szdata = 0	\
	};

_NODE_TYPE(char, FE_NODE_TYPE_CHAR)
_NODE_TYPE(int8, FE_NODE_TYPE_INT8)
_NODE_TYPE(int16, FE_NODE_TYPE_INT16)
_NODE_TYPE(int32, FE_NODE_TYPE_INT32)
_NODE_TYPE(int64, FE_NODE_TYPE_INT64)
_NODE_TYPE(uint8, FE_NODE_TYPE_UINT8)
_NODE_TYPE(uint16, FE_NODE_TYPE_UINT16)
_NODE_TYPE(uint32, FE_NODE_TYPE_UINT32)
_NODE_TYPE(uint64, FE_NODE_TYPE_UINT64)
_NODE_TYPE(float, FE_NODE_TYPE_FLOAT)
_NODE_TYPE(double, FE_NODE_TYPE_DOUBLE)
_NODE_TYPE(struct, FE_NODE_TYPE_STRUCT)
_NODE_TYPE(array, FE_NODE_TYPE_ARRAY)
_NODE_TYPE(func, FE_NODE_TYPE_FUNC)
_NODE_TYPE(pointer, FE_NODE_TYPE_POINTER)
_NODE_TYPE(void, FE_NODE_TYPE_VOID)

#define	_NODE_TYPE_CHAR		(&_node_type_char)
#define	_NODE_TYPE_INT8		(&_node_type_int8)
#define	_NODE_TYPE_INT16	(&_node_type_int16)
#define	_NODE_TYPE_INT32	(&_node_type_int32)
#define	_NODE_TYPE_INT64	(&_node_type_int64)
#define	_NODE_TYPE_UINT8	(&_node_type_uint8)
#define	_NODE_TYPE_UINT16	(&_node_type_uint16)
#define	_NODE_TYPE_UINT32	(&_node_type_uint32)
#define	_NODE_TYPE_UINT64	(&_node_type_uint64)
#define	_NODE_TYPE_FLOAT	(&_node_type_float)
#define	_NODE_TYPE_DOUBLE	(&_node_type_double)
#define	_NODE_TYPE_STRUCT	(&_node_type_struct)
#define	_NODE_TYPE_ARRAY	(&_node_type_array)
#define	_NODE_TYPE_FUNC		(&_node_type_func)
#define	_NODE_TYPE_POINTER	(&_node_type_pointer)
#define	_NODE_TYPE_VOID		(&_node_type_void)

static ParserASTNode *_ptr_node_type_char = &_node_type_char;

static ParserASTNode _node_type_char_ptr = {
	.type = FE_NODE_TYPE_POINTER,
	.type_name = "FE_NODE_TYPE_POINTER",

	.token = NULL,
	.file = "SEMANTICS",
	.line = 0,
	.column = 0,

	.next = NULL,
	.parent = NULL,

	.symtbl = NULL,

	.szchilds = 1,
	.nchilds = 1,
	.childs = &_ptr_node_type_char,

	.szdata = 0
};

#define	_NODE_TYPE_CHAR_POINTER	(&_node_type_char_ptr)

#define	_SYNERR_TOKEN(ctx, token, msg)	{ (ctx)->syntax_error_token_msg((ctx), (token), (msg)); }
#define	_SYNERR_NODE(ctx, node, msg, args...)	{ char __buffer__[1024]; sprintf(__buffer__, msg, ##args); (ctx)->syntax_error_node_msg((ctx), (node), __buffer__); }

#define	_NEW_NODE(ctx, type, token)		(_new_node((ctx), type, #type, token))	

static void _expr(
	ParserContext *ctx,
	ParserASTNode *node
);

static size_t _calc_type_align(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
);

static void _expr_wrapper(
	ParserContext *ctx,
	ParserASTNode *node
);

static int64_t _get_constexpr_int64(
	ParserContext *ctx,
	ParserASTNode *node
);
static int32_t _get_constexpr_int32(
	ParserContext *ctx,
	ParserASTNode *node
);
static int _get_constexpr_int(
	ParserContext *ctx,
	ParserASTNode *node
);
static long _get_constexpr_long(
	ParserContext *ctx,
	ParserASTNode *node
);
static uint64_t _get_constexpr_uint64(
	ParserContext *ctx,
	ParserASTNode *node
);
static uint32_t _get_constexpr_uint32(
	ParserContext *ctx,
	ParserASTNode *node
);
static unsigned int _get_constexpr_uint(
	ParserContext *ctx,
	ParserASTNode *node
);
static unsigned long _get_constexpr_ulong(
	ParserContext *ctx,
	ParserASTNode *node
);

static ParserASTNode * _new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
) {
	assert(ctx);
	assert(type_name);

	return parser_new_node(ctx, type, type_name, token, 0, NULL);
}

static int _get_node_catagory(
	int32_t node_type
) {
	if (FE_IS_TYPE_NODE(node_type)) {
		return FE_SEM_NODE_CATEGORY_TYPE;
	} else if (FE_IS_EXPR_NODE(node_type)) {
		return FE_SEM_NODE_CATAGORY_EXPR;
	} else {
		return FE_SEM_NODE_CATEGORY_UNKNOWN;
	}
}

static uint8_t _get_type_by_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case FE_NODE_TYPE_CHAR: {
			return FE_TYPE_CHAR;
		}
		case FE_NODE_TYPE_INT8: {
			return FE_TYPE_INT8;
		}
		case FE_NODE_TYPE_UINT8: {
			return FE_TYPE_UINT8;
		}
		case FE_NODE_TYPE_INT16: {
			return FE_TYPE_INT16;
		}
		case FE_NODE_TYPE_UINT16: {
			return FE_TYPE_UINT16;
		}
		case FE_NODE_TYPE_INT32: {
			return FE_TYPE_INT32;
		}
		case FE_NODE_TYPE_UINT32: {
			return FE_TYPE_UINT32;
		}
		case FE_NODE_TYPE_INT64: {
			return FE_TYPE_INT64;
		}
		case FE_NODE_TYPE_UINT64: {
			return FE_TYPE_UINT64;
		}
		case FE_NODE_TYPE_FLOAT: {
			return FE_TYPE_FLOAT;
		}
		case FE_NODE_TYPE_DOUBLE: {
			return FE_TYPE_DOUBLE;
		}
		case FE_NODE_TYPE_STRUCT: {
			return FE_TYPE_STRUCT;
		}
		case FE_NODE_TYPE_ARRAY: {
			return FE_TYPE_ARRAY;
		}
		case FE_NODE_TYPE_FUNC: {
			return FE_TYPE_FUNC;
		}
		case FE_NODE_TYPE_POINTER: {
			return FE_TYPE_POINTER;
		}
		case FE_NODE_TYPE_VOID: {
			return FE_TYPE_VOID;
		}
		default: {
			assert(0);
		}
	}

	return FE_TYPE_UNKNOWN;
}

static size_t _get_primitive_type_size(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR: {
			return 1;
		}
		case FE_TYPE_INT8: {
			return 1;
		}
		case FE_TYPE_UINT8: {
			return 1;
		}
		case FE_TYPE_INT16: {
			return 2;
		}
		case FE_TYPE_UINT16: {
			return 2;
		}
		case FE_TYPE_INT32: {
			return 4;
		}
		case FE_TYPE_UINT32: {
			return 4;
		}
		case FE_TYPE_INT64: {
			return 8;
		}
		case FE_TYPE_UINT64: {
			return 8;
		}
		case FE_TYPE_FLOAT: {
			return 4;
		}
		case FE_TYPE_DOUBLE: {
			return 8;
		}
		default: {
			assert(0);
		}
	}
}

static int32_t _get_primitive_type_weight(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR: {
			return 1;
		}
		case FE_TYPE_INT8: {
			return 2;
		}
		case FE_TYPE_UINT8: {
			return 3;
		}
		case FE_TYPE_INT16: {
			return 4;
		}
		case FE_TYPE_UINT16: {
			return 5;
		}
		case FE_TYPE_INT32: {
			return 6;
		}
		case FE_TYPE_UINT32: {
			return 7;
		}
		case FE_TYPE_INT64: {
			return 8;
		}
		case FE_TYPE_UINT64: {
			return 9;
		}
		case FE_TYPE_FLOAT: {
			return 10;
		}
		case FE_TYPE_DOUBLE: {
			return 11;
		}
		default: {
			assert(0);
		}
	}
}

static int32_t _get_primitive_type_weight_by_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	return _get_primitive_type_weight(
		_get_type_by_type_node(ctx, node)
	);
}

static int32_t _get_primitive_type_weight_by_expr_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	return _get_primitive_type_weight(
		FE_EXPR_AST_NODE_GET_TYPE(node)
	);
}

static bool _is_unsigned_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_UINT8:
		case FE_TYPE_UINT16:
		case FE_TYPE_UINT32:
		case FE_TYPE_UINT64: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_unsigned_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_unsigned_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_signed_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR:
		case FE_TYPE_INT8:
		case FE_TYPE_INT16:
		case FE_TYPE_INT32:
		case FE_TYPE_INT64: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_signed_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_signed_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_integer_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR:
		case FE_TYPE_INT8:
		case FE_TYPE_INT16:
		case FE_TYPE_INT32:
		case FE_TYPE_INT64:
		case FE_TYPE_UINT8:
		case FE_TYPE_UINT16:
		case FE_TYPE_UINT32:
		case FE_TYPE_UINT64: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_integer_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_integer_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_float_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_FLOAT:
		case FE_TYPE_DOUBLE: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_float_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_float_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_number_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR:
		case FE_TYPE_INT8:
		case FE_TYPE_INT16:
		case FE_TYPE_INT32:
		case FE_TYPE_INT64:
		case FE_TYPE_UINT8:
		case FE_TYPE_UINT16:
		case FE_TYPE_UINT32:
		case FE_TYPE_UINT64:
		case FE_TYPE_FLOAT:
		case FE_TYPE_DOUBLE: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_number_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_number_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_pointer_type(
	uint8_t type
) {
	return type == FE_TYPE_POINTER;
}

static bool _is_pointer_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_pointer_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_primitive_type(
	uint8_t type
) {
	switch (type) {
		case FE_TYPE_CHAR:
		case FE_TYPE_INT8:
		case FE_TYPE_INT16:
		case FE_TYPE_INT32:
		case FE_TYPE_INT64:
		case FE_TYPE_UINT8:
		case FE_TYPE_UINT16:
		case FE_TYPE_UINT32:
		case FE_TYPE_UINT64:
		case FE_TYPE_FLOAT:
		case FE_TYPE_DOUBLE: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static bool _is_primitive_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));

	return _is_primitive_type(FE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_compatible_primitive_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (same) {
		uint8_t target_type = _get_type_by_type_node(ctx, target);
		uint8_t source_type = _get_type_by_type_node(ctx, source);
		return _is_primitive_type(target_type)
				&& target_type == source_type;
	} else {
		return _is_primitive_type(_get_type_by_type_node(ctx, target))
				&& _is_primitive_type(_get_type_by_type_node(ctx, source));
	}
}

static bool _is_compatible_struct_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_get_type_by_type_node(ctx, target) == FE_TYPE_STRUCT
			&& _get_type_by_type_node(ctx, source) == FE_TYPE_STRUCT) {
		return lexer_compare_token(
			target->childs[0]->token,
			source->childs[0]->token
		);
	} else {
		return false;
	}
}

static bool _is_compatible_array_type_dims(
	ParserContext *ctx,
	ParserASTNode *target_dims,
	ParserASTNode *source_dims
) {
	assert(ctx);
	assert(target_dims);
	assert(source_dims);

	if (target_dims->nchilds != source_dims->nchilds) {
		return false;
	}

	for (int32_t i = 0; i < target_dims->nchilds; i++) {
		ParserASTNode *target_dim = target_dims->childs[i];
		ParserASTNode *source_dim = source_dims->childs[i];
		if (fe_parser_get_uint_val(ctx, target_dim) != fe_parser_get_uint_val(ctx, source_dim)) {
			return false;
		}
	}
	
	return true;
}

static bool _is_compatible_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
);

static bool _is_compatible_array_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_get_type_by_type_node(ctx, target) == FE_TYPE_ARRAY
			&& _get_type_by_type_node(ctx, source) == FE_TYPE_ARRAY) {
		ParserASTNode *target_dims = target->childs[0];
		ParserASTNode *target_type = target->childs[1];
		ParserASTNode *source_dims = source->childs[0];
		ParserASTNode *source_type = source->childs[1];

		return _is_compatible_array_type_dims(ctx, target_dims, source_dims)
				&& _is_compatible_type(ctx, target_type, source_type, same);
	} else {
		return false;
	}

	return false;
}

static bool _is_compatible_func_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_get_type_by_type_node(ctx, target) == FE_TYPE_FUNC
			&& _get_type_by_type_node(ctx, source) == FE_TYPE_FUNC
			&& target->nchilds == source->nchilds) {
		ParserASTNode *target_params = target->childs[0];
		ParserASTNode *source_params = source->childs[0];
		if (target_params->nchilds == source_params->nchilds) {
			for (int32_t i = 0; i < target_params->nchilds; i++) {
				ParserASTNode *target_param = target_params->childs[i];
				ParserASTNode *source_param = source_params->childs[i];

				if (target_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type != FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
					return false;
				}
				if (target_param->type != FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
					return false;
				}

				if (target_param->type != FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type != FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
					ParserASTNode *target_param_type = target_param->childs[1];
					ParserASTNode *source_param_type = source_param->childs[1];
					if (!_is_compatible_type(ctx, target_param_type, source_param_type, true)) {
						return false;
					}
				}
			}
		} else {
			return false;
		}

		if (target->nchilds == 2) {
			ParserASTNode *target_ret_type = target->childs[1];
			ParserASTNode *source_ret_type = source->childs[1];
			if (!_is_compatible_type(ctx, target_ret_type, source_ret_type, true)) {
				return false;
			}
		}

		return true;
	} else {
		return false;
	}
}

static bool _is_compatible_pointer_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_get_type_by_type_node(ctx, target) == FE_TYPE_POINTER
			&& _get_type_by_type_node(ctx, source) == FE_TYPE_POINTER) {
		if (same) {
			ParserASTNode *target_type = target->childs[0];
			ParserASTNode *source_type = source->childs[0];
			return _is_compatible_type(ctx, target_type, source_type, true);
		} else {
			if (_get_type_by_type_node(ctx, target->childs[0]) == FE_TYPE_VOID) {
				return true;
			} else {
				ParserASTNode *target_type = target->childs[0];
				ParserASTNode *source_type = source->childs[0];
				return _is_compatible_type(ctx, target_type, source_type, true);
			}
		}
	} else {
		return false;
	}
}

static bool _is_compatible_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_is_compatible_primitive_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_struct_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_array_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_func_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_pointer_type(ctx, target, source, same)) {
		return true;
	}
	return false;
}

static bool _is_compatible_type_for_assign(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_is_compatible_primitive_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_struct_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_func_type(ctx, target, source, same)) {
		return true;
	}
	if (_is_compatible_pointer_type(ctx, target, source, same)) {
		return true;
	}
	return false;
}

static bool _is_int8_ptr_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	return _get_type_by_type_node(ctx, node) == FE_TYPE_POINTER
			&& _get_type_by_type_node(ctx, node->childs[0]) == FE_TYPE_INT8;
}

static void __type(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t *type,
	bool first_type
) {
	assert(ctx);
	assert(node);
	
	#define _SET_TYPE(t) { if (type != NULL) { *type = (t); } }

	switch (node->type) {
		case FE_NODE_TYPE_CHAR: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_CHAR)
			break;
		}
		case FE_NODE_TYPE_INT8: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_INT8)
			break;
		}
		case FE_NODE_TYPE_UINT8: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_UINT8)
			break;
		}
		case FE_NODE_TYPE_INT16: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_INT16)
			break;
		}
		case FE_NODE_TYPE_UINT16: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_UINT16)
			break;
		}
		case FE_NODE_TYPE_INT32: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_INT32)
			break;
		}
		case FE_NODE_TYPE_UINT32: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_UINT32)
			break;
		}
		case FE_NODE_TYPE_INT64: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_INT64)
			break;
		}
		case FE_NODE_TYPE_UINT64: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_UINT64)
			break;
		}
		case FE_NODE_TYPE_FLOAT: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_FLOAT)
			break;
		}
		case FE_NODE_TYPE_DOUBLE: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_DOUBLE)
			break;
		}
		case FE_NODE_TYPE_STRUCT: {
			assert(node->nchilds == 1);

			_SET_TYPE(FE_TYPE_STRUCT)

			ParserASTNode *node_id = node->childs[0];
			assert(node_id->type == FE_NODE_IDENTIFIER);

			LexerToken *token = node_id->token;
			ParserSymbol *symbol_struct = parser_get_symbol_from_node(ctx, node, FE_SYM_STRUCT, token);
			if (symbol_struct == NULL) {
				_SYNERR_TOKEN(ctx, token, "undefined struct.");
			} else {
				if (first_type
						&& FE_STRUCT_SYMBOL_GET_DUMMY(symbol_struct)) {
					_SYNERR_TOKEN(ctx, token, "struct not implemented.");
				}
			}

			break;
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node->nchilds == 2);

			_SET_TYPE(FE_TYPE_ARRAY)

			ParserASTNode *node_array_dims = node->childs[0];
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);
			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == FE_NODE_LITERAL_UINT);

				uint64_t dim = fe_parser_get_uint_val(ctx, node_array_dim);
				if (dim == 0) {
					_SYNERR_NODE(
						ctx,
						node_array_dim,
						"array dimension size cannot be 0."
					);
				}
			}
			
			ParserASTNode *node_type = node->childs[1];
			__type(ctx, node_type, NULL, false);

			break;
		}
		case FE_NODE_TYPE_FUNC: {
			assert(node->nchilds == 1 || node->nchilds == 2);

			_SET_TYPE(FE_TYPE_FUNC)

			ParserASTNode *node_params = NULL;
			ParserASTNode *node_type = NULL;

			node_params = node->childs[0];
			for (int i = 0; i < node_params->nchilds; i++) {
				ParserASTNode *node_param = node_params->childs[i];
				switch (node_param->type) {
					case FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {

						break;
					}
					case FE_NODE_FUNC_PARAMS_ITEM: {
						assert(node_param->nchilds == 2);
						
						ParserASTNode *node_param_id = node_param->childs[0];

						ParserASTNode *node_param_type = node_param->childs[1];
						__type(ctx, node_param_type, NULL, false);

						break;
					}
					default: {
						assert(0);
					}
				}
			}
			
			if (node->nchilds == 2) {
				node_type = node->childs[1];
				__type(ctx, node_type, NULL, false);
			}

			break;
		}
		case FE_NODE_TYPE_POINTER: {
			assert(node->nchilds == 1);

			_SET_TYPE(FE_TYPE_POINTER)

			ParserASTNode *node_type = node->childs[0];
			__type(ctx, node_type, NULL, false);

			break;
		}
		case FE_NODE_TYPE_VOID: {
			assert(node->nchilds == 0);
			_SET_TYPE(FE_TYPE_VOID)
			break;
		}
		default: {
			assert(0);
		}
	}

	#undef	_SET_TYPE
}

static void _type(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t *type
) {
	assert(ctx);
	assert(node);

	__type(ctx, node, type, true);
}

static void _stats_block(
	ParserContext *ctx,
	ParserASTNode *node
);




static uint8_t _get_var_scope_type(
	ParserASTNode *node,
	ParserSymbol **symbol_func
) {
	assert(node);
	assert(node->type == FE_NODE_VAR);

	if (symbol_func != NULL) {
		*symbol_func = NULL;
	}

	ParserASTNode *parent = node->parent;
	while (parent != NULL) {
		switch (parent->type) {
			case FE_NODE_MODULE: {
				return FE_VAR_TYPE_GLOBAL;
			}
			case FE_NODE_FUNC: {
				if (symbol_func != NULL) {
					*symbol_func = FE_FUNC_AST_NODE_GET_SYMBOL(parent);
				}
				return FE_VAR_TYPE_LOCAL;
			}
			case FE_NODE_STRUCT: {
				return FE_VAR_TYPE_STRUCT_MEMBER;
			}
		}
		parent = parent->parent;
	}

	assert(0);
}

static void _var_with_parent(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_parent
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);
	assert(node->nchilds > 0);
	assert(node_parent);

	ParserASTNode *align_node = FE_VAR_AST_NODE_GET_ALIGN_NODE(node);
	int align = 0;
	if (align_node != NULL) {
		assert(align_node->nchilds == 1);
		switch (_get_node_catagory(align_node->childs[0]->type)) {
			case FE_SEM_NODE_CATEGORY_TYPE: {
				ParserASTNode *type_node = align_node->childs[0];
				_type(ctx, type_node, NULL);
				align = (int) _calc_type_align(ctx, node, type_node);
				assert(align >= 1);
				FE_VAR_AST_NODE_SET_ALIGN(node, align);
				break;
			}
			case FE_SEM_NODE_CATAGORY_EXPR: {
				ParserASTNode *expr_node = align_node->childs[0];
				_expr(ctx, expr_node);
				if (!FE_EXPR_AST_NODE_GET_CONSTANT(expr_node)) {
					_SYNERR_NODE(ctx, expr_node, "align parameter must be constant expression.");
				}
				align = _get_constexpr_int(ctx, expr_node);
				if (align <= 0) {
					_SYNERR_NODE(ctx, expr_node, "align parameter must be greater than 0.");
				}
				if (align != 1 && align % 2 != 0) {
					_SYNERR_NODE(ctx, expr_node, "align parameter is not a power of 2.");
				}
				FE_VAR_AST_NODE_SET_ALIGN(node, align);
				break;
			}
			default: {
				assert(0);
				break;
			}
		}
	}

	ParserSymbol *func_symbol = NULL;
	uint8_t var_scope_type = _get_var_scope_type(node, &func_symbol);
	FE_VAR_AST_NODE_SET_TYPE(node, var_scope_type);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == FE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_id = node_var_item->childs[0];
		assert(node_id->type == FE_NODE_IDENTIFIER);

		ParserASTNode *node_type = node_var_item->childs[1];
		uint8_t var_type = 0;
		_type(ctx, node_type, &var_type);
		if (var_type == FE_TYPE_FUNC) {
			_SYNERR_NODE(
					ctx,
					node,
					"cannot define a function type variable, "
					"but can define a function pointer type(*function(...)) variable."
				);
		}
		
		ParserSymbol *symbol = fe_parser_add_var_symbol_to_node(
			ctx,
			node_parent,
			node_id->token,
			var_type,
			node_type
		);
		FE_VAR_SYMBOL_SET_FUNC_SYMBOL(symbol, func_symbol);
		if (align > 0) {
			FE_VAR_SYMBOL_SET_ALIGN(symbol, align);
		} else {
			FE_VAR_SYMBOL_SET_ALIGN(symbol, _calc_type_align(ctx, node, node_type));
		}

		FE_VAR_ITEM_AST_NODE_SET_SYMBOL(node_var_item, symbol);

		if (node_var_item->nchilds == 3) {
			// 如果变量是结构体成员变量，则不被允许包含初始化表达式。
			if (var_scope_type == FE_VAR_TYPE_STRUCT_MEMBER) {
				_SYNERR_NODE(
					ctx,
					node,
					"struct member variable cannot contain initial expression."
				);
			}

			ParserASTNode *node_expr = node_var_item->childs[2];

			// 插入一个cast节点进行类型强制转换。
			ParserASTNode *node_cast = fe_parser_new_node(
				ctx, FE_NODE_EXPR_CAST, "FE_NODE_EXPR_CAST", NULL
			);
			parser_add_child(ctx, node_cast, node_type);
			parser_insert_node(ctx, &(node_expr->childs[0]), node_cast);

			_expr_wrapper(ctx, node_expr);

			// 如果变量是全局变量，则初始化表达式只允许常量表达式。
			bool const_expr = FE_EXPR_AST_NODE_GET_CONSTANT(node_expr);
			if (var_scope_type == FE_VAR_TYPE_GLOBAL && !const_expr) {
				_SYNERR_NODE(
					ctx,
					node,
					"variable initial expression must be constant expression."
				);
			}

			ParserASTNode *node_expr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			if (!_is_compatible_type_for_assign(ctx, node_type, node_expr_type, false)) {
				_SYNERR_NODE(
					ctx,
					node,
					"incompatible initial expression."
				);
			}
		}

	}
}

static void _var(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);
	assert(node->nchilds > 0);

	ParserASTNode *node_parent = node->parent;
	assert(node_parent);

	return _var_with_parent(ctx, node, node_parent);
}

static void _stat_var(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_VAR);

	_var(ctx, node);
}

static void _stat_if(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_IF);
	assert(node->nchilds >= 2);

	ParserASTNode *node_if_cond_expr = node->childs[0];
	_expr_wrapper(ctx, node_if_cond_expr);
	if (!_is_integer_type_node(ctx, node_if_cond_expr)
			&& !_is_pointer_type_node(ctx, node_if_cond_expr)) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'if' condition expression requires integer type or pointer type."
		);
	}

	ParserASTNode *node_if_body = node->childs[1];
	_stats_block(ctx, node_if_body);

	for (int i = 2; i < node->nchilds; i++) {
		ParserASTNode *child = node->childs[i];
		if (child->type == FE_NODE_STAT_ELIF) {
			assert(child->nchilds == 2);
			ParserASTNode *node_elif_cond_expr = child->childs[0];
			_expr_wrapper(ctx, node_elif_cond_expr);
			if (!_is_integer_type_node(ctx, node_elif_cond_expr)
					&& !_is_pointer_type_node(ctx, node_elif_cond_expr)) {
				_SYNERR_NODE(
					ctx,
					node,
					"statement 'elif' condition expression requires integer type or pointer type."
				);
			}

			ParserASTNode *node_elif_body = child->childs[1];
			_stats_block(ctx, node_elif_body);
		} else if (child->type == FE_NODE_STAT_ELSE) {
			assert(child->nchilds == 1);
			_stats_block(ctx, child->childs[0]);
		} else {
			assert(0);
		}
	}
}

static void _stat_while(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_WHILE);
	assert(node->nchilds == 2);

	ParserASTNode *node_while_cond_expr = node->childs[0];
	_expr_wrapper(ctx, node_while_cond_expr);
	if (!_is_integer_type_node(ctx, node_while_cond_expr)
			&& !_is_pointer_type_node(ctx, node_while_cond_expr)) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'while' condition expression requires integer type or pointer type."
		);
	}

	ParserASTNode *node_while_body = node->childs[1];
	_stats_block(ctx, node_while_body);
}

static void _stat_do(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_DO);
	assert(node->nchilds == 2);

	ParserASTNode *node_do_body = node->childs[0];
	_stats_block(ctx, node_do_body);

	ParserASTNode *node_do_cond_expr = node->childs[1];
	_expr_wrapper(ctx, node_do_cond_expr);
	if (!_is_integer_type_node(ctx, node_do_cond_expr)
			&& !_is_pointer_type_node(ctx, node_do_cond_expr)) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'do...while' condition expression requires integer type or pointer type."
		);
	}
}

static void _stat_for(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_FOR);
	assert(node->nchilds == 4);

	ParserASTNode *node_for_init = node->childs[0];
	assert(node_for_init->type == FE_NODE_STAT_FOR_INIT);
	assert(node_for_init->nchilds == 0 || node_for_init->nchilds == 1);
	if (node_for_init->nchilds == 1) {
		ParserASTNode *node_for_init_var = node_for_init->childs[0];
		_var_with_parent(ctx, node_for_init_var, node);
	}

	ParserASTNode *node_for_expr_cond = node->childs[1];
	assert(node_for_expr_cond->type == FE_NODE_STAT_FOR_EXPR_COND);
	assert(node_for_expr_cond->nchilds == 0 || node_for_expr_cond->nchilds == 1);
	if (node_for_expr_cond->nchilds == 1) {
		ParserASTNode *node_for_expr_cond_expr = node_for_expr_cond->childs[0];
		_expr_wrapper(ctx, node_for_expr_cond_expr);

		if (!_is_integer_type_node(ctx, node_for_expr_cond_expr)
				&& !_is_pointer_type_node(ctx, node_for_expr_cond_expr)) {
			_SYNERR_NODE(
				ctx,
				node,
				"statement 'for' condition expression requires integer type or pointer type."
			);
		}
	}

	ParserASTNode *node_for_expr = node->childs[2];
	assert(node_for_expr->type == FE_NODE_STAT_FOR_EXPR);
	assert(node_for_expr->nchilds == 0 || node_for_expr->nchilds == 1);
	if (node_for_expr->nchilds == 1) {
		ParserASTNode *node_for_expr_expr = node_for_expr->childs[0];
		_expr_wrapper(ctx, node_for_expr_expr);
	}

	ParserASTNode *node_for_body = node->childs[3];
	assert(node_for_body->type == FE_NODE_STATS_BLOCK);
	_stats_block(ctx, node_for_body);
}

static ParserASTNode * __search_node_along_parent(
	ParserASTNode *node,
	va_list arg_ptr
) {
	assert(node);
	assert(arg_ptr);

	unsigned int types[32];
	size_t count = 0;

	for (;;) {
		unsigned int type = va_arg(arg_ptr, unsigned int);
		if (type == FE_NODE_INVALID) {
			break;
		}
		if (count >= sizeof(types) / sizeof(unsigned int)) {
			assert(0);
		}
		types[count++] = type;
	}

	ParserASTNode *parent = node;
	while (parent != NULL) {
		for (int i = 0; i < count; i++) {
			if (parent->type == types[i]) {
				return parent;
			}
		}
		parent = parent->parent;
	}
	return NULL;
}

static ParserASTNode * _search_node_along_parent(
	ParserASTNode *node,
	...
) {
	assert(node);

	va_list arg_ptr;
	va_start(arg_ptr, node);
	
	return __search_node_along_parent(node, arg_ptr);
}

static void _stat_break(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_BREAK);
	assert(node->nchilds == 0);

	if (NULL == _search_node_along_parent(
		node->parent, FE_NODE_STAT_WHILE, FE_NODE_STAT_DO, FE_NODE_STAT_FOR, 0
	)) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'break' must be use in loop."
		);
	}
}

static void _stat_continue(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_CONTINUE);
	assert(node->nchilds == 0);

	if (NULL == _search_node_along_parent(
		node->parent, FE_NODE_STAT_WHILE, FE_NODE_STAT_DO, FE_NODE_STAT_FOR, 0
	)) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'continue' must be use in loop."
		);
	}
}

static void _stat_return(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_RETURN);

	ParserASTNode *node_func = _search_node_along_parent(node->parent, FE_NODE_FUNC, 0);
	if (node_func == NULL) {
		_SYNERR_NODE(
			ctx,
			node,
			"statement 'return' must be use in function."
		);
	}

	ParserASTNode *node_return_type = NULL;
	if (node_func->nchilds == 4) {
		node_return_type = node_func->childs[2];
	}

	if (node->nchilds == 0) {
		if (node_return_type != NULL) {
			_SYNERR_NODE(
				ctx,
				node,
				"statement 'return' requires expression."
			);
		}
	} else if (node->nchilds == 1) {
		if (node_return_type == NULL) {
			_SYNERR_NODE(
				ctx,
				node,
				"statement 'return' does not require expression."
			);
		}

		ParserASTNode *node_return_expr = node->childs[0];
		_expr_wrapper(ctx, node_return_expr);

		if (!_is_compatible_type_for_assign(
			ctx, node_return_type, FE_EXPR_AST_NODE_GET_TYPE_NODE(node_return_expr), false
		)) {
			_SYNERR_NODE(
				ctx,
				node,
				"statement 'return' expression type not matched function return type."
			);
		}
	} else {
		assert(0);
	}
}

static void _stat_goto(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_GOTO);
	assert(node->nchilds == 1);

	ParserASTNode *node_id = node->childs[0];
	assert(node_id->type == FE_NODE_IDENTIFIER);

	if (!parser_get_symbol_from_node(ctx, node, FE_SYM_LABEL, node_id->token)) {
		_SYNERR_TOKEN(
			ctx,
			node_id->token,
			"undefined label."
		);
	}
}

static void _stat_label(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_LABEL);
	assert(node->nchilds == 1);


}

static void _stat_asm(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_ASM);

	_SYNERR_NODE(ctx, node, "asm is not supported.");
}

static void _stat_dummy(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_DUMMY);

	// 什么也不干。
}

static void _check_va(
	ParserContext *ctx,
	ParserASTNode *node,
	const char *name
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_START
			|| node->type == FE_NODE_STAT_VA_END
			|| node->type == FE_NODE_STAT_VA_COPY
			|| node->type == FE_NODE_EXPR_VA_ARG);
	assert(name);

	ParserASTNode *parent = node->parent;
	while (parent != NULL) {
		if (parent->type == FE_NODE_FUNC) {
			
			return;
		}
		parent = parent->parent;
	}

	assert(0);
}

static void _stat_va_start(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_START);
	assert(node->nchilds == 1);

	ParserASTNode *node_expr = node->childs[0];
	_expr_wrapper(ctx, node_expr);
	


	if (!_is_int8_ptr_type_node(
		ctx,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr)
	)) {
		_SYNERR_NODE(ctx, node, "__va_start parameter type must be '*int8'.");
	}
}

static void _stat_va_end(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_END);
	assert(node->nchilds == 1);

	ParserASTNode *node_expr = node->childs[0];
	_expr_wrapper(ctx, node_expr);

	if (!_is_int8_ptr_type_node(
		ctx,
		FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr)
	)) {
		_SYNERR_NODE(ctx, node, "__va_end parameter type must be '*int8'.");
	}
}

static void _stat_va_copy(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STAT_VA_COPY);
	assert(node->nchilds == 2);

	ParserASTNode *node_expr_target = node->childs[0];
	_expr_wrapper(ctx, node_expr_target);
	ParserASTNode *node_expr_target_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_target);

	ParserASTNode *node_expr_source = node->childs[1];
	_expr_wrapper(ctx, node_expr_source);
	ParserASTNode *node_expr_source_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_source);

	if (!_is_int8_ptr_type_node(ctx, node_expr_target_type)
			|| !_is_int8_ptr_type_node(ctx, node_expr_source_type)) {
		_SYNERR_NODE(ctx, node, "__va_end first parameter type and second parameter type must be '*int8'.");
	}
}

static int _expr_is_lvalue(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	if (node->type == FE_NODE_EXPR_ATOM
			&& node->childs[0]->type == FE_NODE_IDENTIFIER) {
		return 1;
	}

	if (node->type == FE_NODE_EXPR_MEMBER) {
		return 1;
	}

	if (node->type == FE_NODE_EXPR_PMEMBER) {
		return 1;
	}

	if (node->type == FE_NODE_EXPR_ARRAY_INDEX) {
		return 1;
	}

	if (node->type == FE_NODE_EXPR_DEREF) {
		return 1;
	}

	return 0;
}




// 1、	结构体中元素按照定义顺序依次置于内存中，但并不是紧密排列。
//		从结构体首地址开始依次将元素放入内存时，元素会被放置在其自身对齐大小的整数倍地址上。
//
// 2、	如果结构体大小不是所有元素中最大对齐大小的整数倍，
//		则结构体对齐到最大元素对齐大小的整数倍，填充空间放置到结构体末尾。
//
// 3、	基本数据类型的对齐大小为其自身的大小，
//		结构体数据类型的对齐大小为其元素中最大对齐大小元素的对齐大小。

static size_t _calc_struct_type_align(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);
	assert(node_type->type == FE_NODE_TYPE_STRUCT);
	assert(node_type->nchilds == 1);

	ParserASTNode *node_id = node_type->childs[0];
	assert(node_id);
	assert(node_id->type == FE_NODE_IDENTIFIER);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(ctx, node, FE_SYM_STRUCT, node_id->token);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, node_id->token, "undefined struct.");
	}

	ParserASTNode *struct_node = FE_STRUCT_SYMBOL_GET_STRUCT_NODE(struct_symbol);
	assert(struct_node);
	assert(struct_node->type == FE_NODE_STRUCT);
	if (FE_STRUCT_AST_NODE_GET_PACKED(struct_node)) {
		return 1;
	}

	ParserASTNode *struct_body_node = FE_STRUCT_SYMBOL_GET_BODY_NODE(struct_symbol);
	assert(struct_body_node);
	assert(struct_body_node->type == FE_NODE_STRUCT_BODY);

	size_t max_align = 1;

	for (int i = 0; i < struct_body_node->nchilds; i++) {
		ParserASTNode *struct_body_member_node = struct_body_node->childs[i];
		assert(struct_body_member_node);
		assert(struct_body_member_node->type == FE_NODE_VAR);
		for (int j = 0; j < struct_body_member_node->nchilds; j++) {
			ParserASTNode *var_item_node = struct_body_member_node->childs[j];
			assert(var_item_node);
			assert(var_item_node->type == FE_NODE_VAR_ITEM);
			assert(var_item_node->nchilds >= 2);

			ParserASTNode *node_var_type = var_item_node->childs[1];
			size_t align = _calc_type_align(ctx, node, node_var_type);
			if (align > max_align) {
				max_align = align;
			}
		}
	}

	return max_align;
}

static size_t _calc_type_align(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR: {
			return 1;
		}
		case FE_NODE_TYPE_INT8: {
			return 1;
		}
		case FE_NODE_TYPE_INT16: {
			return 2;
		}
		case FE_NODE_TYPE_INT32: {
			return 4;
		}
		case FE_NODE_TYPE_INT64: {
			return 8;
		}
		case FE_NODE_TYPE_UINT8: {
			return 1;
		}
		case FE_NODE_TYPE_UINT16: {
			return 2;
		}
		case FE_NODE_TYPE_UINT32: {
			return 4;
		}
		case FE_NODE_TYPE_UINT64: {
			return 8;
		}
		case FE_NODE_TYPE_FLOAT: {
			return 4;
		}
		case FE_NODE_TYPE_DOUBLE: {
			return 8;
		}
		case FE_NODE_TYPE_STRUCT: {
			return _calc_struct_type_align(ctx, node, node_type);
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_type = node_type->childs[1];
			assert(node_array_type);

			return _calc_type_align(ctx, node, node_array_type);
		}
		case FE_NODE_TYPE_FUNC: {
			return 0;
		}
		case FE_NODE_TYPE_POINTER: {
			if (ctx->arch == FE_ARCH_32) {
				return 4;
			} else if (ctx->arch == FE_ARCH_64) {
				return 8;
			} else {
				assert(0);
				return 0;
			}
		}
		case FE_NODE_TYPE_VOID: {
			return 1;
		}
		default: {
			assert(0);
			return 0;
		}
	}
}

static size_t _calc_type_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
);

static size_t _calc_struct_type_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);
	assert(node_type->type == FE_NODE_TYPE_STRUCT);
	assert(node_type->nchilds == 1);

	ParserASTNode *node_id = node_type->childs[0];
	assert(node_id);
	assert(node_id->type == FE_NODE_IDENTIFIER);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(ctx, node, FE_SYM_STRUCT, node_id->token);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, node_id->token, "undefined struct.");
	}

	ParserASTNode *struct_node = FE_STRUCT_SYMBOL_GET_STRUCT_NODE(struct_symbol);
	assert(struct_node);
	assert(struct_node->type == FE_NODE_STRUCT);

	ParserASTNode *struct_body_node = FE_STRUCT_SYMBOL_GET_BODY_NODE(struct_symbol);
	assert(struct_body_node);
	assert(struct_body_node->type == FE_NODE_STRUCT_BODY);

	size_t max_align = 1;
	size_t size = 0;

	for (int i = 0; i < struct_body_node->nchilds; i++) {
		ParserASTNode *struct_body_member_node = struct_body_node->childs[i];
		assert(struct_body_member_node);
		assert(struct_body_member_node->type == FE_NODE_VAR);
		for (int j = 0; j < struct_body_member_node->nchilds; j++) {
			ParserASTNode *var_item_node = struct_body_member_node->childs[j];
			assert(var_item_node);
			assert(var_item_node->type == FE_NODE_VAR_ITEM);
			assert(var_item_node->nchilds >= 2);

			ParserASTNode *node_var_type = var_item_node->childs[1];
			size_t align = 1;
			if (FE_STRUCT_AST_NODE_GET_PACKED(struct_node)) {
				align = 1;
			} else {
				align = _calc_type_align(ctx, node, node_var_type);
			}

			if (align > max_align) {
				max_align = align;
			}

			if (size % align > 0) {
				size += align - size % align;
			}

			size += _calc_type_size(ctx, node, node_var_type);
		}
	}

	if (size % max_align > 0) {
		size += max_align - size % max_align;
	}

	return size;
}

static size_t _calc_type_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);

	switch (node_type->type) {
		case FE_NODE_TYPE_CHAR: {
			return 1;
		}
		case FE_NODE_TYPE_INT8: {
			return 1;
		}
		case FE_NODE_TYPE_INT16: {
			return 2;
		}
		case FE_NODE_TYPE_INT32: {
			return 4;
		}
		case FE_NODE_TYPE_INT64: {
			return 8;
		}
		case FE_NODE_TYPE_UINT8: {
			return 1;
		}
		case FE_NODE_TYPE_UINT16: {
			return 2;
		}
		case FE_NODE_TYPE_UINT32: {
			return 4;
		}
		case FE_NODE_TYPE_UINT64: {
			return 8;
		}
		case FE_NODE_TYPE_FLOAT: {
			return 4;
		}
		case FE_NODE_TYPE_DOUBLE: {
			return 8;
		}
		case FE_NODE_TYPE_STRUCT: {
			return _calc_struct_type_size(ctx, node, node_type);
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_dims = node_type->childs[0];
			assert(node_array_dims);
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);

			ParserASTNode *node_array_type = node_type->childs[1];
			assert(node_array_type);

			size_t size = _calc_type_size(ctx, node, node_array_type);
			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim);

				size *= fe_parser_get_uint_val(ctx, node_array_dim);
			}

			return size;
		}
		case FE_NODE_TYPE_FUNC: {
			return 0;
		}
		case FE_NODE_TYPE_POINTER: {
			if (ctx->arch == FE_ARCH_32) {
				return 4;
			} else if (ctx->arch == FE_ARCH_64) {
				return 8;
			} else {
				assert(0);
				return 0;
			}
		}
		case FE_NODE_TYPE_VOID: {
			return 1;
		}
		default: {
			assert(0);
			return 0;
		}
	}
}

static size_t _calc_pointer_type_increment_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);
	assert(node_type->type == FE_NODE_TYPE_POINTER);
	assert(node_type->nchilds == 1);

	return _calc_type_size(ctx, node, node_type->childs[0]);
}

static void _calc_unary_constexpr(
	ParserContext *ctx,
	uint32_t expr_node_type,
	ParserASTNode *target,
	ParserASTNode *source
) {
	assert(ctx);
	assert(target);
	assert(source);
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source));

	uint8_t source_type = FE_EXPR_AST_NODE_GET_TYPE(source);

	#define	_A(op, type)	\
		case FE_TYPE_##type: {	\
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(target, op FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(source));	\
			break;	\
		}
	
	#define	_B(op)	\
		_A(op, CHAR)	\
		_A(op, INT8)	\
		_A(op, INT16)	\
		_A(op, INT32)	\
		_A(op, INT64)	\
		_A(op, UINT8)	\
		_A(op, UINT16)	\
		_A(op, UINT32)	\
		_A(op, UINT64)

	#define	_C(op, node_type)	\
		case node_type: {	\
			switch (source_type) {	\
				_B(op)	\
				default: {	\
					assert(0);	\
					break;	\
				}	\
			}	\
			break;	\
		}

	switch (expr_node_type) {
		_C(!, FE_NODE_EXPR_NOT)
		_C(~, FE_NODE_EXPR_BNOT)
		_C(+, FE_NODE_EXPR_POSITIVE)
		_C(-, FE_NODE_EXPR_NEGATIVE)
		default: {
			assert(0);
			break;
		}
	}

	#undef	_A
	#undef	_B
	#undef	_C
}

static void _calc_cast_constexpr(
	ParserContext *ctx,
	uint8_t target_type,
	ParserASTNode *target,
	ParserASTNode *source
) {
	assert(ctx);
	assert(target);
	assert(source);

	uint8_t source_type = _get_type_by_type_node(ctx, FE_EXPR_AST_NODE_GET_TYPE_NODE(source));

	// 常量表达式不能把指针类型转换为非指针类型。
	if (source_type == FE_TYPE_POINTER
			&& target_type != FE_TYPE_POINTER) {
		_SYNERR_NODE(ctx, source, "not a compile-time constant.");
	}

	#define	_A(target_t, target_t_c)	\
		case FE_TYPE_##target_t: {	\
			switch (source_type) {	\
				case FE_TYPE_CHAR: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_INT8: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_INT16: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_INT32: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_INT64: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_UINT8: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_UINT16: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_UINT32: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_UINT64: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_FLOAT: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_DOUBLE: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(source)	\
					);	\
					break;	\
				}	\
				case FE_TYPE_POINTER: {	\
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source)	\
					);	\
					break;	\
				}	\
				default: {	\
					assert(0);	\
				}	\
			}	\
			break;	\
		}


	switch (target_type) {
		_A(CHAR, char)
		_A(INT8, int8_t)
		_A(INT16, int16_t)
		_A(INT32, int32_t)
		_A(INT64, int64_t)
		_A(UINT8, uint8_t)
		_A(UINT16, uint16_t)
		_A(UINT32, uint32_t)
		_A(UINT64, uint64_t)
		_A(FLOAT, float)
		_A(DOUBLE, double)
		_A(POINTER, uint64_t)
		default: {
			assert(0);
		}
	}


	#undef	_A
}

static void _set_constexpr_int64(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	int64_t value
) {
	assert(ctx);
	assert(node);

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
			break;	\
		}

	switch (target_type) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
		}
	}

	#undef	_A
}

static void _set_constexpr_int32(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	int32_t value
) {
	return _set_constexpr_int64(ctx, node, target_type, value);
}

static void _set_constexpr_uint64(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	uint64_t value
) {
	assert(ctx);
	assert(node);

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
			break;	\
		}

	switch (target_type) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
		}
	}

	#undef	_A
}

static void _set_constexpr_uint32(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	uint32_t value
) {
	_set_constexpr_uint64(ctx, node, target_type, value);
}

static void _set_constexpr_double(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	double value
) {
	assert(ctx);
	assert(node);

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
			break;	\
		}

	switch (target_type) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
		}
	}

	#undef	_A
}

static void _set_constexpr_float(
	ParserContext *ctx,
	ParserASTNode *node,
	uint8_t target_type,
	float value
) {
	_set_constexpr_double(ctx, node, target_type, value);
}




static int64_t _get_constexpr_int64(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			return (int64_t) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
			return 0;
		}
	}

	#undef	_A
}

static int32_t _get_constexpr_int32(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (int32_t) _get_constexpr_int64(ctx, node);
}

static int _get_constexpr_int(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (int) _get_constexpr_int64(ctx, node);
}

static long _get_constexpr_long(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (long) _get_constexpr_int64(ctx, node);
}

static uint64_t _get_constexpr_uint64(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			return (uint64_t) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
			return 0;
		}
	}

	#undef	_A
}

static uint32_t _get_constexpr_uint32(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (uint32_t) _get_constexpr_uint64(ctx, node);
}

static unsigned int _get_constexpr_uint(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (unsigned int) _get_constexpr_uint64(ctx, node);
}

static unsigned long _get_constexpr_ulong(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return (unsigned long) _get_constexpr_uint64(ctx, node);
}

static float _get_constexpr_float(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			return (float) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		default: {
			assert(0);
			return 0;
		}
	}

	#undef	_A
}

static double _get_constexpr_double(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			return (double) FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		default: {
			assert(0);
			return 0;
		}
	}

	#undef	_A
}

static bool _get_constexpr_bool(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(FE_IS_EXPR_NODE(node->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case FE_TYPE_##type: {	\
			return FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node) != 0;	\
		}

	switch (FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
		_A(CHAR)
		_A(INT8)
		_A(INT16)
		_A(INT32)
		_A(INT64)
		_A(UINT8)
		_A(UINT16)
		_A(UINT32)
		_A(UINT64)
		_A(FLOAT)
		_A(DOUBLE)
		_A(POINTER)
		default: {
			assert(0);
			return 0;
		}
	}

	#undef	_A
}

static uint8_t _get_binary_constexpr_type(
	ParserContext *ctx,
	ParserASTNode *source1,
	ParserASTNode *source2
) {
	assert(ctx);
	assert(source1);
	assert(FE_IS_EXPR_NODE(source1->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(FE_IS_EXPR_NODE(source2->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t source1_type = FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source1);
	int32_t source1_type_weight = _get_primitive_type_weight(source1_type);

	uint8_t source2_type = FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source2);
	int32_t source2_type_weight = _get_primitive_type_weight(source2_type);

	if (_is_signed_type(source1_type) && _is_signed_type(source2_type)) {
		return FE_TYPE_INT64;
	} else if (_is_unsigned_type(source1_type) && _is_unsigned_type(source2_type)) {
		return FE_TYPE_UINT64;
	} else if (_is_signed_type(source1_type) && _is_unsigned_type(source2_type)) {
		if (source1_type_weight > source2_type_weight) {
			return FE_TYPE_INT64;
		} else {
			return FE_TYPE_UINT64;
		}
	} else if (_is_unsigned_type(source1_type) && _is_signed_type(source2_type)) {
		if (source1_type_weight > source2_type_weight) {
			return FE_TYPE_UINT64;
		} else {
			return FE_TYPE_INT64;
		}
	} else if (source1_type == FE_TYPE_DOUBLE
			|| source2_type == FE_TYPE_DOUBLE) {
		return FE_TYPE_DOUBLE;
	} else if (source1_type == FE_TYPE_FLOAT
			|| source2_type == FE_TYPE_FLOAT) {
		return FE_TYPE_FLOAT;
	} else {
		assert(0);
		return FE_TYPE_UNKNOWN;
	}
}

static void _calc_binary_constexpr(
	ParserContext *ctx,
	uint32_t expr_node_type,
	ParserASTNode *target,
	ParserASTNode *source1,
	ParserASTNode *source2
) {
	assert(ctx);
	assert(target);
	assert(source1);
	assert(FE_IS_EXPR_NODE(source1->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(FE_IS_EXPR_NODE(source2->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t target_type = FE_EXPR_AST_NODE_GET_TYPE(target);
	uint8_t source1_type = FE_EXPR_AST_NODE_GET_TYPE(source1);
	uint8_t source1_constant_type = FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source1);
	uint8_t source2_type = FE_EXPR_AST_NODE_GET_TYPE(source2);
	uint8_t source2_constant_type = FE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source2);

	#define	_A(op)	\
		switch (source_type) {	\
			case FE_TYPE_INT64: {	\
				int64_t result = _get_constexpr_int64(ctx, source1) op _get_constexpr_int64(ctx, source2);	\
				_set_constexpr_int64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case FE_TYPE_UINT64: {	\
				uint64_t result = _get_constexpr_uint64(ctx, source1) op _get_constexpr_uint64(ctx, source2);	\
				_set_constexpr_uint64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case FE_TYPE_FLOAT: {	\
				float result = _get_constexpr_float(ctx, source1) op _get_constexpr_float(ctx, source2);	\
				_set_constexpr_float(ctx, target, target_type, result);	\
				break;	\
			}	\
			case FE_TYPE_DOUBLE: {	\
				double result = _get_constexpr_double(ctx, source1) op _get_constexpr_double(ctx, source2);	\
				_set_constexpr_double(ctx, target, target_type, result);	\
				break;	\
			}	\
			default: {	\
				assert(0);	\
				break;	\
			}	\
		}

	#define	_B(type, op)	\
		case type: {	\
			uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);	\
			_A(op)	\
			break;	\
		}

	#define	_A1(op)	\
		switch (source_type) {	\
			case FE_TYPE_INT64: {	\
				int64_t result = _get_constexpr_int64(ctx, source1) op _get_constexpr_int64(ctx, source2);	\
				_set_constexpr_int64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case FE_TYPE_UINT64: {	\
				uint64_t result = _get_constexpr_uint64(ctx, source1) op _get_constexpr_uint64(ctx, source2);	\
				_set_constexpr_uint64(ctx, target, target_type, result);	\
				break;	\
			}	\
			default: {	\
				assert(0);	\
				break;	\
			}	\
		}
	
	#define	_B1(type, op)	\
		case type: {	\
			uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);	\
			_A1(op)	\
			break;	\
		}

	switch (expr_node_type) {
		_B(FE_NODE_EXPR_MUL, *)
		_B(FE_NODE_EXPR_DIV, /)
		case FE_NODE_EXPR_MOD: {
			uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
			switch (source_type) {
				case FE_TYPE_INT64: {
					int64_t result = _get_constexpr_int64(ctx, source1) % _get_constexpr_int64(ctx, source2);
					_set_constexpr_int64(ctx, target, target_type, result);
					break;
				}
				case FE_TYPE_UINT64: {
					uint64_t result = _get_constexpr_uint64(ctx, source1) % _get_constexpr_uint64(ctx, source2);
					_set_constexpr_uint64(ctx, target, target_type, result);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case FE_NODE_EXPR_ADD: {
			if (source1_type == FE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source1,
					FE_EXPR_AST_NODE_GET_TYPE_NODE(source1)
				);
				uint64_t source1_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				if (_is_signed_type(source2_type)) {
					int64_t source2_value = _get_constexpr_int64(ctx, source2);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value + increment_size * source2_value
					);
				} else if (_is_unsigned_type(source2_type)) {
					uint64_t source2_value = _get_constexpr_uint64(ctx, source2);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value + increment_size * source2_value
					);
				} else {
					assert(0);
				}
			} else if (source2_type == FE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source2,
					FE_EXPR_AST_NODE_GET_TYPE_NODE(source2)
				);
				uint64_t source2_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (_is_signed_type(source1_type)) {
					int64_t source1_value =  _get_constexpr_int64(ctx, source1);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source2_value + increment_size * source1_value
					);
				} else if (_is_unsigned_type(source1_type)) {
					uint64_t source1_value =  _get_constexpr_uint64(ctx, source1);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source2_value + increment_size * source1_value
					);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(+)
			}
			break;
		}
		case FE_NODE_EXPR_SUB: {
			if (source1_type == FE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source1,
					FE_EXPR_AST_NODE_GET_TYPE_NODE(source1)
				);
				uint64_t source1_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				if (_is_signed_type(source2_type)) {
					int64_t source2_value = _get_constexpr_int64(ctx, source2);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value - increment_size * source2_value
					);
				} else if (_is_unsigned_type(source2_type)) {
					uint64_t source2_value = _get_constexpr_uint64(ctx, source2);
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value - increment_size * source2_value
					);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(-)
			}
			break;
		}
		_B(FE_NODE_EXPR_LT, <)
		_B(FE_NODE_EXPR_LE, <=)
		_B(FE_NODE_EXPR_GT, >)
		_B(FE_NODE_EXPR_GE, >=)
		case FE_NODE_EXPR_EQ: {
			if (source1_type == FE_TYPE_POINTER
					&& source2_type == FE_TYPE_POINTER) {
				uint64_t source1_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				uint64_t source2_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(target, source1_value == source2_value);
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(target, source1_value == source2_value);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(==)
			}
			break;
		}
		case FE_NODE_EXPR_NEQ: {
			if (source1_type == FE_TYPE_POINTER
					&& source2_type == FE_TYPE_POINTER) {
				uint64_t source1_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				uint64_t source2_value = FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(target, source1_value != source2_value);
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(target, source1_value != source2_value);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(!=)
			}
			break;
		}
		_B1(FE_NODE_EXPR_BAND, &)
		_B1(FE_NODE_EXPR_BXOR, ^)
		_B1(FE_NODE_EXPR_BOR, |)
		_B1(FE_NODE_EXPR_AND, &&)
		_B1(FE_NODE_EXPR_OR, ||)
		default: {
			assert(0);
			break;
		}
	}

	#undef	_A
	#undef	_B
	#undef	_A1
	#undef	_B1
}

static void _calc_shift_constexpr(
	ParserContext *ctx,
	uint32_t expr_node_type,
	ParserASTNode *target,
	ParserASTNode *source1,
	ParserASTNode *source2
) {
	assert(ctx);
	assert(target);
	assert(source1);
	assert(FE_IS_EXPR_NODE(source1->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(FE_IS_EXPR_NODE(source2->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t source1_type = FE_EXPR_AST_NODE_GET_TYPE(source1);
	int64_t source2_value = _get_constexpr_int64(ctx, source2);

	#define	_A(type, op)	\
		case FE_TYPE_##type: {	\
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(	\
				target,	\
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(source1) op source2_value	\
			);	\
			break;	\
		}

	#define	_B(node_type, op)	\
		case node_type: {	\
			switch (source1_type) {	\
				_A(CHAR, op)	\
				_A(INT8, op)	\
				_A(INT16, op)	\
				_A(INT32, op)	\
				_A(INT64, op)	\
				_A(UINT8, op)	\
				_A(UINT16, op)	\
				_A(UINT32, op)	\
				_A(UINT64, op)	\
				default: {	\
					assert(0);	\
					break;	\
				}	\
			}	\
			break;	\
		}

	switch (expr_node_type) {
		_B(FE_NODE_EXPR_SHIFT_LEFT, <<)
		_B(FE_NODE_EXPR_SHIFT_RIGHT, >>)
		default: {
			assert(0);
			break;
		}
	}

	#undef	_A
	#undef	_B
}

static void _calc_cond_constexpr(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *cond,
	ParserASTNode *source1,
	ParserASTNode *source2
) {
	assert(ctx);
	assert(target);
	assert(cond);
	assert(FE_IS_EXPR_NODE(cond->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(cond));
	assert(source1);
	assert(FE_IS_EXPR_NODE(source1->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(FE_IS_EXPR_NODE(source2->type));
	assert(FE_EXPR_AST_NODE_GET_CONSTANT(source2));

	if (_is_primitive_type_node(ctx, source1)) {
		ParserASTNode *source = NULL;

		if (_get_constexpr_bool(ctx, cond)) {
			source = source1;
		} else {
			source = source2;
		}

		_calc_cast_constexpr(
			ctx,
			FE_EXPR_AST_NODE_GET_TYPE(target),
			target,
			source
		);
	} else if (_is_pointer_type_node(ctx, source1)) {
		if (_get_constexpr_bool(ctx, cond)) {
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
				target,
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1)
			);
		} else {
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
				target,
				FE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2)
			);
		}
	} else {
		assert(0);
	}
}




static void _expr_atom(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_EXPR_ATOM);
	assert(node->nchilds == 1);
	
	ParserASTNode *child = node->childs[0];

	switch (child->type) {
		case FE_NODE_LITERAL_UINT: {
			if (ctx->arch == FE_ARCH_32) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT32);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

				uint32_t val = fe_parser_get_uint32_val(ctx, child);
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(node, val);
			} else if (ctx->arch == FE_ARCH_64) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT64);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

				uint64_t val = fe_parser_get_uint64_val(ctx, child);
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(node, val);
			} else {
				assert(0);
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case FE_NODE_LITERAL_REAL: {
			if (ctx->arch == FE_ARCH_32) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_FLOAT);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_FLOAT);

				float val = fe_parser_get_float_val(ctx, child);
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_FLOAT_VAL(node, val);
			} else if (ctx->arch == FE_ARCH_64) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_DOUBLE);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_DOUBLE);

				double val = fe_parser_get_double_val(ctx, child);
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_DOUBLE_VAL(node, val);
			} else {
				assert(0);
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case FE_NODE_LITERAL_CHAR: {
			FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_CHAR);
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_CHAR);

			char val = fe_parser_get_char_val(ctx, child);
			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_CHAR_VAL(node, val);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case FE_NODE_LITERAL_STRING: {
			FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_POINTER);
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_CHAR_POINTER);

			FE_EXPR_AST_NODE_SET_CONSTANT_TYPE(node, FE_TYPE_POINTER);

			FE_EXPR_AST_NODE_SET_HAS_CONSTSTR(node, true);

			FeParserConstexprString *conststr = FE_EXPR_AST_NODE_GET_CONSTSTR(node);
			conststr->str = child->token->content + 1;
			conststr->len = child->token->len - 2;

			FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(node, 0);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case FE_NODE_IDENTIFIER: {
			ParserSymbol *symbol = NULL;
			symbol = parser_get_symbol_from_node(ctx, node, FE_SYM_VAR, child->token);
			if (symbol != NULL) {
				uint8_t type = FE_VAR_SYMBOL_GET_TYPE(symbol);
				ParserASTNode *type_node = FE_VAR_SYMBOL_GET_TYPE_NODE(symbol);

				FE_EXPR_AST_NODE_SET_TYPE(node, type);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, type_node);
			} else {
				symbol = parser_get_symbol_from_node(ctx, node, FE_SYM_FUNC, child->token);
				if (symbol != NULL) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_POINTER);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, FE_FUNC_SYMBOL_GET_FUNC_POINTER_TYPE_NODE(symbol));
				} else {
					_SYNERR_TOKEN(ctx, child->token, "invalid identifier.");
				}
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _expr_parenthesis(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	if (node->type == FE_NODE_EXPR_ATOM) {
		_expr_atom(ctx, node);
	} else {
		_expr(ctx, node);
	}
}

static ParserASTNode * _get_struct_member(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
) {
	assert(ctx);
	assert(node);
	assert(struct_token);
	assert(id_token);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(ctx, node, FE_SYM_STRUCT, struct_token);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, struct_token, "undefined struct.");
	}

	ParserASTNode *struct_body_node = FE_STRUCT_SYMBOL_GET_BODY_NODE(struct_symbol);
	assert(struct_body_node->type == FE_NODE_STRUCT_BODY);
	for (int32_t i = 0; i < struct_body_node->nchilds; i++) {
		ParserASTNode *struct_body_member_node = struct_body_node->childs[i];
		assert(struct_body_member_node->type == FE_NODE_VAR);
		for (int32_t j = 0; j < struct_body_member_node->nchilds; j++) {
			ParserASTNode *var_item_node = struct_body_member_node->childs[j];
			assert(var_item_node->type == FE_NODE_VAR_ITEM);
			if (lexer_compare_token(var_item_node->childs[0]->token, id_token)) {
				return var_item_node;
			}
		}
	}

	_SYNERR_TOKEN(ctx, id_token, "undefined struct member.");

	return NULL;
}

static ParserASTNode * _get_struct_member_type_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
) {
	assert(ctx);
	assert(node);
	assert(struct_token);
	assert(id_token);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(
		ctx, node, FE_SYM_STRUCT, struct_token
	);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, struct_token, "undefined struct.");
	}

	ParserSymbol *struct_member = parser_get_symbol_from_symbol(
		ctx, struct_symbol, FE_SYM_STRUCT_MEMBER_VAR, id_token
	);
	if (struct_member == NULL) {
		_SYNERR_TOKEN(ctx, id_token, "undefined struct member.");
	}

	return FE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE_NODE(struct_member);
}

static int _get_struct_member_index(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
) {
	assert(ctx);
	assert(node);
	assert(struct_token);
	assert(id_token);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(
		ctx, node, FE_SYM_STRUCT, struct_token
	);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, struct_token, "undefined struct.");
	}

	int index = parser_get_symbol_index_from_symbol(
		ctx, struct_symbol, FE_SYM_STRUCT_MEMBER_VAR, id_token	
	);
	if (index < 0) {
		_SYNERR_TOKEN(ctx, id_token, "undefined struct member.");
	}

	return index;
}

static void _check_fncall_params(
	ParserContext *ctx,
	ParserASTNode *node_expr,
	ParserASTNode *node_func_sym_params,
	ParserASTNode *node_func_params
) {
	assert(ctx);
	assert(node_expr);
	assert(node_func_sym_params);
	assert(node_func_params);

	bool func_param_count_matched = false;

	if (node_func_params->nchilds >= node_func_sym_params->nchilds) {
		func_param_count_matched = true;
	}

	if (node_func_sym_params->nchilds > 0
			&& node_func_sym_params->childs[node_func_sym_params->nchilds - 1]->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
			&& node_func_params->nchilds >= node_func_sym_params->nchilds - 1) {
		func_param_count_matched = true;
	}

	if (func_param_count_matched) {
		int i;
		for (i = 0; i < node_func_sym_params->nchilds; i++) {
			ParserASTNode *node_func_sym_param = node_func_sym_params->childs[i];

			if (node_func_sym_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				break;
			}

			ParserASTNode *node_func_param = node_func_params->childs[i];

			_expr_parenthesis(ctx, node_func_param);

			uint8_t param_type = FE_EXPR_AST_NODE_GET_TYPE(node_func_param);
			if (param_type == FE_TYPE_STRUCT
					|| param_type == FE_TYPE_ARRAY
					|| param_type == FE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node_func_param, "parameter type cannot be struct type or array type or function type operand."
				);
			}

			ParserASTNode *node_func_sym_param_type = node_func_sym_param->childs[1];
			ParserASTNode *node_func_param_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_func_param);
			if (!_is_compatible_type_for_assign(ctx, node_func_sym_param_type, node_func_param_type, false)) {
				_SYNERR_NODE(ctx, node_expr, "function call parameter type incompatible.");
			}
		}

		for (; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_func_param = node_func_params->childs[i];
			_expr_parenthesis(ctx, node_func_param);

			uint8_t param_type = FE_EXPR_AST_NODE_GET_TYPE(node_func_param);
			if (param_type == FE_TYPE_STRUCT
					|| param_type == FE_TYPE_ARRAY
					|| param_type == FE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node_func_param, "parameter type cannot be struct type or array type or function type operand."
				);
			}
		}
	} else {
		_SYNERR_NODE(ctx, node_expr, "function call parameter count not matched.");
	}
}

static void _expr_member_arridx_fncall(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case FE_NODE_EXPR_MEMBER: {
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			_expr_parenthesis(ctx, node_expr);

			if (FE_EXPR_AST_NODE_GET_TYPE(node_expr) != FE_TYPE_STRUCT) {
				_SYNERR_NODE(ctx, node_expr, "need struct type expression.");
			}
			ParserASTNode *struct_type_node = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			ParserASTNode *type_node = _get_struct_member_type_node(
				ctx, node, struct_type_node->childs[0]->token, node_id->token
			);
			FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, type_node));
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, type_node);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_PMEMBER: {
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_id = node->childs[1];
			_expr_parenthesis(ctx, node_expr);
			
			if (FE_EXPR_AST_NODE_GET_TYPE(node_expr) != FE_TYPE_POINTER) {
				_SYNERR_NODE(ctx, node_expr, "need struct pointer type expression.");
			}
			ParserASTNode *struct_pointer_type_node = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			assert(struct_pointer_type_node);
			ParserASTNode *struct_type_node = struct_pointer_type_node->childs[0];
			if (_get_type_by_type_node(ctx, struct_type_node) != FE_TYPE_STRUCT) {
				_SYNERR_NODE(ctx, node_expr, "need struct pointer type expression.");
			}
			assert(struct_type_node);
			assert(struct_type_node->nchilds == 1);
			assert(struct_type_node->childs[0]);
			assert(struct_type_node->childs[0]->token);
			assert(node_id->token);

			ParserASTNode *type_node = _get_struct_member_type_node(
				ctx, node, struct_type_node->childs[0]->token, node_id->token
			);
			FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, type_node));
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, type_node);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_ARRAY_INDEX: {
			assert(node->nchilds == 2);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_index = node->childs[1];
			_expr_parenthesis(ctx, node_expr);
			_expr(ctx, node_index);

			uint8_t node_expr_type = FE_EXPR_AST_NODE_GET_TYPE(node_expr);
			if (node_expr_type == FE_TYPE_POINTER) {
				ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
				assert(node_type);
				assert(node_type->nchilds == 1);
				
				FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_type->childs[0]));
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_type->childs[0]);
			} else if (node_expr_type == FE_TYPE_ARRAY) {
				ParserASTNode *node_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
				assert(node_type);
				assert(node_type->nchilds == 2);

				ParserASTNode *node_array_dims = node_type->childs[0];
				ParserASTNode *node_array_type = node_type->childs[1];

				if (node_array_dims->nchilds == 1) {
					FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_array_type));
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_array_type);
				} else if (node_array_dims->nchilds > 1) {
					ParserASTNode *new_node_array_type = _NEW_NODE(ctx, FE_NODE_TYPE_ARRAY, NULL);
					ParserASTNode *new_node_array_dims = _NEW_NODE(ctx, FE_NODE_TYPE_ARRAY_DIMS, NULL);

					for (int32_t i = 1; i < node_array_dims->nchilds; i++) {
						parser_add_child(ctx, new_node_array_dims, node_array_dims->childs[i]);
					}

					parser_add_child(ctx, new_node_array_type, new_node_array_dims);
					parser_add_child(ctx, new_node_array_type, node_array_type);
					FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, new_node_array_type));
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, new_node_array_type);
				} else {
					assert(0);
				}
			} else {
				_SYNERR_NODE(ctx, node_expr, "need pointer type or array type expression.");
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_FUNC_CALL: {
			assert(node->nchilds == 2);
			
			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_func_params = node->childs[1];

			_expr_parenthesis(ctx, node_expr);

			ParserASTNode *node_expr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);

			if (node_expr_type->type == FE_NODE_TYPE_POINTER
					&& node_expr_type->childs[0]->type == FE_NODE_TYPE_FUNC) {
				ParserASTNode *node_expr_params = node_expr_type->childs[0]->childs[0];

				_check_fncall_params(ctx, node_expr, node_expr_params, node_func_params);

				if (node_expr_type->childs[0]->nchilds == 1) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_VOID);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_VOID);
				} else {
					ParserASTNode *node_expr_ret_type = node_expr_type->childs[0]->childs[1];
					FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_expr_ret_type));
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_expr_ret_type);
				}
			} else {
				_SYNERR_NODE(ctx, node_expr, "function call need function pointer type.");
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		default: {
			_expr_parenthesis(ctx, node);
			break;
		}
	}
}

static void _expr_unary_postfix(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_INC_RIGHT:
		case FE_NODE_EXPR_DEC_RIGHT: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_member_arridx_fncall(ctx, node_oprd);

			if (!_expr_is_lvalue(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_INC_RIGHT: {
						_SYNERR_NODE(ctx, node, "unary operator postfix '++' need number left value operand.");
						break;
					}
					case FE_NODE_EXPR_DEC_RIGHT: {
						_SYNERR_NODE(ctx, node, "unary operator postfix '--' need number left value operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			if (!_is_number_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_INC_RIGHT: {
						_SYNERR_NODE(ctx, node, "unary operator postfix '++' need number left value operand.");
						break;
					}
					case FE_NODE_EXPR_DEC_RIGHT: {
						_SYNERR_NODE(ctx, node, "unary operator postfix '--' need number left value operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			FE_EXPR_AST_NODE_COPY(node, node_oprd);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		default: {
			_expr_member_arridx_fncall(ctx, node);
			break;
		}
	}
}

static void _expr_unary(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_NOT:
		case FE_NODE_EXPR_BNOT: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_unary_postfix(ctx, node_oprd);

			if (!_is_integer_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_NOT: {
						_SYNERR_NODE(ctx, node, "unary operator '!' need integer operand.");
						break;
					}
					case FE_NODE_EXPR_BNOT: {
						_SYNERR_NODE(ctx, node, "unary operator '~' need integer operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			FE_EXPR_AST_NODE_COPY(node, node_oprd);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node_oprd)) {
				_calc_unary_constexpr(ctx, node_type, node, node_oprd);
			}
			break;
		}
		case FE_NODE_EXPR_POSITIVE:
		case FE_NODE_EXPR_NEGATIVE: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_unary_postfix(ctx, node_oprd);

			if (!_is_number_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_POSITIVE: {
						_SYNERR_NODE(ctx, node, "unary operator '+' need number operand.");
						break;
					}
					case FE_NODE_EXPR_NEGATIVE: {
						_SYNERR_NODE(ctx, node, "unary operator '-' need number operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			FE_EXPR_AST_NODE_COPY(node, node_oprd);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node_oprd)) {
				FE_EXPR_AST_NODE_COPY_CONSTANT(node, node_oprd);
				_calc_unary_constexpr(ctx, node_type, node, node_oprd);
			}
			break;
		}
		case FE_NODE_EXPR_INC_LEFT:
		case FE_NODE_EXPR_DEC_LEFT: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_unary_postfix(ctx, node_oprd);

			if (!_expr_is_lvalue(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_INC_LEFT: {
						_SYNERR_NODE(ctx, node, "unary operator prefix '++' need number left value operand.");
						break;
					}
					case FE_NODE_EXPR_DEC_LEFT: {
						_SYNERR_NODE(ctx, node, "unary operator prefix '--' need number left value operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			if (!_is_number_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case FE_NODE_EXPR_INC_LEFT: {
						_SYNERR_NODE(ctx, node, "unary operator prefix '++' need number left value operand.");
						break;
					}
					case FE_NODE_EXPR_DEC_LEFT: {
						_SYNERR_NODE(ctx, node, "unary operator prefix '--' need number left value operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			FE_EXPR_AST_NODE_COPY(node, node_oprd);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_SIZEOF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			ParserASTNode *type_node = NULL;
			switch (_get_node_catagory(node_oprd->type)) {
				case FE_SEM_NODE_CATEGORY_TYPE: {
					_type(ctx, node_oprd, NULL);
					type_node = node_oprd;
					break;
				}
				case FE_SEM_NODE_CATAGORY_EXPR: {
					_expr(ctx, node_oprd);
					type_node = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			if (ctx->arch == FE_ARCH_32) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_UINT32);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(
					node, (uint32_t) _calc_type_size(ctx, node, type_node)
				);
			} else if (ctx->arch == FE_ARCH_64) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_UINT64);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);
			
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(
					node, (uint64_t) _calc_type_size(ctx, node, type_node)
				);
			} else {
				assert(0);
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);

			break;
		}
		case FE_NODE_EXPR_ALIGNOF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			ParserASTNode *type_node = NULL;
			switch (_get_node_catagory(node_oprd->type)) {
				case FE_SEM_NODE_CATEGORY_TYPE: {
					_type(ctx, node_oprd, NULL);
					type_node = node_oprd;
					break;
				}
				case FE_SEM_NODE_CATAGORY_EXPR: {
					_expr(ctx, node_oprd);
					type_node = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			size_t align = 0;
			if (node_oprd->type == FE_NODE_EXPR_ATOM
					&& node_oprd->childs[0]->type == FE_NODE_IDENTIFIER) {
				ParserSymbol *symbol = parser_get_symbol_from_node(
					ctx, node, FE_SYM_VAR, node_oprd->childs[0]->token
				);
				if (symbol != NULL) {
					align = FE_VAR_SYMBOL_GET_ALIGN(symbol);
				} else {
					align = _calc_type_align(ctx, node, type_node);
				}
			} else {
				align = _calc_type_align(ctx, node, type_node);
			}

			if (ctx->arch == FE_ARCH_32) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_UINT32);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(
					node, (uint32_t) align
				);
			} else if (ctx->arch == FE_ARCH_64) {
				FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_UINT64);
				FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);
			
				FE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(
					node, (uint64_t) align
				);
			} else {
				assert(0);
			}

			FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case FE_NODE_EXPR_REF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_unary_postfix(ctx, node_oprd);

			if (!_expr_is_lvalue(ctx, node_oprd)) {
				_SYNERR_NODE(ctx, node, "unary operator '&' need left value operand.");
			}

			ParserASTNode *node_oprd_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);

			ParserASTNode *node_pointer_type = _NEW_NODE(ctx, FE_NODE_TYPE_POINTER, NULL);
			parser_add_child(ctx, node_pointer_type, node_oprd_type);

			FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_POINTER);
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_pointer_type);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_DEREF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_unary_postfix(ctx, node_oprd);

			ParserASTNode *node_oprd_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);
			if (node_oprd_type->type != FE_NODE_TYPE_POINTER) {
				_SYNERR_NODE(ctx, node, "unary operator '*' need pointer type operand.");
			}

			ParserASTNode *node_oprd_child_type = node_oprd_type->childs[0];
			if (node_oprd_child_type->type == FE_NODE_TYPE_VOID) {
				_SYNERR_NODE(ctx, node, "unary operator '*' operand type cannot be void pointer type.");
			}

			FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_oprd_child_type));
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_oprd_child_type);

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);
			break;
		}
		case FE_NODE_EXPR_CAST: {
			assert(node->nchilds == 2);

			ParserASTNode *node_type = node->childs[0];
			ParserASTNode *node_expr = node->childs[1];

			_type(ctx, node_type, NULL);
			_expr(ctx, node_expr);

			FE_EXPR_AST_NODE_COPY(node, node_expr);

			uint8_t node_type_type = _get_type_by_type_node(ctx, node_type);
			if (node_type_type == FE_TYPE_STRUCT
					|| node_type_type == FE_TYPE_ARRAY
					|| node_type_type == FE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node, "operator cast cannot cast operand to struct type or array type or function type."
				);
			}

			ParserASTNode *node_expr_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			uint8_t node_expr_type_type = _get_type_by_type_node(ctx, node_expr_type);
			if (node_expr_type_type == FE_TYPE_STRUCT
					|| node_expr_type_type == FE_TYPE_ARRAY
					|| node_expr_type_type == FE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node, "operator cast cannot cast struct type or array type or function type operand."
				);
			}

			FE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_type));
			FE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_type);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node_expr)) {
				FE_EXPR_AST_NODE_SET_CONSTANT(node, true);
				_calc_cast_constexpr(ctx, node_type_type, node, node_expr);
			}
			break;
		}
		default: {
			_expr_unary_postfix(ctx, node);
			break;
		}
	}
}

static void _check_expr_mul_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (!_is_number_type_node(ctx, node_oprd1)
			|| !_is_number_type_node(ctx, node_oprd2)) {
		_SYNERR_NODE(
			ctx, node, "binary operator '+' and '-' need number type operand."
		);
	}

	if (set_type) {
		int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
		int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
		if (weight_oprd1_type > weight_oprd2_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		} else {
			FE_EXPR_AST_NODE_COPY(node, node_oprd2);
		}
	}
}

static void _check_expr_mod_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (!_is_integer_type_node(ctx, node_oprd1)
			|| !_is_integer_type_node(ctx, node_oprd2)) {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' need number type operand.",
			operator
		);
	}

	if (set_type) {
		int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
		int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
		if (weight_oprd1_type > weight_oprd2_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		} else {
			FE_EXPR_AST_NODE_COPY(node, node_oprd2);
		}
	}
}

static void _expr_mul(
	ParserContext *ctx,
	ParserASTNode *node
)  {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_MUL:
		case FE_NODE_EXPR_DIV: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == FE_NODE_EXPR_MUL) {
				operator = "*";
			} else if (node_type == FE_NODE_EXPR_DIV) {
				operator = "/";
			} else {
				assert(0);
			}

			_check_expr_mul_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		case FE_NODE_EXPR_MOD: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_mod_operands(
				ctx, node, node_oprd1, node_oprd2, "%", true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_unary(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _check_expr_add_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_pointer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else if (_is_number_type_node(ctx, node_oprd1) && _is_number_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				FE_EXPR_AST_NODE_COPY(node, node_oprd2);
			}
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _check_expr_sub_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_pointer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else if (_is_number_type_node(ctx, node_oprd1) && _is_number_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				FE_EXPR_AST_NODE_COPY(node, node_oprd2);
			}
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_add(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_ADD: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_add_operands(
				ctx, node, node_oprd1, node_oprd2, "+", true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		case FE_NODE_EXPR_SUB: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_sub_operands(
				ctx, node, node_oprd1, node_oprd2, "-", true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_mul(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _check_expr_shift_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_integer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_shift(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_SHIFT_LEFT:
		case FE_NODE_EXPR_SHIFT_RIGHT: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_add(ctx, node_oprd1);
			_expr_add(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == FE_NODE_EXPR_SHIFT_LEFT) {
				operator = "<<";
			} else if (node_type == FE_NODE_EXPR_SHIFT_RIGHT) {
				operator = ">>";
			} else {
				assert(0);
			}

			_check_expr_shift_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_shift_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_add(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr_lt(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_LT:
		case FE_NODE_EXPR_LE:
		case FE_NODE_EXPR_GT:
		case FE_NODE_EXPR_GE: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_shift(ctx, node_oprd1);
			_expr_shift(ctx, node_oprd2);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT32);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT64);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else {
					assert(0);
				}
			} else {
				_SYNERR_NODE(ctx, node, "binary operator '<' and '<=' and '>' and '>=' do not support this operand combination.");
			}

			break;
		}
		default: {
			_expr_shift(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr_eq(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_EQ:
		case FE_NODE_EXPR_NEQ: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_lt(ctx, node_oprd1);
			_expr_lt(ctx, node_oprd2);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if ((_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2))
					|| (_is_pointer_type_node(ctx, node_oprd1) && _is_pointer_type_node(ctx, node_oprd2))) {
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT32);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT64);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else {
					assert(0);
				}
			} else {
				_SYNERR_NODE(ctx, node, "binary operator '==' and '!=' do not support this operand combination.");
			}

			break;
		}
		default: {
			_expr_lt(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _check_expr_band_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_integer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				FE_EXPR_AST_NODE_COPY(node, node_oprd2);
			}
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_band(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_BAND: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_eq(ctx, node_oprd1);
			_expr_eq(ctx, node_oprd2);

			const char *operator = "&";

			_check_expr_band_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_eq(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _check_expr_bxor_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_integer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				FE_EXPR_AST_NODE_COPY(node, node_oprd2);
			}
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_bxor(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_BXOR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_band(ctx, node_oprd1);
			_expr_band(ctx, node_oprd2);

			const char *operator = "^";

			_check_expr_bxor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_band(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _check_expr_bor_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	if (_is_integer_type_node(ctx, node_oprd1) && _is_integer_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				FE_EXPR_AST_NODE_COPY(node, node_oprd2);
			}
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_bor(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_BOR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_bxor(ctx, node_oprd1);
			_expr_bxor(ctx, node_oprd2);

			const char *operator = "|";

			_check_expr_bor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		default: {
			_expr_bxor(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr_and(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_AND: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_bor(ctx, node_oprd1);
			_expr_bor(ctx, node_oprd2);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT32);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT64);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else {
					assert(0);
				}
			} else {
				_SYNERR_NODE(ctx, node, "binary operator '&&' do not support this operand combination.");
			}

			break;
		}
		default: {
			_expr_bor(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr_or(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_OR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_and(ctx, node_oprd1);
			_expr_and(ctx, node_oprd2);

			FE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == FE_ARCH_32) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT32);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == FE_ARCH_64) {
					FE_EXPR_AST_NODE_SET_TYPE(node, FE_TYPE_INT64);
					FE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else {
					assert(0);
				}
			} else {
				_SYNERR_NODE(ctx, node, "binary operator '||' do not support this operand combination.");
			}

			break;
		}
		default: {
			_expr_and(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr_cond(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_COND: {
			assert(node->nchilds == 3);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_expr_true = node->childs[1];
			ParserASTNode *node_expr_false = node->childs[2];

			_expr_or(ctx, node_expr);
			_expr(ctx, node_expr_true);
			_expr(ctx, node_expr_false);

			ParserASTNode *node_expr_true_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_true);
			ParserASTNode *node_expr_false_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_false);

			if (!_is_integer_type_node(ctx, node_expr)
					&& !_is_pointer_type_node(ctx, node_expr)) {
				_SYNERR_NODE(
					ctx,
					node,
					"conditional operator condexpr type must be primitive type or pointer type."
				);
			}

			if (_is_primitive_type_node(ctx, node_expr_true)
					&& _is_compatible_type(ctx, node_expr_true_type, node_expr_false_type, false)) {
				int32_t weight_node_expr_true = _get_primitive_type_weight_by_expr_node(ctx, node_expr_true);
				int32_t weight_node_expr_false = _get_primitive_type_weight_by_expr_node(ctx, node_expr_false);
				if (weight_node_expr_true > weight_node_expr_false) {
					FE_EXPR_AST_NODE_COPY(node, node_expr_true);
				} else {
					FE_EXPR_AST_NODE_COPY(node, node_expr_false);
				}
			} else if (_is_pointer_type_node(ctx, node_expr_true)
					&& _is_compatible_type(ctx, node_expr_true_type, node_expr_false_type, true)) {
				FE_EXPR_AST_NODE_COPY(node, node_expr_true);
			} else {
				_SYNERR_NODE(
					ctx,
					node,
					"conditional operator do not support this operand combination."
				);
			}

			FE_EXPR_AST_NODE_COPY_CONSTANT3(
				node,
				node_expr,
				node_expr_true,
				node_expr_false
			);
			if (FE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_cond_constexpr(ctx, node, node_expr, node_expr_true, node_expr_false);
			}

			break;
		}
		default: {
			_expr_or(ctx, node);
			break;
		}
	}
}

static void _check_expr_assign_operands(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_oprd1,
	ParserASTNode *node_oprd2,
	const char *operator,
	bool set_type
) {
	assert(ctx);
	assert(node);
	assert(node_oprd1);
	assert(node_oprd2);
	assert(operator);

	ParserASTNode *node_oprd1_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd1);
	ParserASTNode *node_oprd2_type = FE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd2);
	if (_is_compatible_type_for_assign(ctx, node_oprd1_type, node_oprd2_type, false)) {
		if (set_type) {
			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else {
		_SYNERR_NODE(
			ctx,
			node,
			"binary operator '%s' do not support this operand combination.",
			operator
		);
	}
}

static void _expr_assign(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	#define	_CHECK	\
	{	\
		assert(node->nchilds == 2);	\
		if (!_expr_is_lvalue(ctx, node->childs[0])) {	\
			_SYNERR_NODE(ctx, node, "invalid left value.");	\
		}	\
	}

	uint32_t node_type = node->type;

	switch (node_type) {
		case FE_NODE_EXPR_ASSIGN: {
			_CHECK

			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			_check_expr_assign_operands(
				ctx, node, node_oprd1, node_oprd2, "=", true
			);

			break;
		}
		case FE_NODE_EXPR_ADD_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			_check_expr_add_operands(
				ctx, node, node_oprd1, node_oprd2, "+=", false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_SUB_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			_check_expr_sub_operands(
				ctx, node, node_oprd1, node_oprd2, "-=", false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_MUL_ASSIGN:
		case FE_NODE_EXPR_DIV_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == FE_NODE_EXPR_MUL_ASSIGN) {
				operator = "*=";
			} else if (node_type == FE_NODE_EXPR_DIV_ASSIGN) {
				operator = "/=";
			} else {
				assert(0);
			}

			_check_expr_mul_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_MOD_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = "%";

			_check_expr_mod_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_BAND_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = "&=";

			_check_expr_band_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_BOR_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = "|=";

			_check_expr_bor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_BXOR_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = "^=";

			_check_expr_bxor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		case FE_NODE_EXPR_SHIFT_LEFT_ASSIGN:
		case FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN: {
			_CHECK

			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_cond(ctx, node_oprd1);
			_expr(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == FE_NODE_EXPR_SHIFT_LEFT_ASSIGN) {
				operator = "<<=";
			} else if (node_type == FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN) {
				operator = ">>=";
			} else {
				assert(0);
			}

			_check_expr_shift_operands(
				ctx, node, node_oprd1, node_oprd2, operator, false
			);

			FE_EXPR_AST_NODE_COPY(node, node_oprd1);
			FE_EXPR_AST_NODE_SET_CONSTANT(node, false);

			break;
		}
		default: {
			_expr_cond(ctx, node);
			break;
		}
	}

	#undef	_CHECK
}

static void _expr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	_expr_assign(ctx, node);
}

static void _expr_wrapper(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_EXPR);

	ParserASTNode *child = node->childs[0];

	_expr(ctx, child);

	FE_EXPR_AST_NODE_COPY(node, child);
}

static void _stat_expr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_EXPR);

	_expr_wrapper(ctx, node);
}

static void _stat(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case FE_NODE_VAR: {
			_stat_var(ctx, node);
			break;
		}
		case FE_NODE_STAT_IF: {
			_stat_if(ctx, node);
			break;
		}
		case FE_NODE_STAT_WHILE: {
			_stat_while(ctx, node);
			break;
		}
		case FE_NODE_STAT_DO: {
			_stat_do(ctx, node);
			break;
		}
		case FE_NODE_STAT_FOR: {
			_stat_for(ctx, node);
			break;
		}
		case FE_NODE_STAT_BREAK: {
			_stat_break(ctx, node);
			break;
		}
		case FE_NODE_STAT_CONTINUE: {
			_stat_continue(ctx, node);
			break;
		}
		case FE_NODE_STAT_RETURN: {
			_stat_return(ctx, node);
			break;
		}
		case FE_NODE_STAT_GOTO: {
			_stat_goto(ctx, node);			
			break;
		}
		case FE_NODE_STAT_LABEL: {
			_stat_label(ctx, node);
			break;
		}
		case FE_NODE_STAT_ASM: {
			_stat_asm(ctx, node);
			break;
		}
		case FE_NODE_EXPR: {
			_stat_expr(ctx, node);
			break;
		}
		case FE_NODE_STAT_DUMMY: {
			_stat_dummy(ctx, node);
			break;
		}
		case FE_NODE_STAT_VA_START: {
			_stat_va_start(ctx, node);
			break;
		}
		case FE_NODE_STAT_VA_END: {
			_stat_va_end(ctx, node);
			break;
		}
		case FE_NODE_STAT_VA_COPY: {
			_stat_va_copy(ctx, node);
			break;
		}
		case FE_NODE_STATS_BLOCK: {
			_stats_block(ctx, node);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _stats_block(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STATS_BLOCK);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *child = node->childs[i];

		_stat(ctx, child);
	}
}

static void _process_struct_member(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_struct_body,
	ParserSymbol *symbol_struct
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);
	assert(node->nchilds == 2);
	assert(node_struct_body);
	assert(node_struct_body->type == FE_NODE_STRUCT_BODY);
	assert(symbol_struct);

	for (int i = 0; i < node_struct_body->nchilds; i++) {
		ParserASTNode *child = node_struct_body->childs[i];
		assert(child);
		if (child->type == FE_NODE_VAR) {
			for (int j = 0; j < child->nchilds; j++) {
				ParserASTNode *node_var_item = child->childs[j];
				assert(node_var_item->type == FE_NODE_VAR_ITEM);
				
				if (node_var_item->nchilds == 2) {
					ParserASTNode *node_var_id = node_var_item->childs[0];
					assert(node_var_id->type == FE_NODE_IDENTIFIER);

					ParserASTNode *node_var_type = node_var_item->childs[1];
					uint8_t var_type = 0;
					_type(ctx, node_var_type, &var_type);

					fe_parser_add_struct_member_var_symbol_to_symbol(
						ctx,
						symbol_struct,
						node_var_id->token,
						var_type,
						node_var_type
					);
				} else if (node_var_item->nchilds == 3) {
					_SYNERR_NODE(
						ctx,
						node,
						"struct variable cannot contain initial expression."
					);
				} else {
					assert(0);
				}
			}
		} else {
			assert(0);
		}
	}
}

static void _struct(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STRUCT);
	assert(node->nchilds == 2);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == FE_NODE_IDENTIFIER);

	ParserASTNode *node_struct_body = node->childs[1];
	assert(node_struct_body->type == FE_NODE_STRUCT_BODY
			|| node_struct_body->type == FE_NODE_DUMMY);

	ParserASTNode *node_parent = node->parent;
	assert(node_parent);

	if (node_struct_body->type == FE_NODE_DUMMY) {
		fe_parser_add_struct_symbol_to_node(
			ctx, node_parent, node_identifier->token, node_struct_body, true
		);
	} else {
		ParserSymbol *symbol = parser_get_symbol_from_node(
			ctx, node_parent, FE_SYM_STRUCT, node_identifier->token
		);

		if (symbol != NULL
				&& FE_STRUCT_SYMBOL_GET_DUMMY(symbol)) {
			_process_struct_member(ctx, node, node_struct_body, symbol);

			FE_STRUCT_SYMBOL_SET_DUMMY(symbol, false);
			FE_STRUCT_SYMBOL_SET_BODY_NODE(symbol, node_struct_body);
		} else {
			ParserSymbol *symbol_struct = fe_parser_new_struct_symbol(
				ctx, node_identifier->token, node, node_struct_body, false
			);

			_process_struct_member(ctx, node, node_struct_body, symbol_struct);

			parser_add_symptr_to_node(ctx, node_parent, symbol_struct);
		}
	}
}

static void __func_process_label(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *current
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STATS_BLOCK);
	assert(current);

	if (current->type == FE_NODE_STAT_LABEL) {
		parser_add_symbol_to_node(
			ctx,
			node,
			FE_SYM_LABEL,
			current->childs[0]->token,
			0,
			NULL
		);
	}

	for (int i = 0; i < current->nchilds; i++) {
		ParserASTNode *child = current->childs[i];
		__func_process_label(ctx, node, child);
	}
}

static void _func_process_label(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_STATS_BLOCK);

	__func_process_label(ctx, node, node);
}

static void _func(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_FUNC);

	ParserASTNode *node_identifier = NULL;
	ParserASTNode *node_func_params = NULL;
	ParserASTNode *node_type = NULL;
	ParserASTNode *node_body = NULL;

	if (node->nchilds == 3) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_body = node->childs[2];
	} else if (node->nchilds == 4) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_type = node->childs[2];
		node_body = node->childs[3];
	} else {
		assert(0);
	}

	if (node_func_params->nchilds >= 1
			&& node_func_params->childs[0]->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
		_SYNERR_NODE(
			ctx,
			node_func_params->childs[0],
			"first function parameter cannot be '...'."
		);
	}

	for (int i = 0; i < node_func_params->nchilds; i++) {
		ParserASTNode *node_func_param = node_func_params->childs[i];
		if (node_func_param->type == FE_NODE_FUNC_PARAMS_ITEM) {
			ParserASTNode *node_func_param_id = node_func_param->childs[0];
			ParserASTNode *node_func_param_type = node_func_param->childs[1];

			uint8_t param_type = 0;
			_type(ctx, node_func_param_type, &param_type);
			if (!_is_primitive_type(param_type)
					&& !_is_pointer_type(param_type)) {
				_SYNERR_NODE(
					ctx,
					node_func_param_type,
					"function parameter type must be primitive type or pointer type."
				);
			}

			ParserSymbol *symbol = fe_parser_add_var_symbol_to_node(
				ctx,
				node,
				node_func_param_id->token,
				param_type,
				node_func_param_type
			);

			FE_FUNC_PARAM_AST_NODE_SET_SYMBOL(node_func_param, symbol);
		}
	}

	if (node_type != NULL) {
		uint8_t return_type = 0;
		_type(ctx, node_type, &return_type);
		if (!_is_primitive_type(return_type)
				&& !_is_pointer_type(return_type)) {
			_SYNERR_NODE(
				ctx,
				node_type,
				"function return type must be primitive type or pointer type."
			);
		}
	}

	ParserASTNode *node_parent = node->parent;
	assert(node_parent);

	ParserSymbol *symbol = NULL;
	if (node_body->type == FE_NODE_DUMMY) {
		symbol = fe_parser_add_func_symbol_to_node(
			ctx, node_parent, node_identifier->token, node_func_params, node_type, true
		);

		FE_FUNC_AST_NODE_SET_SYMBOL(node, symbol);
	} else {
		symbol = parser_get_symbol_from_node(
			ctx, node_parent, FE_SYM_FUNC, node_identifier->token
		);

		if (symbol != NULL
				&& FE_FUNC_SYMBOL_GET_DUMMY(symbol)) {
			ParserASTNode *func_type_node = _NEW_NODE(ctx, FE_NODE_TYPE_FUNC, NULL);
			parser_add_child(ctx, func_type_node, node_func_params);
			if (node_type != NULL) {
				parser_add_child(ctx, func_type_node, node_type);
			}

			if (!_is_compatible_func_type(ctx, func_type_node, FE_FUNC_SYMBOL_GET_FUNC_TYPE_NODE(symbol), true)) {
				_SYNERR_TOKEN(
					ctx,
					node_identifier->token,
					"function declaration signature and function implementation signature are not matched."
				);
			}

			FE_FUNC_SYMBOL_SET_DUMMY(symbol, false);
		} else {
			symbol = fe_parser_add_func_symbol_to_node(
				ctx, node_parent, node_identifier->token, node_func_params, node_type, false
			);
		}

		assert(symbol);
		FE_FUNC_AST_NODE_SET_SYMBOL(node, symbol);

		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_func_param = node_func_params->childs[i];
			if (node_func_param->type == FE_NODE_FUNC_PARAMS_ITEM) {
				FE_VAR_SYMBOL_SET_FUNC_SYMBOL(
					FE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_func_param),
					symbol
				);
			}
		}

		_func_process_label(ctx, node_body);

		_stats_block(ctx, node_body);
	}
}

static void _module_item(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case FE_NODE_VAR: {
			_var(ctx, node);
			break;
		}
		case FE_NODE_STRUCT: {
			_struct(ctx, node);
			break;
		}
		case FE_NODE_FUNC: {
			_func(ctx, node);
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

void fe_sem_process(
	ParserContext *ctx
) {
	assert(ctx);
	assert(ctx->ast);

	ParserASTNode *node_module = ctx->ast;
	_module(ctx, node_module);
}

int fe_sem_get_struct_member_index(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
) {
	return _get_struct_member_index(ctx, node, struct_token, id_token);
}

ParserASTNode * fe_sem_get_struct_member_type_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *struct_token,
	LexerToken *id_token
) {
	return _get_struct_member_type_node(ctx, node, struct_token, id_token);
}

int32_t fe_sem_get_primitive_type_weight(
	uint8_t type
) {
	return _get_primitive_type_weight(type);
}

size_t fe_sem_get_primitive_type_size(
	uint8_t type
) {
	return _get_primitive_type_size(type);
}

bool fe_sem_is_signed_type(
	uint8_t type
) {
	return _is_signed_type(type);
}

bool fe_sem_is_unsigned_type(
	uint8_t type
) {
	return _is_unsigned_type(type);
}

bool fe_sem_is_integer_type(
	uint8_t type
) {
	return _is_integer_type(type);
}

bool fe_sem_is_float_type(
	uint8_t type
) {
	return _is_float_type(type);
}

bool fe_sem_is_primitive_type(
	uint8_t type
) {
	return _is_primitive_type(type);
}

bool fe_sem_is_pointer_type(
	uint8_t type
) {
	return _is_pointer_type(type);
}

bool fe_sem_is_number_type(
	uint8_t type
) {
	return _is_number_type(type);
}

bool fe_sem_is_compatible_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	return _is_compatible_type(ctx, target, source, same);
}

uint8_t fe_sem_get_type_by_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	return _get_type_by_type_node(ctx, node);
}

ParserASTNode * fe_sem_search_node_along_parent(
	ParserASTNode *node,
	...
) {
	assert(node);

	va_list arg_ptr;
	va_start(arg_ptr, node);
	
	return __search_node_along_parent(node, arg_ptr);
}




static void _print_type_node(ParserASTNode *node, FILE *file);

static void _print_func_params(ParserASTNode *node, FILE *file) {
	assert(node);
	assert(file);

	if (node->nchilds > 0) {
		ParserASTNode *node_param = node->childs[0];
		assert(node_param->type == FE_NODE_FUNC_PARAMS_ITEM);

		#define _NODE_PARAM	\
			switch (node_param->type) {	\
				case FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {	\
					fputs("...", file);	\
					break;	\
				}	\
				case FE_NODE_FUNC_PARAMS_ITEM: {	\
					assert(node_param->nchilds == 2);	\
					ParserASTNode *node_id = node_param->childs[0];	\
					assert(node_id->type == FE_NODE_IDENTIFIER);	\
					ParserASTNode *node_t = node_param->childs[1];	\
						\
					cmn_print_token(node_id->token, file);	\
						\
					fputs(":", file);	\
						\
					_print_type_node(node_t, file);	\
					break;	\
				}	\
				default: {	\
					assert(0);	\
					break;	\
				}	\
			}

		_NODE_PARAM

		for (int i = 1; i < node->nchilds; i++) {
			node_param = node->childs[i];
			assert(node_param->type == FE_NODE_FUNC_PARAMS_ITEM
					|| node_param->type == FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM);

			fputs(", ", file);

			_NODE_PARAM
		}

		#undef	_NODE_PARAM
	}
}

static void _print_type_node(ParserASTNode *node, FILE *file) {
	assert(node);
	assert(file);

	switch (node->type) {
		case FE_NODE_TYPE_CHAR: {
			assert(node->nchilds == 0);
			fputs("char", file);
			break;
		}
		case FE_NODE_TYPE_INT8: {
			assert(node->nchilds == 0);
			fputs("int8", file);
			break;
		}
		case FE_NODE_TYPE_UINT8: {
			assert(node->nchilds == 0);
			fputs("uint8", file);
			break;
		}
		case FE_NODE_TYPE_INT16: {
			assert(node->nchilds == 0);
			fputs("int16", file);
			break;
		}
		case FE_NODE_TYPE_UINT16: {
			assert(node->nchilds == 0);
			fputs("uint16", file);
			break;
		}
		case FE_NODE_TYPE_INT32: {
			assert(node->nchilds == 0);
			fputs("int32", file);
			break;
		}
		case FE_NODE_TYPE_UINT32: {
			assert(node->nchilds == 0);
			fputs("uint32", file);
			break;
		}
		case FE_NODE_TYPE_INT64: {
			assert(node->nchilds == 0);
			fputs("int64", file);
			break;
		}
		case FE_NODE_TYPE_UINT64: {
			assert(node->nchilds == 0);
			fputs("uint64", file);
			break;
		}
		case FE_NODE_TYPE_FLOAT: {
			assert(node->nchilds == 0);
			fputs("float", file);
			break;
		}
		case FE_NODE_TYPE_DOUBLE: {
			assert(node->nchilds == 0);
			fputs("double", file);
			break;
		}
		case FE_NODE_TYPE_STRUCT: {
			assert(node->nchilds == 1);

			fputs("struct ", file);

			ParserASTNode *node_id = node->childs[0];
			assert(node_id->type == FE_NODE_IDENTIFIER);

			cmn_print_token(node_id->token, file);
			break;
		}
		case FE_NODE_TYPE_ARRAY: {
			assert(node->nchilds == 2);
			ParserASTNode *node_array_dims = node->childs[0];
			assert(node_array_dims->type == FE_NODE_TYPE_ARRAY_DIMS);
			ParserASTNode *node_t = node->childs[1];

			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == FE_NODE_LITERAL_UINT);
				fputc('[', file);
				cmn_print_token(node_array_dim->token, file);
				fputc(']', file);
			}

			_print_type_node(node_t, file);

			break;
		}
		case FE_NODE_TYPE_FUNC: {
			assert(node->nchilds == 1 || node->nchilds == 2);
			ParserASTNode *node_params = NULL;
			ParserASTNode *node_t = NULL;

			node_params = node->childs[0];
			if (node->nchilds == 2) {
				node_t = node->childs[1];
			}

			_print_func_params(node_params, file);

			if (node_t != NULL) {
				fputc(':', file);
				_print_type_node(node_t, file);
			}
			break;
		}
		case FE_NODE_TYPE_POINTER: {
			assert(node->nchilds == 1);
			ParserASTNode *node_t = node->childs[0];

			fputc('*', file);

			_print_type_node(node_t, file);
			break;
		}
		case FE_NODE_TYPE_VOID: {
			assert(node->nchilds == 0);
			fputs("void", file);
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _print_ast(ParserASTNode *node, FILE *file, int level, char padchr) {
	assert(node);
	assert(file);
	assert(level >= 0);

	for (int i = 0; i < level; i++) {
		fputc(padchr, file);
	}
	fprintf(
		file,
		"@%s#%x#%p#%p",
		node->type_name != NULL ? node->type_name : "N/A",
		node->type,
		node->parent,
		node
	);
	if (node->token != NULL) {
		fputc('(', file);
		for (int i = 0; i < node->token->len; i++) {
			fputc(*(node->token->content + i), file);
		}
		fprintf(file, "#%x)", node->token->type);
	}
	fputc('\n', file);

	if (FE_IS_EXPR_NODE(node->type)) {
		for (int i = 0; i < level; i++) {
			fputc(padchr, file);
		}
		fputs("|ExprNodeType: ", file);
		ParserASTNode *node_t = FE_EXPR_AST_NODE_GET_TYPE_NODE(node);
		if (node_t != NULL) {
			_print_type_node(node_t, file);
		}
		fputc('\n', file);
	}

	for (int i = 0; i < node->nchilds; i++) {
		_print_ast(node->childs[i], file, level + 1, padchr);
	}
}

void fe_sem_parser_print_ast(
	ParserContext * ctx,
	FILE *file
) {
	assert(ctx);
	assert(ctx->ast);
	assert(file);

	_print_ast(ctx->ast, file, 0, '\t');
}
