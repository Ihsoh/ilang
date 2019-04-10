#include "parser.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "lexer.h"
#include "parser.h"

#include "../../lexer.h"
#include "../../parser.h"

static ParserASTNode * _new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
) {
	assert(ctx);
	assert(type_name);

	return parser_new_node(
		ctx, type, type_name, token, 0, NULL
	);
}

#define	_NEW_NODE(ctx, type, token)	(_new_node((ctx), type, #type, (token)))

#define	_RULE_NAME(name)	_rule_##name

#define	_RULE_FUNC_DECL(name)	static ParserASTNode * _RULE_NAME(name)(ParserContext *__ctx)

#define	_RULE(name)	\
	_RULE_FUNC_DECL(name) {	\
		assert(__ctx);	\
		assert(__ctx->lexctx);	\
		ParserASTNode *__node = NULL;	\
		lexer_store_current(__ctx->lexctx);	\
		LexerToken *__token = NULL;	\
		{

#define	_RULE_END	\
		}	\
		lexer_abandon_curstack_top(__ctx->lexctx);	\
		return __node;	\
	__not_matched:	\
		lexer_resume_current(__ctx->lexctx);	\
		return NULL;	\
	}

#define	_RULE_PARSER_CTX	(__ctx)
#define	_RULE_LEXER_CTX		(__ctx->lexctx)

#define	_RULE_TOKEN			(__token)
#define	_RULE_TOKEN_TYPE	(__token == NULL ? BE_TOKEN_INVALID : __token->type)

#define	_RULE_PREV_TOKEN	{ __token = lexer_prev_token(__ctx->lexctx); __ctx->current_token = __token; }
#define	_RULE_NEXT_TOKEN	{ __token = lexer_next_token(__ctx->lexctx); __ctx->current_token = __token; }

#define	_RULE_PUSH_LEXCTX	{ lexer_store_current(__ctx->lexctx); }
#define	_RULE_POP_LEXCTX	{ lexer_resume_current(__ctx->lexctx); }
#define	_RULE_ABANDON_LEXCTX	{ lexer_abandon_curstack_top(__ctx->lexctx); }

#define	_RULE_NODE(type, token)	{	\
	__node = _new_node(__ctx, type, #type, (token));	\
	__ctx->previous_node = __ctx->last_node;	\
	__ctx->last_node = __node;	\
}
#define	_RULE_CURRENT_NODE		(__node)
#define	_RULE_LAST_NODE			(__node->last_node)
#define	_RULE_PREVIOUS_NODE		(__node->previous_node)
#define	_RULE_RETURNED_NODE(node)	{ __node = (node); }
#define	_RULE_NEW_NODE(type, token)	(_new_node(__ctx, type, #type, (token)))
#define	_RULE_ADD_CHILD(node)	{ (node)->parent = __node; __node = parser_add_child(__ctx, __node, (node)); }
#define	_RULE_ADD_CHILD_TO_NODE(node, child)	{ (child)->parent = node; node = parser_add_child(__ctx, (node), (child)); }

#define	_RULE_NOT_MATCHED	{ goto __not_matched; }




_RULE_FUNC_DECL(identifier);
_RULE_FUNC_DECL(literal_uint);
_RULE_FUNC_DECL(literal_real);
_RULE_FUNC_DECL(literal_char);
_RULE_FUNC_DECL(literal_string);
_RULE_FUNC_DECL(type_simple);
_RULE_FUNC_DECL(type_struct);
_RULE_FUNC_DECL(type_array);
_RULE_FUNC_DECL(type_func);
_RULE_FUNC_DECL(type_pointer);
static ParserASTNode * _RULE_NAME(type)(ParserContext *ctx);

_RULE_FUNC_DECL(func_params_item);
_RULE_FUNC_DECL(func_params_ellipsis_item);
_RULE_FUNC_DECL(func_params);
_RULE_FUNC_DECL(func);




_RULE(identifier)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_IDENTIFIER) {
		_RULE_NODE(BE_NODE_IDENTIFIER, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_uint)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case BE_TOKEN_LITERAL_UINT_BIN:
		case BE_TOKEN_LITERAL_UINT_OCT:
		case BE_TOKEN_LITERAL_UINT_DEC:
		case BE_TOKEN_LITERAL_UINT_HEX: {
			_RULE_NODE(BE_NODE_LITERAL_UINT, _RULE_TOKEN)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(literal_real)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_LITERAL_REAL) {
		_RULE_NODE(BE_NODE_LITERAL_REAL, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_char)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_LITERAL_CHAR) {
		_RULE_NODE(BE_NODE_LITERAL_CHAR, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_string)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_LITERAL_STRING) {
		_RULE_NODE(BE_NODE_LITERAL_STRING, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(type_simple)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case BE_TOKEN_KEYWORD_CHAR: {
			_RULE_NODE(BE_NODE_TYPE_CHAR, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_INT8: {
			_RULE_NODE(BE_NODE_TYPE_INT8, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_INT16: {
			_RULE_NODE(BE_NODE_TYPE_INT16, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_INT32: {
			_RULE_NODE(BE_NODE_TYPE_INT32, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_INT64: {
			_RULE_NODE(BE_NODE_TYPE_INT64, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_UINT8: {
			_RULE_NODE(BE_NODE_TYPE_UINT8, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_UINT16: {
			_RULE_NODE(BE_NODE_TYPE_UINT16, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_UINT32: {
			_RULE_NODE(BE_NODE_TYPE_UINT32, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_UINT64: {
			_RULE_NODE(BE_NODE_TYPE_UINT64, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_FLOAT: {
			_RULE_NODE(BE_NODE_TYPE_FLOAT, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_DOUBLE: {
			_RULE_NODE(BE_NODE_TYPE_DOUBLE, NULL)
			break;
		}
		case BE_TOKEN_KEYWORD_VA_LIST: {
			_RULE_NODE(BE_NODE_TYPE_VA_LIST, NULL)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(type_struct)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_KEYWORD_STRUCT) {
		_RULE_NODE(BE_NODE_TYPE_STRUCT, NULL)
		
		ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
		if (node_identifier != NULL) {
			_RULE_ADD_CHILD(node_identifier)
		} else {
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(type_array)
	_RULE_NODE(BE_NODE_TYPE_ARRAY, NULL)

	ParserASTNode *node_array_dims = _RULE_NEW_NODE(BE_NODE_TYPE_ARRAY_DIMS, NULL);

	for (;;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_BRACKETS_LEFT) {
			_RULE_POP_LEXCTX
			break;
		}

		ParserASTNode *node_array_dim = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
		if (node_array_dim != NULL) {
			_RULE_ADD_CHILD_TO_NODE(node_array_dims, node_array_dim)
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_BRACKETS_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
		_RULE_ABANDON_LEXCTX
	}
	if (node_array_dims->nchilds != 0) {
		_RULE_ADD_CHILD(node_array_dims)
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
	if (node_type != NULL) {
		_RULE_ADD_CHILD(node_type)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(type_func)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_KEYWORD_FUNC) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(BE_NODE_TYPE_FUNC, NULL);

	ParserASTNode *node_params = _RULE_NAME(func_params)(_RULE_PARSER_CTX);
	if (node_params != NULL) {
		_RULE_ADD_CHILD(node_params)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == BE_TOKEN_PNCT_COLON) {
		ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
		if (node_type != NULL) {
			_RULE_ADD_CHILD(node_type)
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}
_RULE_END

_RULE(type_pointer)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_MUL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(BE_NODE_TYPE_POINTER, NULL)

	ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
	if (node_type != NULL) {
		_RULE_ADD_CHILD(node_type)
	} else {
		// _RULE_NOT_MATCHED
		ParserASTNode *node_void = _RULE_NEW_NODE(BE_NODE_TYPE_VOID, NULL);
		_RULE_ADD_CHILD(node_void)
	}
_RULE_END

_RULE(type)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(type_simple)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(type_struct)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(type_array)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(type_func)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(type_pointer)(_RULE_PARSER_CTX))
	}
_RULE_END

_RULE(func_params_item)
	_RULE_NODE(BE_NODE_FUNC_PARAMS_ITEM, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_COLON) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
	if (node_type != NULL) {
		_RULE_ADD_CHILD(node_type)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END
_RULE(func_params_ellipsis_item)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_ELLIPSIS) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM, NULL)
_RULE_END
_RULE(func_params)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	int matched_ellipsis = 0;

	_RULE_NODE(BE_NODE_FUNC_PARAMS, NULL)

	ParserASTNode * node_param = _RULE_NAME(func_params_item)(_RULE_PARSER_CTX);
	
	// 函数的第一个参数不允许为“...”。
	//
	// if (node_param == NULL) {
	// 	node_param = _RULE_NAME(func_params_ellipsis_item)(_RULE_PARSER_CTX);
	// 	if (node_param != NULL) {
	// 		matched_ellipsis = 1;
	// 	}
	// }

	if (node_param != NULL) {
		_RULE_ADD_CHILD(node_param)

		for (;;) {
			_RULE_PUSH_LEXCTX

			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_COMMA) {
				_RULE_POP_LEXCTX
				break;
			}

			node_param = _RULE_NAME(func_params_item)(_RULE_PARSER_CTX);
			if (node_param == NULL) {
				node_param = _RULE_NAME(func_params_ellipsis_item)(_RULE_PARSER_CTX);
				if (node_param != NULL && matched_ellipsis) {
					_RULE_POP_LEXCTX
					_RULE_NOT_MATCHED
				}
				if (node_param != NULL) {
					matched_ellipsis = 1;
				}
			}
			if (node_param != NULL) {
				_RULE_ADD_CHILD(node_param)
			} else {
				_RULE_POP_LEXCTX
				_RULE_NOT_MATCHED
			}

			_RULE_ABANDON_LEXCTX
		}
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != BE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}
_RULE_END
