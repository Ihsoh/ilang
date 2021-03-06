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

_NODE_TYPE(char, BE_NODE_TYPE_CHAR)
_NODE_TYPE(int8, BE_NODE_TYPE_INT8)
_NODE_TYPE(int16, BE_NODE_TYPE_INT16)
_NODE_TYPE(int32, BE_NODE_TYPE_INT32)
_NODE_TYPE(int64, BE_NODE_TYPE_INT64)
_NODE_TYPE(uint8, BE_NODE_TYPE_UINT8)
_NODE_TYPE(uint16, BE_NODE_TYPE_UINT16)
_NODE_TYPE(uint32, BE_NODE_TYPE_UINT32)
_NODE_TYPE(uint64, BE_NODE_TYPE_UINT64)
_NODE_TYPE(float, BE_NODE_TYPE_FLOAT)
_NODE_TYPE(double, BE_NODE_TYPE_DOUBLE)
_NODE_TYPE(struct, BE_NODE_TYPE_STRUCT)
_NODE_TYPE(array, BE_NODE_TYPE_ARRAY)
_NODE_TYPE(func, BE_NODE_TYPE_FUNC)
_NODE_TYPE(pointer, BE_NODE_TYPE_POINTER)
_NODE_TYPE(void, BE_NODE_TYPE_VOID)

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
	.type = BE_NODE_TYPE_POINTER,
	.type_name = "BE_NODE_TYPE_POINTER",

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
	if (BE_IS_TYPE_NODE(node_type)) {
		return BE_SEM_NODE_CATEGORY_TYPE;
	} else if (BE_IS_EXPR_NODE(node_type)) {
		return BE_SEM_NODE_CATAGORY_EXPR;
	} else {
		return BE_SEM_NODE_CATEGORY_UNKNOWN;
	}
}

static uint8_t _get_type_by_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case BE_NODE_TYPE_CHAR: {
			return BE_TYPE_CHAR;
		}
		case BE_NODE_TYPE_INT8: {
			return BE_TYPE_INT8;
		}
		case BE_NODE_TYPE_UINT8: {
			return BE_TYPE_UINT8;
		}
		case BE_NODE_TYPE_INT16: {
			return BE_TYPE_INT16;
		}
		case BE_NODE_TYPE_UINT16: {
			return BE_TYPE_UINT16;
		}
		case BE_NODE_TYPE_INT32: {
			return BE_TYPE_INT32;
		}
		case BE_NODE_TYPE_UINT32: {
			return BE_TYPE_UINT32;
		}
		case BE_NODE_TYPE_INT64: {
			return BE_TYPE_INT64;
		}
		case BE_NODE_TYPE_UINT64: {
			return BE_TYPE_UINT64;
		}
		case BE_NODE_TYPE_FLOAT: {
			return BE_TYPE_FLOAT;
		}
		case BE_NODE_TYPE_DOUBLE: {
			return BE_TYPE_DOUBLE;
		}
		case BE_NODE_TYPE_VA_LIST: {
			return BE_TYPE_VA_LIST;
		}
		case BE_NODE_TYPE_STRUCT: {
			return BE_TYPE_STRUCT;
		}
		case BE_NODE_TYPE_ARRAY: {
			return BE_TYPE_ARRAY;
		}
		case BE_NODE_TYPE_FUNC: {
			return BE_TYPE_FUNC;
		}
		case BE_NODE_TYPE_POINTER: {
			return BE_TYPE_POINTER;
		}
		case BE_NODE_TYPE_VOID: {
			return BE_TYPE_VOID;
		}
		default: {
			assert(0);
		}
	}

	return BE_TYPE_UNKNOWN;
}

static size_t _get_primitive_type_size(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR: {
			return 1;
		}
		case BE_TYPE_INT8: {
			return 1;
		}
		case BE_TYPE_UINT8: {
			return 1;
		}
		case BE_TYPE_INT16: {
			return 2;
		}
		case BE_TYPE_UINT16: {
			return 2;
		}
		case BE_TYPE_INT32: {
			return 4;
		}
		case BE_TYPE_UINT32: {
			return 4;
		}
		case BE_TYPE_INT64: {
			return 8;
		}
		case BE_TYPE_UINT64: {
			return 8;
		}
		case BE_TYPE_FLOAT: {
			return 4;
		}
		case BE_TYPE_DOUBLE: {
			return 8;
		}
		default: {
			assert(0);
		}
	}
}

static size_t _get_type_size(
	uint8_t type
) {
	if (type == BE_TYPE_POINTER) {
		return 8;
	} else {
		return _get_primitive_type_size(type);
	}
}

static int32_t _get_primitive_type_weight(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR: {
			return 1;
		}
		case BE_TYPE_INT8: {
			return 2;
		}
		case BE_TYPE_UINT8: {
			return 3;
		}
		case BE_TYPE_INT16: {
			return 4;
		}
		case BE_TYPE_UINT16: {
			return 5;
		}
		case BE_TYPE_INT32: {
			return 6;
		}
		case BE_TYPE_UINT32: {
			return 7;
		}
		case BE_TYPE_INT64: {
			return 8;
		}
		case BE_TYPE_UINT64: {
			return 9;
		}
		case BE_TYPE_FLOAT: {
			return 10;
		}
		case BE_TYPE_DOUBLE: {
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
		BE_EXPR_AST_NODE_GET_TYPE(node)
	);
}

static bool _is_unsigned_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_UINT8:
		case BE_TYPE_UINT16:
		case BE_TYPE_UINT32:
		case BE_TYPE_UINT64: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_unsigned_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_signed_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_INT16:
		case BE_TYPE_INT32:
		case BE_TYPE_INT64: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_signed_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_integer_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_INT16:
		case BE_TYPE_INT32:
		case BE_TYPE_INT64:
		case BE_TYPE_UINT8:
		case BE_TYPE_UINT16:
		case BE_TYPE_UINT32:
		case BE_TYPE_UINT64: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_integer_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_float_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_FLOAT:
		case BE_TYPE_DOUBLE: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_float_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_number_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_INT16:
		case BE_TYPE_INT32:
		case BE_TYPE_INT64:
		case BE_TYPE_UINT8:
		case BE_TYPE_UINT16:
		case BE_TYPE_UINT32:
		case BE_TYPE_UINT64:
		case BE_TYPE_FLOAT:
		case BE_TYPE_DOUBLE: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_number_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_pointer_type(
	uint8_t type
) {
	return type == BE_TYPE_POINTER;
}

static bool _is_pointer_type_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_pointer_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_primitive_type(
	uint8_t type
) {
	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_INT16:
		case BE_TYPE_INT32:
		case BE_TYPE_INT64:
		case BE_TYPE_UINT8:
		case BE_TYPE_UINT16:
		case BE_TYPE_UINT32:
		case BE_TYPE_UINT64:
		case BE_TYPE_FLOAT:
		case BE_TYPE_DOUBLE: {
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
	assert(BE_IS_EXPR_NODE(node->type));

	return _is_primitive_type(BE_EXPR_AST_NODE_GET_TYPE(node));
}

static bool _is_compatible_void_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	uint8_t target_type = _get_type_by_type_node(ctx, target);
	uint8_t source_type = _get_type_by_type_node(ctx, source);
	return target_type == BE_TYPE_VOID
			&& source_type == BE_TYPE_VOID;
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

	if (_get_type_by_type_node(ctx, target) == BE_TYPE_STRUCT
			&& _get_type_by_type_node(ctx, source) == BE_TYPE_STRUCT) {
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
		if (be_parser_get_uint_val(ctx, target_dim) != be_parser_get_uint_val(ctx, source_dim)) {
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

	if (_get_type_by_type_node(ctx, target) == BE_TYPE_ARRAY
			&& _get_type_by_type_node(ctx, source) == BE_TYPE_ARRAY) {
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

	if (_get_type_by_type_node(ctx, target) == BE_TYPE_FUNC
			&& _get_type_by_type_node(ctx, source) == BE_TYPE_FUNC
			&& target->nchilds == source->nchilds) {
		ParserASTNode *target_params = target->childs[0];
		ParserASTNode *source_params = source->childs[0];
		if (target_params->nchilds == source_params->nchilds) {
			for (int32_t i = 0; i < target_params->nchilds; i++) {
				ParserASTNode *target_param = target_params->childs[i];
				ParserASTNode *source_param = source_params->childs[i];

				if (target_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type != BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
					return false;
				}
				if (target_param->type != BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
					return false;
				}

				if (target_param->type != BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
						&& source_param->type != BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
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

	if (_get_type_by_type_node(ctx, target) == BE_TYPE_POINTER
			&& _get_type_by_type_node(ctx, source) == BE_TYPE_POINTER) {
		if (same) {
			ParserASTNode *target_type = target->childs[0];
			ParserASTNode *source_type = source->childs[0];
			return _is_compatible_type(ctx, target_type, source_type, true);
		} else {
			if (_get_type_by_type_node(ctx, target->childs[0]) == BE_TYPE_VOID) {
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

static bool _is_compatible_va_list_type(
	ParserContext *ctx,
	ParserASTNode *target,
	ParserASTNode *source,
	bool same
) {
	assert(ctx);
	assert(target);
	assert(source);

	if (_get_type_by_type_node(ctx, target) == BE_TYPE_VA_LIST
			&& _get_type_by_type_node(ctx, source) == BE_TYPE_VA_LIST) {
		return true;
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

	if (_is_compatible_void_type(ctx, target, source, same)) {
		return true;
	}
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
	if (_is_compatible_va_list_type(ctx, target, source, same)) {
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
		case BE_NODE_TYPE_CHAR: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_CHAR)
			break;
		}
		case BE_NODE_TYPE_INT8: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_INT8)
			break;
		}
		case BE_NODE_TYPE_UINT8: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_UINT8)
			break;
		}
		case BE_NODE_TYPE_INT16: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_INT16)
			break;
		}
		case BE_NODE_TYPE_UINT16: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_UINT16)
			break;
		}
		case BE_NODE_TYPE_INT32: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_INT32)
			break;
		}
		case BE_NODE_TYPE_UINT32: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_UINT32)
			break;
		}
		case BE_NODE_TYPE_INT64: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_INT64)
			break;
		}
		case BE_NODE_TYPE_UINT64: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_UINT64)
			break;
		}
		case BE_NODE_TYPE_FLOAT: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_FLOAT)
			break;
		}
		case BE_NODE_TYPE_DOUBLE: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_DOUBLE)
			break;
		}
		case BE_NODE_TYPE_VA_LIST: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_VA_LIST)
			break;
		}
		case BE_NODE_TYPE_STRUCT: {
			assert(node->nchilds == 1);

			_SET_TYPE(BE_TYPE_STRUCT)

			ParserASTNode *node_id = node->childs[0];
			assert(node_id->type == BE_NODE_IDENTIFIER);

			LexerToken *token = node_id->token;
			ParserSymbol *symbol_struct = parser_get_symbol_from_node(ctx, node, BE_SYM_STRUCT, token);
			if (symbol_struct == NULL) {
				_SYNERR_TOKEN(ctx, token, "undefined struct.");
			} else {
				if (first_type
						&& BE_STRUCT_SYMBOL_GET_DUMMY(symbol_struct)) {
					_SYNERR_TOKEN(ctx, token, "struct not implemented.");
				}
			}

			break;
		}
		case BE_NODE_TYPE_ARRAY: {
			assert(node->nchilds == 2);

			_SET_TYPE(BE_TYPE_ARRAY)

			ParserASTNode *node_array_dims = node->childs[0];
			assert(node_array_dims->type == BE_NODE_TYPE_ARRAY_DIMS);
			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == BE_NODE_LITERAL_UINT);

				uint64_t dim = be_parser_get_uint_val(ctx, node_array_dim);
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
		case BE_NODE_TYPE_FUNC: {
			assert(node->nchilds == 1 || node->nchilds == 2);

			_SET_TYPE(BE_TYPE_FUNC)

			ParserASTNode *node_params = NULL;
			ParserASTNode *node_type = NULL;

			node_params = node->childs[0];
			for (int i = 0; i < node_params->nchilds; i++) {
				ParserASTNode *node_param = node_params->childs[i];
				switch (node_param->type) {
					case BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {

						break;
					}
					case BE_NODE_FUNC_PARAMS_ITEM: {
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
		case BE_NODE_TYPE_POINTER: {
			assert(node->nchilds == 1);

			_SET_TYPE(BE_TYPE_POINTER)

			ParserASTNode *node_type = node->childs[0];
			__type(ctx, node_type, NULL, false);

			break;
		}
		case BE_NODE_TYPE_VOID: {
			assert(node->nchilds == 0);
			_SET_TYPE(BE_TYPE_VOID)
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
	assert(node_type->type == BE_NODE_TYPE_STRUCT);
	assert(node_type->nchilds == 1);

	ParserASTNode *node_id = node_type->childs[0];
	assert(node_id);
	assert(node_id->type == BE_NODE_IDENTIFIER);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(ctx, node, BE_SYM_STRUCT, node_id->token);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, node_id->token, "undefined struct.");
	}

	ParserASTNode *struct_node = BE_STRUCT_SYMBOL_GET_STRUCT_NODE(struct_symbol);
	assert(struct_node);
	assert(struct_node->type == BE_NODE_STRUCT);
	if (BE_STRUCT_AST_NODE_GET_PACKED(struct_node)) {
		return 1;
	}

	ParserASTNode *struct_body_node = BE_STRUCT_SYMBOL_GET_BODY_NODE(struct_symbol);
	assert(struct_body_node);
	assert(struct_body_node->type == BE_NODE_STRUCT_BODY);

	size_t max_align = 1;

	for (int i = 0; i < struct_body_node->nchilds; i++) {
		ParserASTNode *struct_body_member_node = struct_body_node->childs[i];
		assert(struct_body_member_node);
		assert(struct_body_member_node->type == BE_NODE_VAR);
		for (int j = 0; j < struct_body_member_node->nchilds; j++) {
			ParserASTNode *var_item_node = struct_body_member_node->childs[j];
			assert(var_item_node);
			assert(var_item_node->type == BE_NODE_VAR_ITEM);
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
		case BE_NODE_TYPE_CHAR: {
			return 1;
		}
		case BE_NODE_TYPE_INT8: {
			return 1;
		}
		case BE_NODE_TYPE_INT16: {
			return 2;
		}
		case BE_NODE_TYPE_INT32: {
			return 4;
		}
		case BE_NODE_TYPE_INT64: {
			return 8;
		}
		case BE_NODE_TYPE_UINT8: {
			return 1;
		}
		case BE_NODE_TYPE_UINT16: {
			return 2;
		}
		case BE_NODE_TYPE_UINT32: {
			return 4;
		}
		case BE_NODE_TYPE_UINT64: {
			return 8;
		}
		case BE_NODE_TYPE_FLOAT: {
			return 4;
		}
		case BE_NODE_TYPE_DOUBLE: {
			return 8;
		}
		case BE_NODE_TYPE_VA_LIST: {
			return 1;
		}
		case BE_NODE_TYPE_STRUCT: {
			return _calc_struct_type_align(ctx, node, node_type);
		}
		case BE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_type = node_type->childs[1];
			assert(node_array_type);

			return _calc_type_align(ctx, node, node_array_type);
		}
		case BE_NODE_TYPE_FUNC: {
			return 0;
		}
		case BE_NODE_TYPE_POINTER: {
			if (ctx->arch == BE_ARCH_32) {
				return 4;
			} else if (ctx->arch == BE_ARCH_64) {
				return 8;
			} else {
				assert(0);
				return 0;
			}
		}
		case BE_NODE_TYPE_VOID: {
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
	assert(node_type->type == BE_NODE_TYPE_STRUCT);
	assert(node_type->nchilds == 1);

	ParserASTNode *node_id = node_type->childs[0];
	assert(node_id);
	assert(node_id->type == BE_NODE_IDENTIFIER);

	ParserSymbol *struct_symbol = parser_get_symbol_from_node(ctx, node, BE_SYM_STRUCT, node_id->token);
	if (struct_symbol == NULL) {
		_SYNERR_TOKEN(ctx, node_id->token, "undefined struct.");
	}

	ParserASTNode *struct_node = BE_STRUCT_SYMBOL_GET_STRUCT_NODE(struct_symbol);
	assert(struct_node);
	assert(struct_node->type == BE_NODE_STRUCT);

	ParserASTNode *struct_body_node = BE_STRUCT_SYMBOL_GET_BODY_NODE(struct_symbol);
	assert(struct_body_node);
	assert(struct_body_node->type == BE_NODE_STRUCT_BODY);

	size_t max_align = 1;
	size_t size = 0;

	for (int i = 0; i < struct_body_node->nchilds; i++) {
		ParserASTNode *struct_body_member_node = struct_body_node->childs[i];
		assert(struct_body_member_node);
		assert(struct_body_member_node->type == BE_NODE_VAR);
		for (int j = 0; j < struct_body_member_node->nchilds; j++) {
			ParserASTNode *var_item_node = struct_body_member_node->childs[j];
			assert(var_item_node);
			assert(var_item_node->type == BE_NODE_VAR_ITEM);
			assert(var_item_node->nchilds >= 2);

			ParserASTNode *node_var_type = var_item_node->childs[1];
			size_t align = 1;
			if (BE_STRUCT_AST_NODE_GET_PACKED(struct_node)) {
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
		case BE_NODE_TYPE_CHAR: {
			return 1;
		}
		case BE_NODE_TYPE_INT8: {
			return 1;
		}
		case BE_NODE_TYPE_INT16: {
			return 2;
		}
		case BE_NODE_TYPE_INT32: {
			return 4;
		}
		case BE_NODE_TYPE_INT64: {
			return 8;
		}
		case BE_NODE_TYPE_UINT8: {
			return 1;
		}
		case BE_NODE_TYPE_UINT16: {
			return 2;
		}
		case BE_NODE_TYPE_UINT32: {
			return 4;
		}
		case BE_NODE_TYPE_UINT64: {
			return 8;
		}
		case BE_NODE_TYPE_FLOAT: {
			return 4;
		}
		case BE_NODE_TYPE_DOUBLE: {
			return 8;
		}
		case BE_NODE_TYPE_VA_LIST: {
			return 8;
		}
		case BE_NODE_TYPE_STRUCT: {
			return _calc_struct_type_size(ctx, node, node_type);
		}
		case BE_NODE_TYPE_ARRAY: {
			assert(node_type->nchilds == 2);

			ParserASTNode *node_array_dims = node_type->childs[0];
			assert(node_array_dims);
			assert(node_array_dims->type == BE_NODE_TYPE_ARRAY_DIMS);

			ParserASTNode *node_array_type = node_type->childs[1];
			assert(node_array_type);

			size_t size = _calc_type_size(ctx, node, node_array_type);
			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim);

				size *= be_parser_get_uint_val(ctx, node_array_dim);
			}

			return size;
		}
		case BE_NODE_TYPE_FUNC: {
			return 0;
		}
		case BE_NODE_TYPE_POINTER: {
			if (ctx->arch == BE_ARCH_32) {
				return 4;
			} else if (ctx->arch == BE_ARCH_64) {
				return 8;
			} else {
				assert(0);
				return 0;
			}
		}
		case BE_NODE_TYPE_VOID: {
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
	assert(node_type->type == BE_NODE_TYPE_POINTER);
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
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source));

	uint8_t source_type = BE_EXPR_AST_NODE_GET_TYPE(source);

	#define	_A(op, type)	\
		case BE_TYPE_##type: {	\
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(target, op BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(source));	\
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
		_C(!, BE_NODE_EXPR_NOT)
		_C(~, BE_NODE_EXPR_BNOT)
		_C(+, BE_NODE_EXPR_POSITIVE)
		_C(-, BE_NODE_EXPR_NEGATIVE)
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

	uint8_t source_type = _get_type_by_type_node(ctx, BE_EXPR_AST_NODE_GET_TYPE_NODE(source));

	// 常量表达式不能把指针类型转换为非指针类型。
	if (source_type == BE_TYPE_POINTER
			&& target_type != BE_TYPE_POINTER) {
		_SYNERR_NODE(ctx, source, "not a compile-time constant.");
	}

	#define	_A(target_t, target_t_c)	\
		case BE_TYPE_##target_t: {	\
			switch (source_type) {	\
				case BE_TYPE_CHAR: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_INT8: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_INT16: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_INT32: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_INT64: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_UINT8: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_UINT16: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_UINT32: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_UINT64: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_FLOAT: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_DOUBLE: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(source)	\
					);	\
					break;	\
				}	\
				case BE_TYPE_POINTER: {	\
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##target_t##_VAL(	\
						target,	\
						(target_t_c) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source)	\
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
		case BE_TYPE_##type: {	\
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
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
		case BE_TYPE_##type: {	\
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
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
		case BE_TYPE_##type: {	\
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(node, value);	\
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
	assert(BE_IS_EXPR_NODE(node->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case BE_TYPE_##type: {	\
			return (int64_t) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
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
	assert(BE_IS_EXPR_NODE(node->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case BE_TYPE_##type: {	\
			return (uint64_t) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
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
	assert(BE_IS_EXPR_NODE(node->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case BE_TYPE_##type: {	\
			return (float) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
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
	assert(BE_IS_EXPR_NODE(node->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case BE_TYPE_##type: {	\
			return (double) BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node);	\
		}

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
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
	assert(BE_IS_EXPR_NODE(node->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node));

	#define	_A(type)	\
		case BE_TYPE_##type: {	\
			return BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(node) != 0;	\
		}

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node)) {
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
	assert(BE_IS_EXPR_NODE(source1->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(BE_IS_EXPR_NODE(source2->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t source1_type = BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source1);
	int32_t source1_type_weight = _get_primitive_type_weight(source1_type);

	uint8_t source2_type = BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source2);
	int32_t source2_type_weight = _get_primitive_type_weight(source2_type);

	if (_is_signed_type(source1_type) && _is_signed_type(source2_type)) {
		return BE_TYPE_INT64;
	} else if (_is_unsigned_type(source1_type) && _is_unsigned_type(source2_type)) {
		return BE_TYPE_UINT64;
	} else if (_is_signed_type(source1_type) && _is_unsigned_type(source2_type)) {
		if (source1_type_weight > source2_type_weight) {
			return BE_TYPE_INT64;
		} else {
			return BE_TYPE_UINT64;
		}
	} else if (_is_unsigned_type(source1_type) && _is_signed_type(source2_type)) {
		if (source1_type_weight > source2_type_weight) {
			return BE_TYPE_UINT64;
		} else {
			return BE_TYPE_INT64;
		}
	} else if (source1_type == BE_TYPE_DOUBLE
			|| source2_type == BE_TYPE_DOUBLE) {
		return BE_TYPE_DOUBLE;
	} else if (source1_type == BE_TYPE_FLOAT
			|| source2_type == BE_TYPE_FLOAT) {
		return BE_TYPE_FLOAT;
	} else {
		assert(0);
		return BE_TYPE_UNKNOWN;
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
	assert(BE_IS_EXPR_NODE(source1->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(BE_IS_EXPR_NODE(source2->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t target_type = BE_EXPR_AST_NODE_GET_TYPE(target);
	uint8_t source1_type = BE_EXPR_AST_NODE_GET_TYPE(source1);
	uint8_t source1_constant_type = BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source1);
	uint8_t source2_type = BE_EXPR_AST_NODE_GET_TYPE(source2);
	uint8_t source2_constant_type = BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(source2);

	#define	_A(op)	\
		switch (source_type) {	\
			case BE_TYPE_INT64: {	\
				int64_t result = _get_constexpr_int64(ctx, source1) op _get_constexpr_int64(ctx, source2);	\
				_set_constexpr_int64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case BE_TYPE_UINT64: {	\
				uint64_t result = _get_constexpr_uint64(ctx, source1) op _get_constexpr_uint64(ctx, source2);	\
				_set_constexpr_uint64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case BE_TYPE_FLOAT: {	\
				float result = _get_constexpr_float(ctx, source1) op _get_constexpr_float(ctx, source2);	\
				_set_constexpr_float(ctx, target, target_type, result);	\
				break;	\
			}	\
			case BE_TYPE_DOUBLE: {	\
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
			case BE_TYPE_INT64: {	\
				int64_t result = _get_constexpr_int64(ctx, source1) op _get_constexpr_int64(ctx, source2);	\
				_set_constexpr_int64(ctx, target, target_type, result);	\
				break;	\
			}	\
			case BE_TYPE_UINT64: {	\
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
		_B(BE_NODE_EXPR_MUL, *)
		_B(BE_NODE_EXPR_DIV, /)
		case BE_NODE_EXPR_MOD: {
			uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
			switch (source_type) {
				case BE_TYPE_INT64: {
					int64_t result = _get_constexpr_int64(ctx, source1) % _get_constexpr_int64(ctx, source2);
					_set_constexpr_int64(ctx, target, target_type, result);
					break;
				}
				case BE_TYPE_UINT64: {
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
		case BE_NODE_EXPR_ADD: {
			if (source1_type == BE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source1,
					BE_EXPR_AST_NODE_GET_TYPE_NODE(source1)
				);
				uint64_t source1_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				if (_is_signed_type(source2_type)) {
					int64_t source2_value = _get_constexpr_int64(ctx, source2);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value + increment_size * source2_value
					);
				} else if (_is_unsigned_type(source2_type)) {
					uint64_t source2_value = _get_constexpr_uint64(ctx, source2);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value + increment_size * source2_value
					);
				} else {
					assert(0);
				}
			} else if (source2_type == BE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source2,
					BE_EXPR_AST_NODE_GET_TYPE_NODE(source2)
				);
				uint64_t source2_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (_is_signed_type(source1_type)) {
					int64_t source1_value =  _get_constexpr_int64(ctx, source1);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source2_value + increment_size * source1_value
					);
				} else if (_is_unsigned_type(source1_type)) {
					uint64_t source1_value =  _get_constexpr_uint64(ctx, source1);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
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
		case BE_NODE_EXPR_SUB: {
			if (source1_type == BE_TYPE_POINTER) {
				size_t increment_size = _calc_pointer_type_increment_size(
					ctx,
					source1,
					BE_EXPR_AST_NODE_GET_TYPE_NODE(source1)
				);
				uint64_t source1_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				if (_is_signed_type(source2_type)) {
					int64_t source2_value = _get_constexpr_int64(ctx, source2);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
						target,
						source1_value - increment_size * source2_value
					);
				} else if (_is_unsigned_type(source2_type)) {
					uint64_t source2_value = _get_constexpr_uint64(ctx, source2);
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
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
		_B(BE_NODE_EXPR_LT, <)
		_B(BE_NODE_EXPR_LE, <=)
		_B(BE_NODE_EXPR_GT, >)
		_B(BE_NODE_EXPR_GE, >=)
		case BE_NODE_EXPR_EQ: {
			if (source1_type == BE_TYPE_POINTER
					&& source2_type == BE_TYPE_POINTER) {
				uint64_t source1_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				uint64_t source2_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(target, source1_value == source2_value);
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(target, source1_value == source2_value);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(==)
			}
			break;
		}
		case BE_NODE_EXPR_NEQ: {
			if (source1_type == BE_TYPE_POINTER
					&& source2_type == BE_TYPE_POINTER) {
				uint64_t source1_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1);
				uint64_t source2_value = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2);
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(target, source1_value != source2_value);
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(target, source1_value != source2_value);
				} else {
					assert(0);
				}
			} else {
				uint8_t source_type = _get_binary_constexpr_type(ctx, source1, source2);
				_A(!=)
			}
			break;
		}
		_B1(BE_NODE_EXPR_BAND, &)
		_B1(BE_NODE_EXPR_BXOR, ^)
		_B1(BE_NODE_EXPR_BOR, |)
		_B1(BE_NODE_EXPR_AND, &&)
		_B1(BE_NODE_EXPR_OR, ||)
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
	assert(BE_IS_EXPR_NODE(source1->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(BE_IS_EXPR_NODE(source2->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source2));

	uint8_t source1_type = BE_EXPR_AST_NODE_GET_TYPE(source1);
	int64_t source2_value = _get_constexpr_int64(ctx, source2);

	#define	_A(type, op)	\
		case BE_TYPE_##type: {	\
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_##type##_VAL(	\
				target,	\
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_##type##_VAL(source1) op source2_value	\
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
		_B(BE_NODE_EXPR_SHIFT_LEFT, <<)
		_B(BE_NODE_EXPR_SHIFT_RIGHT, >>)
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
	assert(BE_IS_EXPR_NODE(cond->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(cond));
	assert(source1);
	assert(BE_IS_EXPR_NODE(source1->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source1));
	assert(source2);
	assert(BE_IS_EXPR_NODE(source2->type));
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(source2));

	if (_is_primitive_type_node(ctx, source1)) {
		ParserASTNode *source = NULL;

		if (_get_constexpr_bool(ctx, cond)) {
			source = source1;
		} else {
			source = source2;
		}

		_calc_cast_constexpr(
			ctx,
			BE_EXPR_AST_NODE_GET_TYPE(target),
			target,
			source
		);
	} else if (_is_pointer_type_node(ctx, source1)) {
		if (_get_constexpr_bool(ctx, cond)) {
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
				target,
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source1)
			);
		} else {
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(
				target,
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(source2)
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
	assert(node->type == BE_NODE_EXPR_ATOM);
	assert(node->nchilds == 1);
	
	ParserASTNode *child = node->childs[0];

	switch (child->type) {
		case BE_NODE_LITERAL_UINT: {
			uint64_t val = be_parser_get_uint64_val(ctx, child);
			bool has_unsigned_mark = be_parser_has_unsigned_mark(ctx, child);
			if (val <= 0xffffffff) {
				if (has_unsigned_mark) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(node, (uint32_t)val);
				} else {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(node, *(int32_t *)&val);
				}
			} else {
				if (has_unsigned_mark) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);

					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(node, val);
				} else {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(node, *(int64_t *)&val);
				}
			}

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_LITERAL_REAL: {
			if (be_parser_has_float_mark(ctx, child)) {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_FLOAT);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_FLOAT);

				float val = be_parser_get_float_val(ctx, child);
				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_FLOAT_VAL(node, val);
			} else {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_DOUBLE);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_DOUBLE);

				double val = be_parser_get_double_val(ctx, child);
				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_DOUBLE_VAL(node, val);
			}

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_LITERAL_CHAR: {
			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_CHAR);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_CHAR);

			char val = be_parser_get_char_val(ctx, child);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_CHAR_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_LITERAL_STRING: {
			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_POINTER);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_CHAR_POINTER);

			BE_EXPR_AST_NODE_SET_CONSTANT_TYPE(node, BE_TYPE_POINTER);

			BE_EXPR_AST_NODE_SET_HAS_CONSTSTR(node, true);

			BeParserConstexprString *conststr = BE_EXPR_AST_NODE_GET_CONSTSTR(node);
			conststr->str = child->token->content + 1;
			conststr->len = child->token->len - 2;

			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_POINTER_VAL(node, 0);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_U8CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT8);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT8);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT8_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_U16CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT16);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT16);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT16_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_U32CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT32);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_U64CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT64);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_I8CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT8);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT8);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT8_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_I16CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT16);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT16);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT16_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_I32CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT32_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_I64CONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_uint = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

			uint64_t val = be_parser_get_uint64_val(ctx, node_literal_uint);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_INT64_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_FCONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_real = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_FLOAT);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_FLOAT);

			float val = be_parser_get_float_val(ctx, node_literal_real);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_FLOAT_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_DCONST: {
			assert(child->nchilds == 1);
			ParserASTNode *node_literal_real = child->childs[0];

			BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_DOUBLE);
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_DOUBLE);

			double val = be_parser_get_float_val(ctx, node_literal_real);
			BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_DOUBLE_VAL(node, val);

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
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

	if (node->type == BE_NODE_EXPR_ATOM) {
		_expr_atom(ctx, node);
	} else {
		_expr(ctx, node);
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
		case BE_NODE_EXPR_NOT:
		case BE_NODE_EXPR_BNOT: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_parenthesis(ctx, node_oprd);

			if (!_is_integer_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case BE_NODE_EXPR_NOT: {
						_SYNERR_NODE(ctx, node, "unary operator '!' need integer operand.");
						break;
					}
					case BE_NODE_EXPR_BNOT: {
						_SYNERR_NODE(ctx, node, "unary operator '~' need integer operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			BE_EXPR_AST_NODE_COPY(node, node_oprd);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node_oprd)) {
				_calc_unary_constexpr(ctx, node_type, node, node_oprd);
			}
			break;
		}
		case BE_NODE_EXPR_POSITIVE:
		case BE_NODE_EXPR_NEGATIVE: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			_expr_parenthesis(ctx, node_oprd);

			if (!_is_number_type_node(ctx, node_oprd)) {
				switch (node_type) {
					case BE_NODE_EXPR_POSITIVE: {
						_SYNERR_NODE(ctx, node, "unary operator '+' need number operand.");
						break;
					}
					case BE_NODE_EXPR_NEGATIVE: {
						_SYNERR_NODE(ctx, node, "unary operator '-' need number operand.");
						break;
					}
					default: {
						assert(0);
					}
				}
			}

			BE_EXPR_AST_NODE_COPY(node, node_oprd);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node_oprd)) {
				BE_EXPR_AST_NODE_COPY_CONSTANT(node, node_oprd);
				_calc_unary_constexpr(ctx, node_type, node, node_oprd);
			}
			break;
		}
		case BE_NODE_EXPR_SIZEOF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			ParserASTNode *type_node = NULL;
			switch (_get_node_catagory(node_oprd->type)) {
				case BE_SEM_NODE_CATEGORY_TYPE: {
					_type(ctx, node_oprd, NULL);
					type_node = node_oprd;
					break;
				}
				case BE_SEM_NODE_CATAGORY_EXPR: {
					_expr(ctx, node_oprd);
					type_node = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			if (ctx->arch == BE_ARCH_32) {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT32);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(
					node, (uint32_t) _calc_type_size(ctx, node, type_node)
				);
			} else if (ctx->arch == BE_ARCH_64) {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT64);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);
			
				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(
					node, (uint64_t) _calc_type_size(ctx, node, type_node)
				);
			} else {
				assert(0);
			}

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);

			break;
		}
		case BE_NODE_EXPR_ALIGNOF: {
			assert(node->nchilds == 1);

			ParserASTNode *node_oprd = node->childs[0];
			ParserASTNode *type_node = NULL;
			switch (_get_node_catagory(node_oprd->type)) {
				case BE_SEM_NODE_CATEGORY_TYPE: {
					_type(ctx, node_oprd, NULL);
					type_node = node_oprd;
					break;
				}
				case BE_SEM_NODE_CATAGORY_EXPR: {
					_expr(ctx, node_oprd);
					type_node = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_oprd);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}

			size_t align = 0;
			if (node_oprd->type == BE_NODE_EXPR_ATOM
					&& node_oprd->childs[0]->type == BE_NODE_IDENTIFIER) {
				ParserSymbol *symbol = parser_get_symbol_from_node(
					ctx, node, BE_SYM_VAR, node_oprd->childs[0]->token
				);
				if (symbol != NULL) {
					align = BE_VAR_SYMBOL_GET_ALIGN(symbol);
				} else {
					align = _calc_type_align(ctx, node, type_node);
				}
			} else {
				align = _calc_type_align(ctx, node, type_node);
			}

			if (ctx->arch == BE_ARCH_32) {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT32);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT32);

				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT32_VAL(
					node, (uint32_t) align
				);
			} else if (ctx->arch == BE_ARCH_64) {
				BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_UINT64);
				BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_UINT64);
			
				BE_EXPR_AST_NODE_SET_CONSTEXPR_RESULT_UINT64_VAL(
					node, (uint64_t) align
				);
			} else {
				assert(0);
			}

			BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
			break;
		}
		case BE_NODE_EXPR_CAST: {
			assert(node->nchilds == 2);

			ParserASTNode *node_type = node->childs[0];
			ParserASTNode *node_expr = node->childs[1];

			_type(ctx, node_type, NULL);
			_expr(ctx, node_expr);

			BE_EXPR_AST_NODE_COPY(node, node_expr);

			uint8_t node_type_type = _get_type_by_type_node(ctx, node_type);
			if (node_type_type == BE_TYPE_STRUCT
					|| node_type_type == BE_TYPE_ARRAY
					|| node_type_type == BE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node, "operator cast cannot cast operand to struct type or array type or function type."
				);
			}

			ParserASTNode *node_expr_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
			uint8_t node_expr_type_type = _get_type_by_type_node(ctx, node_expr_type);
			if (node_expr_type_type == BE_TYPE_STRUCT
					|| node_expr_type_type == BE_TYPE_ARRAY
					|| node_expr_type_type == BE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node, "operator cast cannot cast struct type or array type or function type operand."
				);
			}

			BE_EXPR_AST_NODE_SET_TYPE(node, _get_type_by_type_node(ctx, node_type));
			BE_EXPR_AST_NODE_SET_TYPE_NODE(node, node_type);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node_expr)) {
				BE_EXPR_AST_NODE_SET_CONSTANT(node, true);
				_calc_cast_constexpr(ctx, node_type_type, node, node_expr);
			}
			break;
		}
		default: {
			_expr_parenthesis(ctx, node);
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
			BE_EXPR_AST_NODE_COPY(node, node_oprd1);
		} else {
			BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
			BE_EXPR_AST_NODE_COPY(node, node_oprd1);
		} else {
			BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
		case BE_NODE_EXPR_MUL:
		case BE_NODE_EXPR_DIV: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == BE_NODE_EXPR_MUL) {
				operator = "*";
			} else if (node_type == BE_NODE_EXPR_DIV) {
				operator = "/";
			} else {
				assert(0);
			}

			_check_expr_mul_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		case BE_NODE_EXPR_MOD: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_mod_operands(
				ctx, node, node_oprd1, node_oprd2, "%", true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
			BE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else if (_is_number_type_node(ctx, node_oprd1) && _is_number_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				BE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
			BE_EXPR_AST_NODE_COPY(node, node_oprd1);
		}
	} else if (_is_number_type_node(ctx, node_oprd1) && _is_number_type_node(ctx, node_oprd2)) {
		if (set_type) {
			int32_t weight_oprd1_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd1);
			int32_t weight_oprd2_type = _get_primitive_type_weight_by_expr_node(ctx, node_oprd2);
			if (weight_oprd1_type > weight_oprd2_type) {
				BE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
		case BE_NODE_EXPR_ADD: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_add_operands(
				ctx, node, node_oprd1, node_oprd2, "+", true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
				_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
			}
			break;
		}
		case BE_NODE_EXPR_SUB: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_unary(ctx, node_oprd1);
			_expr_unary(ctx, node_oprd2);

			_check_expr_sub_operands(
				ctx, node, node_oprd1, node_oprd2, "-", true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
			BE_EXPR_AST_NODE_COPY(node, node_oprd1);
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
		case BE_NODE_EXPR_SHIFT_LEFT:
		case BE_NODE_EXPR_SHIFT_RIGHT: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_add(ctx, node_oprd1);
			_expr_add(ctx, node_oprd2);

			const char *operator = NULL;
			if (node_type == BE_NODE_EXPR_SHIFT_LEFT) {
				operator = "<<";
			} else if (node_type == BE_NODE_EXPR_SHIFT_RIGHT) {
				operator = ">>";
			} else {
				assert(0);
			}

			_check_expr_shift_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
		case BE_NODE_EXPR_LT:
		case BE_NODE_EXPR_LE:
		case BE_NODE_EXPR_GT:
		case BE_NODE_EXPR_GE: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_shift(ctx, node_oprd1);
			_expr_shift(ctx, node_oprd2);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
		case BE_NODE_EXPR_EQ:
		case BE_NODE_EXPR_NEQ: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_lt(ctx, node_oprd1);
			_expr_lt(ctx, node_oprd2);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if ((_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2))
					|| (_is_pointer_type_node(ctx, node_oprd1) && _is_pointer_type_node(ctx, node_oprd2))) {
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
				BE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
		case BE_NODE_EXPR_BAND: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_eq(ctx, node_oprd1);
			_expr_eq(ctx, node_oprd2);

			const char *operator = "&";

			_check_expr_band_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
				BE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
		case BE_NODE_EXPR_BXOR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_band(ctx, node_oprd1);
			_expr_band(ctx, node_oprd2);

			const char *operator = "^";

			_check_expr_bxor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
				BE_EXPR_AST_NODE_COPY(node, node_oprd1);
			} else {
				BE_EXPR_AST_NODE_COPY(node, node_oprd2);
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
		case BE_NODE_EXPR_BOR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_bxor(ctx, node_oprd1);
			_expr_bxor(ctx, node_oprd2);

			const char *operator = "|";

			_check_expr_bor_operands(
				ctx, node, node_oprd1, node_oprd2, operator, true
			);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
		case BE_NODE_EXPR_AND: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_bor(ctx, node_oprd1);
			_expr_bor(ctx, node_oprd2);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
		case BE_NODE_EXPR_OR: {
			_CHECK
			ParserASTNode *node_oprd1 = node->childs[0];
			ParserASTNode *node_oprd2 = node->childs[1];
			_expr_and(ctx, node_oprd1);
			_expr_and(ctx, node_oprd2);

			BE_EXPR_AST_NODE_COPY_CONSTANT2(node, node_oprd1, node_oprd2);

			if (_is_primitive_type_node(ctx, node_oprd1) && _is_primitive_type_node(ctx, node_oprd2)) {
				if (ctx->arch == BE_ARCH_32) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT32);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT32);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
						_calc_binary_constexpr(ctx, node_type, node, node_oprd1, node_oprd2);
					}
				} else if (ctx->arch == BE_ARCH_64) {
					BE_EXPR_AST_NODE_SET_TYPE(node, BE_TYPE_INT64);
					BE_EXPR_AST_NODE_SET_TYPE_NODE(node, _NODE_TYPE_INT64);

					if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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
		case BE_NODE_EXPR_COND: {
			assert(node->nchilds == 3);

			ParserASTNode *node_expr = node->childs[0];
			ParserASTNode *node_expr_true = node->childs[1];
			ParserASTNode *node_expr_false = node->childs[2];

			_expr_or(ctx, node_expr);
			_expr(ctx, node_expr_true);
			_expr(ctx, node_expr_false);

			ParserASTNode *node_expr_true_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_true);
			ParserASTNode *node_expr_false_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr_false);

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
					BE_EXPR_AST_NODE_COPY(node, node_expr_true);
				} else {
					BE_EXPR_AST_NODE_COPY(node, node_expr_false);
				}
			} else if (_is_pointer_type_node(ctx, node_expr_true)
					&& _is_compatible_type(ctx, node_expr_true_type, node_expr_false_type, true)) {
				BE_EXPR_AST_NODE_COPY(node, node_expr_true);
			} else {
				_SYNERR_NODE(
					ctx,
					node,
					"conditional operator do not support this operand combination."
				);
			}

			BE_EXPR_AST_NODE_COPY_CONSTANT3(
				node,
				node_expr,
				node_expr_true,
				node_expr_false
			);
			if (BE_EXPR_AST_NODE_GET_CONSTANT(node)) {
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

static void _expr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	_expr_cond(ctx, node);
}

static void _expr_wrapper(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_EXPR);

	ParserASTNode *child = node->childs[0];

	_expr(ctx, child);

	BE_EXPR_AST_NODE_COPY(node, child);
}



static uint8_t _get_var_scope_type(
	ParserASTNode *node,
	ParserSymbol **symbol_func
) {
	assert(node);
	assert(node->type == BE_NODE_VAR);

	if (symbol_func != NULL) {
		*symbol_func = NULL;
	}

	ParserASTNode *parent = node->parent;
	while (parent != NULL) {
		switch (parent->type) {
			case BE_NODE_MODULE: {
				return BE_VAR_TYPE_GLOBAL;
			}
			case BE_NODE_FUNC: {
				if (symbol_func != NULL) {
					*symbol_func = BE_FUNC_AST_NODE_GET_SYMBOL(parent);
				}
				return BE_VAR_TYPE_LOCAL;
			}
			case BE_NODE_STRUCT: {
				return BE_VAR_TYPE_STRUCT_MEMBER;
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
	assert(node->type == BE_NODE_VAR);
	assert(node->nchilds > 0);
	assert(node_parent);

	ParserASTNode *align_node = BE_VAR_AST_NODE_GET_ALIGN_NODE(node);
	int align = 0;
	if (align_node != NULL) {
		assert(align_node->nchilds == 1);
		switch (_get_node_catagory(align_node->childs[0]->type)) {
			case BE_SEM_NODE_CATEGORY_TYPE: {
				ParserASTNode *type_node = align_node->childs[0];
				_type(ctx, type_node, NULL);
				align = (int) _calc_type_align(ctx, node, type_node);
				assert(align >= 1);
				BE_VAR_AST_NODE_SET_ALIGN(node, align);
				break;
			}
			case BE_SEM_NODE_CATAGORY_EXPR: {
				ParserASTNode *expr_node = align_node->childs[0];
				_expr(ctx, expr_node);
				if (!BE_EXPR_AST_NODE_GET_CONSTANT(expr_node)) {
					_SYNERR_NODE(ctx, expr_node, "align parameter must be constant expression.");
				}
				align = _get_constexpr_int(ctx, expr_node);
				if (align <= 0) {
					_SYNERR_NODE(ctx, expr_node, "align parameter must be greater than 0.");
				}
				if (align != 1 && align % 2 != 0) {
					_SYNERR_NODE(ctx, expr_node, "align parameter is not a power of 2.");
				}
				BE_VAR_AST_NODE_SET_ALIGN(node, align);
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
	BE_VAR_AST_NODE_SET_TYPE(node, var_scope_type);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == BE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_id = node_var_item->childs[0];
		assert(node_id->type == BE_NODE_IDENTIFIER);

		ParserASTNode *node_type = node_var_item->childs[1];
		uint8_t var_type = 0;
		_type(ctx, node_type, &var_type);
		if (var_type == BE_TYPE_FUNC) {
			_SYNERR_NODE(
					ctx,
					node,
					"cannot define a function type variable, "
					"but can define a function pointer type(*function(...)) variable."
				);
		}

		size_t type_size = _calc_type_size(ctx, node, node_type);
		
		ParserSymbol *symbol = be_parser_add_var_symbol_to_node(
			ctx,
			node_parent,
			node_id->token,
			var_type,
			node_type,
			type_size
		);
		BE_VAR_SYMBOL_SET_FUNC_SYMBOL(symbol, func_symbol);
		if (align > 0) {
			BE_VAR_SYMBOL_SET_ALIGN(symbol, align);
		} else {
			BE_VAR_SYMBOL_SET_ALIGN(symbol, _calc_type_align(ctx, node, node_type));
		}
		BE_VAR_SYMBOL_SET_SCOPE_TYPE(symbol, var_scope_type);

		BE_VAR_ITEM_AST_NODE_SET_SYMBOL(node_var_item, symbol);

		if (node_var_item->nchilds == 3) {
			// 如果变量是结构体成员变量，则不被允许包含初始化表达式。
			if (var_scope_type == BE_VAR_TYPE_STRUCT_MEMBER) {
				_SYNERR_NODE(
					ctx,
					node,
					"struct member variable cannot contain initial expression."
				);
			}

			// 如果变量是局部变量，则不被允许包含初始化表达式。
			if (var_scope_type == BE_VAR_TYPE_LOCAL) {
				_SYNERR_NODE(
					ctx,
					node,
					"local variable cannot contain initial expression."
				);
			}

			ParserASTNode *node_expr = node_var_item->childs[2];

			// 插入一个cast节点进行类型强制转换。
			ParserASTNode *node_cast = be_parser_new_node(
				ctx, BE_NODE_EXPR_CAST, "BE_NODE_EXPR_CAST", NULL
			);
			parser_add_child(ctx, node_cast, node_type);
			parser_insert_node(ctx, &(node_expr->childs[0]), node_cast);

			_expr_wrapper(ctx, node_expr);

			// 如果变量是全局变量，则初始化表达式只允许常量表达式。
			bool const_expr = BE_EXPR_AST_NODE_GET_CONSTANT(node_expr);
			if (var_scope_type == BE_VAR_TYPE_GLOBAL && !const_expr) {
				_SYNERR_NODE(
					ctx,
					node,
					"variable initial expression must be constant expression."
				);
			}

			ParserASTNode *node_expr_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_expr);
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
	assert(node->type == BE_NODE_VAR);
	assert(node->nchilds > 0);

	ParserASTNode *node_parent = node->parent;
	assert(node_parent);

	return _var_with_parent(ctx, node, node_parent);
}

static void _process_struct_member(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_struct_body,
	ParserSymbol *symbol_struct
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STRUCT);
	assert(node->nchilds == 2);
	assert(node_struct_body);
	assert(node_struct_body->type == BE_NODE_STRUCT_BODY);
	assert(symbol_struct);

	size_t max_align = 1;
	size_t size = 0;

	for (int i = 0; i < node_struct_body->nchilds; i++) {
		ParserASTNode *child = node_struct_body->childs[i];
		assert(child);
		if (child->type == BE_NODE_VAR) {
			for (int j = 0; j < child->nchilds; j++) {
				ParserASTNode *node_var_item = child->childs[j];
				assert(node_var_item->type == BE_NODE_VAR_ITEM);
				
				if (node_var_item->nchilds == 2) {
					ParserASTNode *node_var_id = node_var_item->childs[0];
					assert(node_var_id->type == BE_NODE_IDENTIFIER);

					ParserASTNode *node_var_type = node_var_item->childs[1];
					uint8_t var_type = 0;
					_type(ctx, node_var_type, &var_type);

					size_t align = 1;
					if (BE_STRUCT_AST_NODE_GET_PACKED(node)) {
						align = 1;
					} else {
						align = _calc_type_align(ctx, node, node_var_type);
					}

					size_t offset = size;

					be_parser_add_struct_member_var_symbol_to_symbol(
						ctx,
						symbol_struct,
						node_var_id->token,
						var_type,
						node_var_type,
						offset
					);

					if (align > max_align) {
						max_align = align;
					}

					if (size % align > 0) {
						size += align - size % align;
					}

					size += _calc_type_size(ctx, node, node_var_type);
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
	assert(node->type == BE_NODE_STRUCT);
	assert(node->nchilds == 2);

	ParserASTNode *node_identifier = node->childs[0];
	assert(node_identifier->type == BE_NODE_IDENTIFIER);

	ParserASTNode *node_struct_body = node->childs[1];
	assert(node_struct_body->type == BE_NODE_STRUCT_BODY
			|| node_struct_body->type == BE_NODE_DUMMY);

	ParserASTNode *node_parent = node->parent;
	assert(node_parent);

	if (node_struct_body->type == BE_NODE_DUMMY) {
		be_parser_add_struct_symbol_to_node(
			ctx, node_parent, node_identifier->token, node_struct_body, true
		);
	} else {
		ParserSymbol *symbol = parser_get_symbol_from_node(
			ctx, node_parent, BE_SYM_STRUCT, node_identifier->token
		);

		if (symbol != NULL
				&& BE_STRUCT_SYMBOL_GET_DUMMY(symbol)) {
			_process_struct_member(ctx, node, node_struct_body, symbol);

			BE_STRUCT_SYMBOL_SET_DUMMY(symbol, false);
			BE_STRUCT_SYMBOL_SET_STRUCT_NODE(symbol, node);
			BE_STRUCT_SYMBOL_SET_BODY_NODE(symbol, node_struct_body);
		} else {
			ParserSymbol *symbol_struct = be_parser_new_struct_symbol(
				ctx, node_identifier->token, node, node_struct_body, false
			);

			_process_struct_member(ctx, node, node_struct_body, symbol_struct);

			parser_add_symptr_to_node(ctx, node_parent, symbol_struct);
		}
	}
}




static void _stat_var(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_VAR);

	_var(ctx, node);
}

static ParserSymbol * _get_var_symbol_by_id_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_IDENTIFIER);

	ParserSymbol *symbol = parser_get_symbol_from_node(
		ctx, node, BE_SYM_VAR, node->token
	);
	if (symbol == NULL) {
		_SYNERR_TOKEN(ctx, node->token, "invalid identifier.");
	}

	return symbol;
}

static ParserSymbol * _get_struct_symbol_by_id_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_IDENTIFIER);

	ParserSymbol *symbol = parser_get_symbol_from_node(
		ctx, node, BE_SYM_STRUCT, node->token
	);
	if (symbol == NULL) {
		_SYNERR_TOKEN(ctx, node->token, "invalid identifier.");
	}

	return symbol;
}

static ParserSymbol * _get_struct_member_symbol(
	ParserContext *ctx,
	ParserASTNode *node_struct_id,
	ParserASTNode *node_struct_member_id
) {
	assert(ctx);
	assert(node_struct_id);
	assert(node_struct_id->type == BE_NODE_IDENTIFIER);
	assert(node_struct_member_id);
	assert(node_struct_member_id->type == BE_NODE_IDENTIFIER);

	ParserSymbol *symbol_struct = _get_struct_symbol_by_id_node(
		ctx, node_struct_id
	);

	ParserSymbol *symbol_struct_member = parser_get_symbol_from_symbol(
		ctx, symbol_struct, BE_SYM_STRUCT_MEMBER_VAR, node_struct_member_id->token
	);
	if (symbol_struct_member == NULL) {
		_SYNERR_TOKEN(ctx, node_struct_member_id->token, "invalid identifier.");
	}

	return symbol_struct_member;
}

static ParserSymbol * _get_symbol_by_id_node(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_IDENTIFIER);

	ParserSymbol *symbol = parser_get_symbol_from_node(
		ctx, node, 0, node->token
	);
	if (symbol == NULL) {
		_SYNERR_TOKEN(ctx, node->token, "invalid identifier.");
	}

	return symbol;
}

static bool _is_assignable_type(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	
	uint8_t type = _get_type_by_type_node(ctx, node);

	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_INT16:
		case BE_TYPE_INT32:
		case BE_TYPE_INT64:
		case BE_TYPE_UINT8:
		case BE_TYPE_UINT16:
		case BE_TYPE_UINT32:
		case BE_TYPE_UINT64:
		case BE_TYPE_FLOAT:
		case BE_TYPE_DOUBLE:
		case BE_TYPE_POINTER: {
			return true;
		}
		default: {
			return false;
		}
	}
}

static ParserASTNode * _check_assignable(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserSymbol *symbol
) {
	assert(ctx);
	assert(node);
	assert(symbol);
	assert(symbol->type == BE_SYM_VAR);

	ParserASTNode *node_type = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol);
	if (!_is_assignable_type(ctx, node_type)) {
		_SYNERR_NODE(ctx, node, "variable is not assignable.");
	}

	return node_type;
}

static void _check_param_combination2(
	ParserContext *ctx,
	ParserASTNode *node,		// 用来报错时提供错误位置。
	ParserASTNode *node_type1,
	ParserASTNode *node_type2
) {
	assert(ctx);
	assert(node);
	assert(node_type1);
	assert(node_type2);

	if (!_is_compatible_type(ctx, node_type1, node_type2, true)) {
		_SYNERR_NODE(ctx, node, "invalid parameter combination.");
	}
}

static void _check_param_combination3(
	ParserContext *ctx,
	ParserASTNode *node,		// 用来报错时提供错误位置。
	ParserASTNode *node_type1,
	ParserASTNode *node_type2,
	ParserASTNode *node_type3
) {
	_check_param_combination2(ctx, node, node_type1, node_type2);
	_check_param_combination2(ctx, node, node_type2, node_type3);
}

typedef struct {
	ParserASTNode	*node_type;
	uint8_t			type;
} _GetParamInfoResult;

static void _get_param_info(
	ParserContext *ctx,
	ParserASTNode *node_param,
	_GetParamInfoResult *result
) {
	assert(ctx);
	assert(node_param);
	assert(result);

	ParserASTNode *node_type = NULL;
	uint8_t type = BE_TYPE_UNKNOWN;

	if (node_param->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol = _get_symbol_by_id_node(ctx, node_param);
		if (symbol->type == BE_SYM_VAR) {
			node_type = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol);
		} else if (symbol->type == BE_SYM_FUNC) {
			node_type = BE_FUNC_SYMBOL_GET_FUNC_POINTER_TYPE_NODE(symbol);
		} else {
			_SYNERR_NODE(
				ctx,
				node_param,
				"expect variable name or function name."
			);
		}
	} else if (node_param->type == BE_NODE_EXPR) {
		_expr_wrapper(ctx, node_param);
		node_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_param);
	} else {
		assert(0);
	}
	type = _get_type_by_type_node(ctx, node_type);

	result->node_type = node_type;
	result->type = type;
}

static void _stat_assign(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASSIGN);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_target_type = _check_assignable(ctx, node_target, symbol_target);

	ParserASTNode *node_source = node->childs[1];
	_GetParamInfoResult result_source;
	_get_param_info(ctx, node_source, &result_source);
	_check_param_combination2(ctx, node, node_target_type, result_source.node_type);
}

static void _stat_asm(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM);

	// 什么也不干。
}

static void _stat_asm_set_reg(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM_SET_REG);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_LITERAL_STRING);

	ParserASTNode *node_source = node->childs[1];
	_GetParamInfoResult result_source;
	_get_param_info(ctx, node_source, &result_source);
	if (!_is_primitive_type(result_source.type)
			&& !_is_pointer_type(result_source.type)) {
		_SYNERR_NODE(
			ctx,
			node_source,
			"source parameter type must be primitive type or pointer type."
		);
	}
}

static void _stat_asm_get_reg(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM_GET_REG);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	uint8_t type_target = BE_VAR_SYMBOL_GET_TYPE(symbol_target);
	if (!_is_primitive_type(type_target)
			&& !_is_pointer_type(type_target)) {
		_SYNERR_NODE(ctx, node_target, "target parameter type must be primitive type or pointer type.");
	}

	ParserASTNode *node_source = node->childs[1];
	assert(node_source->type == BE_NODE_LITERAL_STRING);
}

static void _stat_label(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_LABEL);
	assert(node->nchilds == 1);
	assert(node->childs[0]->type == BE_NODE_IDENTIFIER);

	// 什么也不做。
}

static void _stat_br(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_BR);
	assert(node->nchilds == 1);

	ParserASTNode *node_id = node->childs[0];
	assert(node_id->type == BE_NODE_IDENTIFIER);

	if (!parser_get_symbol_from_node(ctx, node, BE_SYM_LABEL, node_id->token)) {
		_SYNERR_TOKEN(
			ctx,
			node_id->token,
			"undefined label."
		);
	}
}

static void _stat_cbr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_CBR);
	assert(node->nchilds == 3);

	ParserASTNode *node_cond = node->childs[0];
	_GetParamInfoResult result_cond;
	_get_param_info(ctx, node_cond, &result_cond);
	if (!_is_integer_type(result_cond.type)
			&& !_is_pointer_type(result_cond.type)) {
		_SYNERR_NODE(
			ctx,
			node_cond,
			"condition parameter type must be integer type or pointer type."
		);
	}

	ParserASTNode *node_label_true = node->childs[1];
	assert(node_label_true->type == BE_NODE_IDENTIFIER);
	if (!parser_get_symbol_from_node(ctx, node, BE_SYM_LABEL, node_label_true->token)) {
		_SYNERR_TOKEN(
			ctx,
			node_label_true->token,
			"undefined label."
		);
	}

	ParserASTNode *node_label_false = node->childs[2];
	assert(node_label_false->type == BE_NODE_IDENTIFIER);
	if (!parser_get_symbol_from_node(ctx, node, BE_SYM_LABEL, node_label_false->token)) {
		_SYNERR_TOKEN(
			ctx,
			node_label_false->token,
			"undefined label."
		);
	}
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
		if (type == BE_NODE_INVALID) {
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

static void _stat_return(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_RETURN);

	ParserASTNode *node_func = _search_node_along_parent(node->parent, BE_NODE_FUNC, 0);
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
				"statement 'return' requires return value."
			);
		}
	} else if (node->nchilds == 1) {
		if (node_return_type == NULL) {
			_SYNERR_NODE(
				ctx,
				node,
				"statement 'return' does not require return value."
			);
		}

		ParserASTNode *node_ret_val = node->childs[0];
		_GetParamInfoResult result_ret_val;
		_get_param_info(ctx, node_ret_val, &result_ret_val);
		_check_param_combination2(ctx, node, node_return_type, result_ret_val.node_type);
	} else {
		assert(0);
	}
}

static void _stat_ref(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_REF);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	if (!_is_pointer_type(BE_VAR_SYMBOL_GET_TYPE(symbol_target))) {
		_SYNERR_NODE(ctx, node_target, "target parameter type must be pointer type.");
	}

	ParserASTNode *node_source = node->childs[1];
	assert(node_source->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);
	ParserASTNode *node_type_source = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_source);
	_check_param_combination2(
		ctx,
		node,
		node_type_target->childs[0],
		node_type_source
	);
}

static void _stat_store(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_STORE);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	_GetParamInfoResult result_target;
	_get_param_info(ctx, node_target, &result_target);

	ParserASTNode *node_source = node->childs[1];
	_GetParamInfoResult result_source;
	_get_param_info(ctx, node_source, &result_source);

	if (!_is_pointer_type(result_target.type)) {
		_SYNERR_NODE(ctx, node_target, "target parameter type must be pointer type.");
	}

	_check_param_combination2(
		ctx,
		node,
		result_target.node_type->childs[0],
		result_source.node_type
	);
}

static void _stat_load(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_LOAD);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);

	ParserASTNode *node_source = node->childs[1];
	_GetParamInfoResult result_source;
	_get_param_info(ctx, node_source, &result_source);

	if (!_is_pointer_type(result_source.type)) {
		_SYNERR_NODE(ctx, node_source, "source parameter type must be pointer type.");
	}

	_check_param_combination2(
		ctx,
		node,
		node_type_target,
		result_source.node_type->childs[0]
	);
}

typedef struct {
	ParserASTNode		*node_target;
	ParserASTNode		*node_type_target;
	uint8_t				type_target;
	size_t				size_target;

	ParserASTNode		*node_source;
	ParserASTNode		*node_type_source;
	uint8_t				type_source;
	size_t				size_source;
} _ResultCheckStat_I_CI;

static void _check_stat_i_ci(
	ParserContext *ctx,
	ParserASTNode *node,
	uint32_t node_type,
	_ResultCheckStat_I_CI *result
) {
	assert(ctx);
	assert(node);
	assert(node->type == node_type);
	assert(node->nchilds == 2);
	assert(result);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);

	ParserASTNode *node_source = node->childs[1];
	_GetParamInfoResult result_source;
	_get_param_info(ctx, node_source, &result_source);

	result->node_target = node_target;
	result->node_type_target = node_type_target;
	result->type_target = _get_type_by_type_node(ctx, node_type_target);
	result->size_target = _calc_type_size(ctx, node, node_type_target);

	result->node_source = node_source;
	result->node_type_source = result_source.node_type;
	result->type_source = result_source.type;
	result->size_source = _calc_type_size(ctx, node, result_source.node_type);
}

typedef struct {
	ParserASTNode		*node_target;
	ParserASTNode		*node_type_target;
	uint8_t				type_target;
	size_t				size_target;

	ParserASTNode		*node_source_left;
	ParserASTNode		*node_type_source_left;
	uint8_t				type_source_left;
	size_t				size_source_left;

	ParserASTNode		*node_source_right;
	ParserASTNode		*node_type_source_right;
	uint8_t				type_source_right;
	size_t				size_source_right;
} _ResultCheckStat_I_CI_CI;

static void _check_stat_i_ci_ci(
	ParserContext *ctx,
	ParserASTNode *node,
	uint32_t node_type,
	_ResultCheckStat_I_CI_CI *result
) {
	assert(ctx);
	assert(node);
	assert(node->type == node_type);
	assert(node->nchilds == 3);
	assert(result);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);

	ParserASTNode *node_source_left = node->childs[1];
	_GetParamInfoResult result_source_left;
	_get_param_info(ctx, node_source_left, &result_source_left);

	ParserASTNode *node_source_right = node->childs[2];
	_GetParamInfoResult result_source_right;
	_get_param_info(ctx, node_source_right, &result_source_right);

	result->node_target = node_target;
	result->node_type_target = node_type_target;
	result->type_target = _get_type_by_type_node(ctx, node_type_target);
	result->size_target = _calc_type_size(ctx, node, node_type_target);

	result->node_source_left = node_source_left;
	result->node_type_source_left = result_source_left.node_type;
	result->type_source_left =  result_source_left.type;
	result->size_source_left = _calc_type_size(ctx, node,  result_source_left.node_type);

	result->node_source_right = node_source_right;
	result->node_type_source_right = result_source_right.node_type;
	result->type_source_right = result_source_right.type;
	result->size_source_right = _calc_type_size(ctx, node, result_source_right.node_type);
}




static void _stat_trunc(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_TRUNC, &check_result);

	if (!_is_integer_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be integer type.");
	}
	if (!_is_integer_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be integer type.");
	}

	if (!(check_result.size_source > check_result.size_target)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter size must be larger than target parameter size.");
	}
}

static void _stat_sext(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_SEXT, &check_result);

	if (!_is_integer_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be integer type.");
	}
	if (!_is_integer_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be integer type.");
	}

	if (!(check_result.size_source < check_result.size_target)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter size must be smaller than target parameter size.");
	}
}

static void _stat_sitofp(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_SITOFP, &check_result);

	if (!_is_float_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be float type.");
	}
	if (!_is_signed_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be signed integer type.");
	}
}

static void _stat_inttoptr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_INTTOPTR, &check_result);

	if (!_is_pointer_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be pointer type.");
	}
	if (!_is_integer_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be integer type.");
	}
}

static void _stat_zext(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_ZEXT, &check_result);

	if (!_is_integer_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be integer type.");
	}
	if (!_is_integer_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be integer type.");
	}

	if (!(check_result.size_source < check_result.size_target)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter size must be smaller than target parameter size.");
	}
}

static void _stat_uitofp(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_UITOFP, &check_result);

	if (!_is_float_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be float type.");
	}
	if (!_is_unsigned_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be unsigned integer type.");
	}
}

static void _stat_fptosi(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_FPTOSI, &check_result);

	if (!_is_signed_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be signed integer type.");
	}
	if (!_is_float_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be float type.");
	}
}

static void _stat_fptoui(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_FPTOUI, &check_result);

	if (!_is_unsigned_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be unsigned integer type.");
	}
	if (!_is_float_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be float type.");
	}
}

static void _stat_fpext(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_FPEXT, &check_result);

	if (check_result.type_target != BE_TYPE_DOUBLE) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be double type.");
	}
	if (check_result.type_source != BE_TYPE_FLOAT) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be float type.");
	}
}

static void _stat_fptrunc(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_FPTRUNC, &check_result);

	if (check_result.type_target != BE_TYPE_FLOAT) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be float type.");
	}
	if (check_result.type_source != BE_TYPE_DOUBLE) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be double type.");
	}
}

static void _stat_ptrtoint(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_PTRTOINT, &check_result);

	if (!_is_integer_type(check_result.type_target)) {
		_SYNERR_NODE(ctx, check_result.node_target, "target parameter type must be integer type.");
	}
	if (!_is_pointer_type(check_result.type_source)) {
		_SYNERR_NODE(ctx, check_result.node_source, "source parameter type must be pointer type.");
	}
}

static void _stat_bitcast(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_BITCAST, &check_result);

	if (!_is_primitive_type(check_result.type_target)
			&& !_is_pointer_type(check_result.type_target)) {
		_SYNERR_NODE(
			ctx,
			check_result.node_target,
			"target parameter type must be primitive type or pointer type."
		);
	}
	if (!_is_primitive_type(check_result.type_source)
			&& !_is_pointer_type(check_result.type_source)) {
		_SYNERR_NODE(
			ctx,
			check_result.node_target,
			"source parameter type must be primitive type or pointer type."
		);
	}
	if (check_result.size_target != check_result.size_source) {
		_SYNERR_NODE(
			ctx,
			check_result.node_target,
			"target parameter size and source parameter size must be same."
		);
	}
}

static void _check_fncall_params(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_func_sym_params,
	ParserASTNode *node_func_params
) {
	assert(ctx);
	assert(node);
	assert(node_func_sym_params);
	assert(node_func_params);

	bool func_param_count_matched = false;

	if (node_func_params->nchilds == node_func_sym_params->nchilds) {
		func_param_count_matched = true;
	}

	if (node_func_sym_params->nchilds > 0
			&& node_func_sym_params->childs[node_func_sym_params->nchilds - 1]->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM
			&& node_func_params->nchilds >= node_func_sym_params->nchilds - 1) {
		func_param_count_matched = true;
	}

	if (func_param_count_matched) {
		int i;
		for (i = 0; i < node_func_sym_params->nchilds; i++) {
			ParserASTNode *node_func_sym_param = node_func_sym_params->childs[i];

			if (node_func_sym_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				break;
			}

			ParserASTNode *node_func_param = node_func_params->childs[i];
			_GetParamInfoResult result_func_param;
			_get_param_info(ctx, node_func_param, &result_func_param);
			ParserASTNode *node_func_param_type = result_func_param.node_type;

			ParserASTNode *node_func_sym_param_type = node_func_sym_param->childs[1];
			if (!_is_compatible_type_for_assign(ctx, node_func_sym_param_type, node_func_param_type, false)) {
				_SYNERR_NODE(ctx, node, "function call parameter type incompatible.");
			}
		}

		for (; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_func_param = node_func_params->childs[i];
			_GetParamInfoResult result_func_param;
			_get_param_info(ctx, node_func_param, &result_func_param);
			ParserASTNode *node_func_param_type = result_func_param.node_type;

			uint8_t param_type = _get_type_by_type_node(ctx, node_func_param_type);
			if (param_type == BE_TYPE_STRUCT
					|| param_type == BE_TYPE_ARRAY
					|| param_type == BE_TYPE_FUNC) {
				_SYNERR_NODE(
					ctx, node, "parameter type cannot be struct type or array type or function type operand."
				);
			}
		}
	} else {
		_SYNERR_NODE(ctx, node, "function call parameter count not matched.");
	}
}

static ParserASTNode * _stat_func_call(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_FUNC_CALL);
	assert(node->nchilds == 2);

	ParserASTNode *node_func = node->childs[0];
	ParserASTNode *node_func_pointer_type = NULL;
	if (node_func->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol_func = _get_symbol_by_id_node(ctx, node_func);
		if (symbol_func->type == BE_SYM_FUNC) {
			node_func_pointer_type = BE_FUNC_SYMBOL_GET_FUNC_POINTER_TYPE_NODE(symbol_func);
		} else if (symbol_func->type == BE_SYM_VAR) {
			node_func_pointer_type = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_func);
		} else {
			_SYNERR_NODE(ctx, node_func, "must be a function or variable.");
		}
	} else if (node_func->type == BE_NODE_EXPR) {
		_expr_wrapper(ctx, node_func);
		node_func_pointer_type = BE_EXPR_AST_NODE_GET_TYPE_NODE(node_func);
	} else {
		assert(0);
	}
	if (node_func_pointer_type->type != BE_NODE_TYPE_POINTER
			|| node_func_pointer_type->childs[0]->type != BE_NODE_TYPE_FUNC) {
		_SYNERR_NODE(ctx, node_func, "must be function pointer type.");
	}
	ParserASTNode *node_func_type = node_func_pointer_type->childs[0];
	ParserASTNode *node_func_params = node_func_type->childs[0];

	ParserASTNode *node_func_call_params = node->childs[1];
	_check_fncall_params(ctx, node, node_func_params, node_func_call_params);

	if (node_func_type->nchilds == 1) {
		return _NODE_TYPE_VOID;
	} else if (node_func_type->nchilds == 2) {
		return node_func_type->childs[1];
	} else {
		assert(0);
		return NULL;
	}
}

static void _stat_vcall(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_VCALL);
	assert(node->nchilds == 1);

	ParserASTNode *node_func_call = node->childs[0];
	ParserASTNode *node_ret_type = _stat_func_call(ctx, node_func_call);
	assert(node_ret_type);
	if (node_ret_type->type != BE_NODE_TYPE_VOID) {
		_SYNERR_NODE(
			ctx,
			node,
			"function have return value, maybe need to use 'call'."
		);
	}
}

static void _stat_call(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_CALL);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);

	ParserASTNode *node_func_call = node->childs[1];
	ParserASTNode *node_ret_type = _stat_func_call(ctx, node_func_call);

	_check_param_combination2(
		ctx,
		node,
		node_type_target,
		node_ret_type
	);
}

static void _stat_add(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_ADD, &check_result);

	if (_is_pointer_type(check_result.type_target)
			&& _is_pointer_type(check_result.type_source_left)
			&& _is_integer_type(check_result.type_source_right)) {
		goto ok;
	}

	if (_is_number_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_sub(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_SUB, &check_result);

	if (_is_pointer_type(check_result.type_target)
			&& _is_pointer_type(check_result.type_source_left)
			&& _is_integer_type(check_result.type_source_right)) {
		goto ok;
	}

	if (_is_number_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_mul(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_MUL, &check_result);

	if (_is_number_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_div(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_DIV, &check_result);

	if (_is_number_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_rem(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_REM, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_mbr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_MBR);
	assert(node->nchilds == 3);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	if (node_type_target->type != BE_NODE_TYPE_POINTER) {
		goto err;
	}

	ParserASTNode *node_source_left = node->childs[1];
	_GetParamInfoResult result_source_left;
	_get_param_info(ctx, node_source_left, &result_source_left);
	ParserASTNode *node_type_source_left = result_source_left.node_type;
	if (node_type_source_left->type != BE_NODE_TYPE_POINTER) {
		goto err;
	}
	if (node_type_source_left->childs[0]->type != BE_NODE_TYPE_STRUCT) {
		goto err;
	}
	ParserASTNode *node_struct_id = node_type_source_left->childs[0]->childs[0];

	ParserASTNode *node_source_right = node->childs[2];
	assert(node_source_right->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_struct_member = _get_struct_member_symbol(
		ctx, node_struct_id, node_source_right
	);
	ParserASTNode *node_type_struct_member = BE_STRUCT_MEMBER_VAR_SYMBOL_GET_TYPE_NODE(symbol_struct_member);
	if (!_is_compatible_type(ctx, node_type_target->childs[0], node_type_struct_member, true)) {
		goto err;
	}

	return;
err:
	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);
}

static bool _check_stat_idx_params(
	ParserContext *ctx,
	ParserASTNode *node,
	_ResultCheckStat_I_CI_CI *check_result
) {
	assert(ctx);
	assert(node);
	assert(check_result);

	if (_get_type_by_type_node(ctx, check_result->node_type_target) != BE_TYPE_POINTER
			|| _get_type_by_type_node(ctx, check_result->node_type_source_left) != BE_TYPE_POINTER) {
		return false;
	}

	ParserASTNode *target = check_result->node_type_target->childs[0];
	ParserASTNode *source = check_result->node_type_source_left->childs[0];

	uint8_t type_target = _get_type_by_type_node(ctx, target);
	uint8_t type_source = _get_type_by_type_node(ctx, source);

	if (type_target == BE_TYPE_ARRAY && type_source == BE_TYPE_ARRAY) {
		ParserASTNode *target_dims = target->childs[0];
		ParserASTNode *target_type = target->childs[1];
		ParserASTNode *source_dims = source->childs[0];
		ParserASTNode *source_type = source->childs[1];

		if (!_is_compatible_type(ctx, target_type, source_type, true)) {
			return false;
		}

		if (target_dims->nchilds + 1 != source_dims->nchilds) {
			return false;
		}

		for (int i = 0; i < target_dims->nchilds; i++) {
			ParserASTNode *target_dim = target_dims->childs[i];
			ParserASTNode *source_dim = source_dims->childs[1 + i];
			if (be_parser_get_uint_val(ctx, target_dim) != be_parser_get_uint_val(ctx, source_dim)) {
				return false;
			}
		}

		return true;
	} else if (type_target != BE_TYPE_ARRAY && type_source == BE_TYPE_ARRAY) {
		ParserASTNode *source_dims = source->childs[0];
		ParserASTNode *source_type = source->childs[1];

		if (!_is_compatible_type(ctx, target, source_type, true)) {
			return false;
		}

		if (source_dims->nchilds != 1) {
			return false;
		}

		return true;
	} else {
		return false;
	}
}

static void _stat_idx(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_IDX, &check_result);

	if (_is_pointer_type(check_result.type_target)
			&& _is_pointer_type(check_result.type_source_left)
			&& check_result.node_type_source_left->childs[0]->type == BE_NODE_TYPE_ARRAY
			&& _check_stat_idx_params(ctx, node, &check_result)
			&& _is_integer_type(check_result.type_source_right)
	) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_not(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_NOT, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_neg(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_NEG, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_bnot(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI check_result;
	_check_stat_i_ci(ctx, node, BE_NODE_STAT_BNOT, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_shl(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_SHL, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& _is_integer_type(check_result.type_source_right)) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_shr(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_SHR, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& _is_integer_type(check_result.type_source_right)) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_eq(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_EQ, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& (_is_number_type(check_result.type_source_left) || _is_pointer_type(check_result.type_source_left))
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_neq(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_NEQ, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& (_is_number_type(check_result.type_source_left) || _is_pointer_type(check_result.type_source_left))
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_lt(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_LT, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& _is_number_type(check_result.type_source_left)
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_le(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_LE, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& _is_number_type(check_result.type_source_left)
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_gt(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_GT, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& _is_number_type(check_result.type_source_left)
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_ge(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_GE, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& _is_number_type(check_result.type_source_left)
			&& check_result.type_source_left == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_band(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_BAND, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_bor(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_BOR, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static void _stat_bxor(
	ParserContext *ctx,
	ParserASTNode *node
) {
	_ResultCheckStat_I_CI_CI check_result;
	_check_stat_i_ci_ci(ctx, node, BE_NODE_STAT_BXOR, &check_result);

	if (_is_integer_type(check_result.type_target)
			&& check_result.type_target == check_result.type_source_left
			&& check_result.type_target == check_result.type_source_right) {
		goto ok;
	}

	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);

ok:
	return;	
}

static bool _is_va_list_pointer(
	ParserASTNode *type_node
) {
	assert(type_node);

	return type_node->type == BE_NODE_TYPE_POINTER
			&& type_node->childs[0]->type == BE_NODE_TYPE_VA_LIST;
}

static void _stat_va_start(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_VA_START);
	assert(node->nchilds == 1);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	if (!_is_va_list_pointer(node_type_target)) {
		goto err;
	}

	return;
err:
	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);
}

static void _stat_va_end(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_VA_END);
	assert(node->nchilds == 1);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	if (!_is_va_list_pointer(node_type_target)) {
		goto err;
	}

	return;
err:
	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);
}

static void _stat_va_copy(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_VA_COPY);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	if (!_is_va_list_pointer(node_type_target)) {
		goto err;
	}

	ParserASTNode *node_source = node->childs[1];
	assert(node_source->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);
	ParserASTNode *node_type_source = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_source);
	if (!_is_va_list_pointer(node_type_source)) {
		goto err;
	}

	return;
err:
	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);
}

static void _stat_va_arg(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_VA_ARG);
	assert(node->nchilds == 3);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	ParserASTNode *node_type_target = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_target);
	
	ParserASTNode *node_type = node->childs[1];
	if (!_is_compatible_type(ctx, node_type_target, node_type, true)) {
		goto err;
	}
	uint8_t type = _get_type_by_type_node(ctx, node_type);
	if (!_is_primitive_type(type)
			&& !_is_pointer_type(type)) {
		goto err;
	}

	ParserASTNode *node_source = node->childs[2];
	assert(node_source->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);
	ParserASTNode *node_type_source = BE_VAR_SYMBOL_GET_TYPE_NODE(symbol_source);
	if (!_is_va_list_pointer(node_type_source)) {
		goto err;
	}

	return;
err:
	_SYNERR_NODE(
		ctx,
		node,
		"invalid parameter combination."
	);
}




static void _stat_dummy(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_DUMMY);

	// 什么也不干。
}

static void _stat(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case BE_NODE_VAR: {
			_stat_var(ctx, node);
			break;
		}
		case BE_NODE_STAT_ASSIGN: {
			_stat_assign(ctx, node);
			break;
		}

		case BE_NODE_STAT_ASM: {
			_stat_asm(ctx, node);
			break;
		}
		case BE_NODE_STAT_ASM_SET_REG: {
			_stat_asm_set_reg(ctx, node);
			break;
		}
		case BE_NODE_STAT_ASM_GET_REG: {
			_stat_asm_get_reg(ctx, node);
			break;
		}

		case BE_NODE_STAT_LABEL: {
			_stat_label(ctx, node);
			break;
		}
		case BE_NODE_STAT_BR: {
			_stat_br(ctx, node);
			break;
		}
		case BE_NODE_STAT_CBR: {
			_stat_cbr(ctx, node);
			break;
		}

		case BE_NODE_STAT_RETURN: {
			_stat_return(ctx, node);
			break;
		}

		case BE_NODE_STAT_REF: {
			_stat_ref(ctx, node);
			break;
		}
		case BE_NODE_STAT_STORE: {
			_stat_store(ctx, node);
			break;
		}
		case BE_NODE_STAT_LOAD: {
			_stat_load(ctx, node);
			break;
		}

		case BE_NODE_STAT_TRUNC: {
			_stat_trunc(ctx, node);
			break;
		}
		case BE_NODE_STAT_SEXT: {
			_stat_sext(ctx, node);
			break;
		}
		case BE_NODE_STAT_SITOFP: {
			_stat_sitofp(ctx, node);
			break;
		}
		case BE_NODE_STAT_INTTOPTR: {
			_stat_inttoptr(ctx, node);
			break;
		}
		case BE_NODE_STAT_ZEXT: {
			_stat_zext(ctx, node);
			break;
		}
		case BE_NODE_STAT_UITOFP: {
			_stat_uitofp(ctx, node);
			break;
		}
		case BE_NODE_STAT_FPTOSI: {
			_stat_fptosi(ctx, node);
			break;
		}
		case BE_NODE_STAT_FPTOUI: {
			_stat_fptoui(ctx, node);
			break;
		}
		case BE_NODE_STAT_FPEXT: {
			_stat_fpext(ctx, node);
			break;
		}
		case BE_NODE_STAT_FPTRUNC: {
			_stat_fptrunc(ctx, node);
			break;
		}
		case BE_NODE_STAT_PTRTOINT: {
			_stat_ptrtoint(ctx, node);
			break;
		}
		case BE_NODE_STAT_BITCAST: {
			_stat_bitcast(ctx, node);
			break;
		}

		case BE_NODE_STAT_VCALL: {
			_stat_vcall(ctx, node);
			break;
		}
		case BE_NODE_STAT_CALL: {
			_stat_call(ctx, node);
			break;
		}

		case BE_NODE_STAT_ADD: {
			_stat_add(ctx, node);
			break;
		}
		case BE_NODE_STAT_SUB: {
			_stat_sub(ctx, node);
			break;
		}
		case BE_NODE_STAT_MUL: {
			_stat_mul(ctx, node);
			break;
		}
		case BE_NODE_STAT_DIV: {
			_stat_div(ctx, node);
			break;
		}
		case BE_NODE_STAT_REM: {
			_stat_rem(ctx, node);
			break;
		}

		case BE_NODE_STAT_MBR: {
			_stat_mbr(ctx, node);
			break;
		} 
		case BE_NODE_STAT_IDX: {
			_stat_idx(ctx, node);
			break;
		} 
		case BE_NODE_STAT_NOT: {
			_stat_not(ctx, node);
			break;
		} 
		case BE_NODE_STAT_NEG: {
			_stat_neg(ctx, node);
			break;
		} 
		case BE_NODE_STAT_BNOT: {
			_stat_bnot(ctx, node);
			break;
		} 

		case BE_NODE_STAT_SHL: {
			_stat_shl(ctx, node);
			break;
		}
		case BE_NODE_STAT_SHR: {
			_stat_shr(ctx, node);
			break;
		}

		case BE_NODE_STAT_EQ: {
			_stat_eq(ctx, node);
			break;
		}
		case BE_NODE_STAT_NEQ: {
			_stat_neq(ctx, node);
			break;
		}
		case BE_NODE_STAT_LT: {
			_stat_lt(ctx, node);
			break;
		}
		case BE_NODE_STAT_LE: {
			_stat_le(ctx, node);
			break;
		}
		case BE_NODE_STAT_GT: {
			_stat_gt(ctx, node);
			break;
		}
		case BE_NODE_STAT_GE: {
			_stat_ge(ctx, node);
			break;
		}

		case BE_NODE_STAT_BAND: {
			_stat_band(ctx, node);
			break;
		}
		case BE_NODE_STAT_BOR: {
			_stat_bor(ctx, node);
			break;
		}
		case BE_NODE_STAT_BXOR: {
			_stat_bxor(ctx, node);
			break;
		}

		case BE_NODE_STAT_VA_START: {
			_stat_va_start(ctx, node);
			break;
		}
		case BE_NODE_STAT_VA_END: {
			_stat_va_end(ctx, node);
			break;
		}
		case BE_NODE_STAT_VA_COPY: {
			_stat_va_copy(ctx, node);
			break;
		}
		case BE_NODE_STAT_VA_ARG: {
			_stat_va_arg(ctx, node);
			break;
		}





		case BE_NODE_STAT_DUMMY: {
			_stat_dummy(ctx, node);
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
	assert(node->type == BE_NODE_STATS_BLOCK);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *child = node->childs[i];

		_stat(ctx, child);
	}
}

static void __func_process_label(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *current
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STATS_BLOCK);
	assert(current);

	if (current->type == BE_NODE_STAT_LABEL) {
		parser_add_symbol_to_node(
			ctx,
			node,
			BE_SYM_LABEL,
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
	assert(node->type == BE_NODE_STATS_BLOCK);

	__func_process_label(ctx, node, node);
}

static void _func(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_FUNC);

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
			&& node_func_params->childs[0]->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
		_SYNERR_NODE(
			ctx,
			node_func_params->childs[0],
			"first function parameter cannot be '...'."
		);
	}

	for (int i = 0; i < node_func_params->nchilds; i++) {
		ParserASTNode *node_func_param = node_func_params->childs[i];
		if (node_func_param->type == BE_NODE_FUNC_PARAMS_ITEM) {
			ParserASTNode *node_func_param_id = node_func_param->childs[0];
			ParserASTNode *node_func_param_type = node_func_param->childs[1];

			uint8_t param_type = 0;
			_type(ctx, node_func_param_type, &param_type);
			if (!_is_primitive_type(param_type)
					&& !_is_pointer_type(param_type)
					&& param_type != BE_TYPE_VA_LIST) {
				_SYNERR_NODE(
					ctx,
					node_func_param_type,
					"function parameter type must be primitive type or pointer type or __va_list type."
				);
			}

			size_t type_size = _calc_type_size(ctx, node, node_func_param_type);

			ParserSymbol *symbol = be_parser_add_var_symbol_to_node(
				ctx,
				node,
				node_func_param_id->token,
				param_type,
				node_func_param_type,
				type_size
			);

			BE_VAR_SYMBOL_SET_SCOPE_TYPE(symbol, BE_VAR_TYPE_FUNC_PARAM);

			BE_FUNC_PARAM_AST_NODE_SET_SYMBOL(node_func_param, symbol);
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
	if (node_body->type == BE_NODE_DUMMY) {
		symbol = be_parser_add_func_symbol_to_node(
			ctx, node_parent, node_identifier->token, node_func_params, node_type, true
		);

		BE_FUNC_AST_NODE_SET_SYMBOL(node, symbol);
	} else {
		symbol = parser_get_symbol_from_node(
			ctx, node_parent, BE_SYM_FUNC, node_identifier->token
		);

		if (symbol != NULL
				&& BE_FUNC_SYMBOL_GET_DUMMY(symbol)) {
			ParserASTNode *func_type_node = _NEW_NODE(ctx, BE_NODE_TYPE_FUNC, NULL);
			parser_add_child(ctx, func_type_node, node_func_params);
			if (node_type != NULL) {
				parser_add_child(ctx, func_type_node, node_type);
			}

			if (!_is_compatible_func_type(ctx, func_type_node, BE_FUNC_SYMBOL_GET_FUNC_TYPE_NODE(symbol), true)) {
				_SYNERR_TOKEN(
					ctx,
					node_identifier->token,
					"function declaration signature and function implementation signature are not matched."
				);
			}

			BE_FUNC_SYMBOL_SET_DUMMY(symbol, false);
		} else {
			symbol = be_parser_add_func_symbol_to_node(
				ctx, node_parent, node_identifier->token, node_func_params, node_type, false
			);
		}

		assert(symbol);

		BE_FUNC_SYMBOL_SET_FUNC_NAME_NODE(symbol, node_identifier);

		BE_FUNC_AST_NODE_SET_SYMBOL(node, symbol);

		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_func_param = node_func_params->childs[i];
			if (node_func_param->type == BE_NODE_FUNC_PARAMS_ITEM) {
				BE_VAR_SYMBOL_SET_FUNC_SYMBOL(
					BE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_func_param),
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
		case BE_NODE_VAR: {
			_var(ctx, node);
			break;
		}
		case BE_NODE_STRUCT: {
			_struct(ctx, node);
			break;
		}
		case BE_NODE_FUNC: {
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

void be_sem_process(
	ParserContext *ctx
) {
	assert(ctx);
	assert(ctx->ast);

	ParserASTNode *node_module = ctx->ast;
	_module(ctx, node_module);
}




static void _print_type_node(ParserASTNode *node, FILE *file);

static void _print_func_params(ParserASTNode *node, FILE *file) {
	assert(node);
	assert(file);

	if (node->nchilds > 0) {
		ParserASTNode *node_param = node->childs[0];
		assert(node_param->type == BE_NODE_FUNC_PARAMS_ITEM);

		#define _NODE_PARAM	\
			switch (node_param->type) {	\
				case BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM: {	\
					fputs("...", file);	\
					break;	\
				}	\
				case BE_NODE_FUNC_PARAMS_ITEM: {	\
					assert(node_param->nchilds == 2);	\
					ParserASTNode *node_id = node_param->childs[0];	\
					assert(node_id->type == BE_NODE_IDENTIFIER);	\
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
			assert(node_param->type == BE_NODE_FUNC_PARAMS_ITEM
					|| node_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM);

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
		case BE_NODE_TYPE_CHAR: {
			assert(node->nchilds == 0);
			fputs("char", file);
			break;
		}
		case BE_NODE_TYPE_INT8: {
			assert(node->nchilds == 0);
			fputs("int8", file);
			break;
		}
		case BE_NODE_TYPE_UINT8: {
			assert(node->nchilds == 0);
			fputs("uint8", file);
			break;
		}
		case BE_NODE_TYPE_INT16: {
			assert(node->nchilds == 0);
			fputs("int16", file);
			break;
		}
		case BE_NODE_TYPE_UINT16: {
			assert(node->nchilds == 0);
			fputs("uint16", file);
			break;
		}
		case BE_NODE_TYPE_INT32: {
			assert(node->nchilds == 0);
			fputs("int32", file);
			break;
		}
		case BE_NODE_TYPE_UINT32: {
			assert(node->nchilds == 0);
			fputs("uint32", file);
			break;
		}
		case BE_NODE_TYPE_INT64: {
			assert(node->nchilds == 0);
			fputs("int64", file);
			break;
		}
		case BE_NODE_TYPE_UINT64: {
			assert(node->nchilds == 0);
			fputs("uint64", file);
			break;
		}
		case BE_NODE_TYPE_FLOAT: {
			assert(node->nchilds == 0);
			fputs("float", file);
			break;
		}
		case BE_NODE_TYPE_DOUBLE: {
			assert(node->nchilds == 0);
			fputs("double", file);
			break;
		}
		case BE_NODE_TYPE_STRUCT: {
			assert(node->nchilds == 1);

			fputs("struct ", file);

			ParserASTNode *node_id = node->childs[0];
			assert(node_id->type == BE_NODE_IDENTIFIER);

			cmn_print_token(node_id->token, file);
			break;
		}
		case BE_NODE_TYPE_ARRAY: {
			assert(node->nchilds == 2);
			ParserASTNode *node_array_dims = node->childs[0];
			assert(node_array_dims->type == BE_NODE_TYPE_ARRAY_DIMS);
			ParserASTNode *node_t = node->childs[1];

			for (int i = 0; i < node_array_dims->nchilds; i++) {
				ParserASTNode *node_array_dim = node_array_dims->childs[i];
				assert(node_array_dim->type == BE_NODE_LITERAL_UINT);
				fputc('[', file);
				cmn_print_token(node_array_dim->token, file);
				fputc(']', file);
			}

			_print_type_node(node_t, file);

			break;
		}
		case BE_NODE_TYPE_FUNC: {
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
		case BE_NODE_TYPE_POINTER: {
			assert(node->nchilds == 1);
			ParserASTNode *node_t = node->childs[0];

			fputc('*', file);

			_print_type_node(node_t, file);
			break;
		}
		case BE_NODE_TYPE_VOID: {
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

	if (BE_IS_EXPR_NODE(node->type)) {
		for (int i = 0; i < level; i++) {
			fputc(padchr, file);
		}
		fputs("|ExprNodeType: ", file);
		ParserASTNode *node_t = BE_EXPR_AST_NODE_GET_TYPE_NODE(node);
		if (node_t != NULL) {
			_print_type_node(node_t, file);
		}
		fputc('\n', file);
	}

	for (int i = 0; i < node->nchilds; i++) {
		_print_ast(node->childs[i], file, level + 1, padchr);
	}
}

void be_sem_parser_print_ast(
	ParserContext * ctx,
	FILE *file
) {
	assert(ctx);
	assert(ctx->ast);
	assert(file);

	_print_ast(ctx->ast, file, 0, '\t');
}

size_t be_sem_calc_type_size(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(node);
	assert(node_type);

	return _calc_type_size(ctx, node, node_type);
}

bool be_sem_is_signed_type(
	uint8_t type
) {
	return _is_signed_type(type);
}

bool be_sem_is_unsigned_type(
	uint8_t type
) {
	return _is_unsigned_type(type);
}

bool be_sem_is_integer_type(
	uint8_t type
) {
	return _is_integer_type(type);
}

bool be_sem_is_float_type(
	uint8_t type
) {
	return _is_float_type(type);
}

bool be_sem_is_primitive_type(
	uint8_t type
) {
	return _is_primitive_type(type);
}

bool be_sem_is_pointer_type(
	uint8_t type
) {
	return _is_pointer_type(type);
}

bool be_sem_is_number_type(
	uint8_t type
) {
	return _is_number_type(type);
}

size_t be_sem_get_primitive_type_size(
	uint8_t type
) {
	return _get_primitive_type_size(type);
}

ParserSymbol * be_sem_get_struct_member_symbol(
	ParserContext *ctx,
	ParserASTNode *node_struct_id,
	ParserASTNode *node_struct_member_id
) {
	return _get_struct_member_symbol(ctx, node_struct_id, node_struct_member_id);
}
