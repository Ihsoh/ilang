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




void _loop_ast_node_releaser(struct _ParserASTNode *node) {
	assert(node);
	assert(node->type == FE_NODE_STAT_WHILE
			|| node->type == FE_NODE_STAT_DO
			|| node->type == FE_NODE_STAT_FOR);

	rstr_free(FE_LOOP_AST_NODE_GET_LABEL_REPEAT(node));
	rstr_free(FE_LOOP_AST_NODE_GET_LABEL_END(node));
}

static ParserASTNode * _new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
) {
	assert(ctx);
	assert(type_name);

	if (FE_IS_EXPR_NODE(type)) {
		FeParserExprASTNodeData data;
		data.type = FE_TYPE_UNKNOWN;
		data.type_node = NULL;

		data.constant = false;
		data.constant_type = FE_TYPE_UNKNOWN;

		data.has_conststr = false;
		data.conststr.str = NULL;
		data.conststr.len = 0;

		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserExprASTNodeData), &data
		);
	} else if (type == FE_NODE_VAR) {
		FeParserVarASTNodeData data;
		data.type = FE_VAR_TYPE_UNKNOWN;
		data.align_node = NULL;
		data.align = 0;
		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserVarASTNodeData), &data
		);
	} else if (type == FE_NODE_VAR_ITEM) {
		FeParserVarItemASTNodeData data;
		data.symbol = NULL;
		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserVarItemASTNodeData), &data
		);
	} else if (type == FE_NODE_FUNC) {
		FeParserFuncASTNodeData data;
		data.symbol = NULL;
		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserFuncASTNodeData), &data
		);
	} else if (type == FE_NODE_FUNC_PARAMS_ITEM) {
		FeParserFuncParamASTNodeData data;
		data.symbol = NULL;
		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserFuncParamASTNodeData), &data
		);
	} else if (type == FE_NODE_STAT_WHILE
			|| type == FE_NODE_STAT_DO
			|| type == FE_NODE_STAT_FOR) {
		FeParserLoopASTNodeData data;
		rstr_init(&(data.label_repeat));
		rstr_init(&(data.label_end));

		ParserASTNode *node = parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserLoopASTNodeData), &data
		);
		node->releaser = _loop_ast_node_releaser;

		return node;
	} else if (type == FE_NODE_STRUCT) {
		FeParserStructASTNodeData data;

		data.packed_node = NULL;
		data.packed = false;

		return parser_new_node(
			ctx, type, type_name, token, sizeof(FeParserStructASTNodeData), &data
		);
	} else {
		return parser_new_node(
			ctx, type, type_name, token, 0, NULL
		);
	}
}

ParserASTNode * fe_parser_new_node(
	ParserContext *ctx,
	int32_t type,
	char *type_name,
	LexerToken *token
) {
	return _new_node(ctx, type, type_name, token);
}

#define	_NEW_NODE(ctx, type, token)	(_new_node((ctx), type, #type, (token)))

static void _check_not_used_attrs(
	ParserContext *ctx
) {
	assert(ctx);

	if (FE_PARSER_CONTEXT_DATA_GET_ALIGN_NODE(ctx) != NULL) {
		ctx->syntax_error(ctx);
	}
}




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
#define	_RULE_TOKEN_TYPE	(__token == NULL ? FE_TOKEN_INVALID : __token->type)

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

_RULE_FUNC_DECL(var_item);
_RULE_FUNC_DECL(var);

_RULE_FUNC_DECL(struct_body);
_RULE_FUNC_DECL(struct);

_RULE_FUNC_DECL(stat_var);

_RULE_FUNC_DECL(stat_elif);
_RULE_FUNC_DECL(stat_else);
_RULE_FUNC_DECL(stat_if);

_RULE_FUNC_DECL(stat_while);

_RULE_FUNC_DECL(stat_do);

_RULE_FUNC_DECL(stat_for_init);
_RULE_FUNC_DECL(stat_for_expr_cond);
_RULE_FUNC_DECL(stat_for_expr);
_RULE_FUNC_DECL(stat_for);

_RULE_FUNC_DECL(stat_break);

_RULE_FUNC_DECL(stat_continue);

_RULE_FUNC_DECL(stat_return);

_RULE_FUNC_DECL(stat_goto);

_RULE_FUNC_DECL(stat_label);

_RULE_FUNC_DECL(stat_label_simple);

_RULE_FUNC_DECL(stat_asm);
_RULE_FUNC_DECL(stat_asm_option);

_RULE_FUNC_DECL(stat_expr);

_RULE_FUNC_DECL(stat_dummy);

_RULE_FUNC_DECL(stat_dummy);

_RULE_FUNC_DECL(stat_va_start);
_RULE_FUNC_DECL(stat_va_end);
_RULE_FUNC_DECL(stat_va_copy);

_RULE_FUNC_DECL(stats_block);





_RULE_FUNC_DECL(expr_wrapper);
_RULE_FUNC_DECL(expr);

_RULE_FUNC_DECL(expr_assign);

_RULE_FUNC_DECL(expr_cond);

_RULE_FUNC_DECL(expr_or);

_RULE_FUNC_DECL(expr_and);

_RULE_FUNC_DECL(expr_bor);

_RULE_FUNC_DECL(expr_bxor);

_RULE_FUNC_DECL(expr_band);

_RULE_FUNC_DECL(expr_eq);

_RULE_FUNC_DECL(expr_lt);

_RULE_FUNC_DECL(expr_shift);

_RULE_FUNC_DECL(expr_add);

_RULE_FUNC_DECL(expr_mul);

_RULE_FUNC_DECL(expr_unary);

_RULE_FUNC_DECL(expr_unary_postfix);

_RULE_FUNC_DECL(expr_func_call_params);
_RULE_FUNC_DECL(expr_member_arridx_fncall);

_RULE_FUNC_DECL(expr_parenthesis);

_RULE_FUNC_DECL(expr_atom);

_RULE_FUNC_DECL(module_item);

_RULE_FUNC_DECL(module);


_RULE(identifier)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_IDENTIFIER) {
		_RULE_NODE(FE_NODE_IDENTIFIER, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_uint)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case FE_TOKEN_LITERAL_UINT_BIN:
		case FE_TOKEN_LITERAL_UINT_OCT:
		case FE_TOKEN_LITERAL_UINT_DEC:
		case FE_TOKEN_LITERAL_UINT_HEX: {
			_RULE_NODE(FE_NODE_LITERAL_UINT, _RULE_TOKEN)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(literal_real)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_LITERAL_REAL) {
		_RULE_NODE(FE_NODE_LITERAL_REAL, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_char)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_LITERAL_CHAR) {
		_RULE_NODE(FE_NODE_LITERAL_CHAR, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_string)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_LITERAL_STRING) {
		_RULE_NODE(FE_NODE_LITERAL_STRING, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(type_simple)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case FE_TOKEN_KEYWORD_CHAR: {
			_RULE_NODE(FE_NODE_TYPE_CHAR, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_INT8: {
			_RULE_NODE(FE_NODE_TYPE_INT8, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_INT16: {
			_RULE_NODE(FE_NODE_TYPE_INT16, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_INT32: {
			_RULE_NODE(FE_NODE_TYPE_INT32, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_INT64: {
			_RULE_NODE(FE_NODE_TYPE_INT64, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_UINT8: {
			_RULE_NODE(FE_NODE_TYPE_UINT8, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_UINT16: {
			_RULE_NODE(FE_NODE_TYPE_UINT16, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_UINT32: {
			_RULE_NODE(FE_NODE_TYPE_UINT32, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_UINT64: {
			_RULE_NODE(FE_NODE_TYPE_UINT64, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_FLOAT: {
			_RULE_NODE(FE_NODE_TYPE_FLOAT, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_DOUBLE: {
			_RULE_NODE(FE_NODE_TYPE_DOUBLE, NULL)
			break;
		}
		case FE_TOKEN_KEYWORD_VA_LIST: {
			_RULE_NODE(FE_NODE_TYPE_VA_LIST, NULL)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(type_struct)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_STRUCT) {
		_RULE_NODE(FE_NODE_TYPE_STRUCT, NULL)
		
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
	_RULE_NODE(FE_NODE_TYPE_ARRAY, NULL)

	ParserASTNode *node_array_dims = _RULE_NEW_NODE(FE_NODE_TYPE_ARRAY_DIMS, NULL);

	for (;;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACKETS_LEFT) {
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
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACKETS_RIGHT) {
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
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_FUNC) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_TYPE_FUNC, NULL);

	ParserASTNode *node_params = _RULE_NAME(func_params)(_RULE_PARSER_CTX);
	if (node_params != NULL) {
		_RULE_ADD_CHILD(node_params)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_COLON) {
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
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_MUL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_TYPE_POINTER, NULL)

	ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
	if (node_type != NULL) {
		_RULE_ADD_CHILD(node_type)
	} else {
		// _RULE_NOT_MATCHED
		ParserASTNode *node_void = _RULE_NEW_NODE(FE_NODE_TYPE_VOID, NULL);
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

_RULE(attr_align)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_ALIGN) {
		_RULE_NOT_MATCHED
	}
	
	LexerToken *tk = _RULE_TOKEN;

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *type_or_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
	if (type_or_expr == NULL) {
		type_or_expr = _RULE_NAME(type)(_RULE_PARSER_CTX);
	}
	if (type_or_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_ATTR_ALIGN, tk)
	_RULE_ADD_CHILD(type_or_expr)
_RULE_END

_RULE(attr_packed)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_PACKED) {
		_RULE_NOT_MATCHED
	}
	
	LexerToken *tk = _RULE_TOKEN;

	_RULE_NODE(FE_NODE_ATTR_PACKED, tk)
_RULE_END

_RULE(attr)
	if (_RULE_CURRENT_NODE == NULL) {
		ParserASTNode *align_node = _RULE_NAME(attr_align)(_RULE_PARSER_CTX);
		if (align_node != NULL) {
			if (FE_PARSER_CONTEXT_DATA_GET_ALIGN_NODE(_RULE_PARSER_CTX) != NULL) {
				_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
			}

			FE_PARSER_CONTEXT_DATA_SET_ALIGN_NODE(_RULE_PARSER_CTX, align_node);

			_RULE_RETURNED_NODE(align_node)
		}
	}

	if (_RULE_CURRENT_NODE == NULL) {
		ParserASTNode *packed_node = _RULE_NAME(attr_packed)(_RULE_PARSER_CTX);
		if (packed_node != NULL) {
			if (FE_PARSER_CONTEXT_DATA_GET_PACKED_NODE(_RULE_PARSER_CTX) != NULL) {
				_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
			}

			FE_PARSER_CONTEXT_DATA_SET_PACKED_NODE(_RULE_PARSER_CTX, packed_node);

			_RULE_RETURNED_NODE(packed_node)
		}
	}
_RULE_END

_RULE(attrs)
	for (;;) {
		ParserASTNode *attr = _RULE_NAME(attr)(_RULE_PARSER_CTX);
		if (attr == NULL) {
			break;
		}
	}
_RULE_END

_RULE(func_params_item)
	_RULE_NODE(FE_NODE_FUNC_PARAMS_ITEM, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COLON) {
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
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_ELLIPSIS) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM, NULL)
_RULE_END
_RULE(func_params)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	int matched_ellipsis = 0;

	_RULE_NODE(FE_NODE_FUNC_PARAMS, NULL)

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
			if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
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
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}
_RULE_END
_RULE(func)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_FUNC) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_FUNC, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}
	
	ParserASTNode *node_func_params = _RULE_NAME(func_params)(_RULE_PARSER_CTX);
	if (node_func_params != NULL) {
		_RULE_ADD_CHILD(node_func_params)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_COLON) {
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

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_DUMMY) {
		ParserASTNode *node_dummy = _RULE_NEW_NODE(FE_NODE_DUMMY, NULL);
		_RULE_ADD_CHILD(node_dummy)
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX

		ParserASTNode *node_body = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
		if (node_body != NULL) {
			_RULE_ADD_CHILD(node_body)
		} else {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(var_item)
	_RULE_NODE(FE_NODE_VAR_ITEM, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COLON) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
	if (node_type != NULL) {
		_RULE_ADD_CHILD(node_type)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_ASSIGN) {
		ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_expr != NULL) {
			_RULE_ADD_CHILD(node_expr)
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}
_RULE_END
_RULE(var)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_VAR) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_VAR, NULL)

	FE_VAR_AST_NODE_SET_ALIGN_NODE(
		_RULE_CURRENT_NODE,
		FE_PARSER_CONTEXT_DATA_GET_ALIGN_NODE(_RULE_PARSER_CTX)
	);
	FE_PARSER_CONTEXT_DATA_SET_ALIGN_NODE(_RULE_PARSER_CTX, NULL);

	ParserASTNode *node_var_item = _RULE_NAME(var_item)(_RULE_PARSER_CTX);
	if (node_var_item != NULL) {
		_RULE_ADD_CHILD(node_var_item)
		for (;;) {
			_RULE_PUSH_LEXCTX

			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
				_RULE_POP_LEXCTX
				break;
			}

			node_var_item = _RULE_NAME(var_item)(_RULE_PARSER_CTX);
			if (node_var_item != NULL) {
				_RULE_ADD_CHILD(node_var_item)
			} else {
				_RULE_POP_LEXCTX
				_RULE_NOT_MATCHED
			}

			_RULE_ABANDON_LEXCTX
		}
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(struct_body)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_DUMMY) {
		_RULE_NODE(FE_NODE_DUMMY, NULL)
	} else {
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACES_LEFT) {
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_STRUCT_BODY, NULL)

		for (;;) {
			ParserASTNode *node_var = _RULE_NAME(stat_var)(_RULE_PARSER_CTX);
			if (node_var != NULL) {
				_RULE_ADD_CHILD(node_var)
			} else {
				break;
			}
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACES_RIGHT) {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END
_RULE(struct)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_STRUCT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STRUCT, NULL)

	ParserASTNode *packed_node = FE_PARSER_CONTEXT_DATA_GET_PACKED_NODE(_RULE_PARSER_CTX);
	FE_STRUCT_AST_NODE_SET_PACKED_NODE(
		_RULE_CURRENT_NODE,
		packed_node
	);
	if (packed_node != NULL) {
		FE_STRUCT_AST_NODE_SET_PACKED(_RULE_CURRENT_NODE, true);
	}
	FE_PARSER_CONTEXT_DATA_SET_PACKED_NODE(_RULE_PARSER_CTX, NULL);

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier == NULL) {
		_RULE_NOT_MATCHED
	}
	_RULE_ADD_CHILD(node_identifier)

	ParserASTNode *node_struct_body = _RULE_NAME(struct_body)(_RULE_PARSER_CTX);
	if (node_struct_body == NULL) {
		_RULE_NOT_MATCHED
	}
	_RULE_ADD_CHILD(node_struct_body)
_RULE_END

_RULE(stat_var)
	_RULE_RETURNED_NODE(_RULE_NAME(var)(_RULE_PARSER_CTX))
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_elif)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_ELIF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_ELIF, NULL)
	_RULE_ADD_CHILD(node_expr)
	_RULE_ADD_CHILD(node_stats_block)
_RULE_END
_RULE(stat_else)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_ELSE) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_stats_block != NULL) {
		_RULE_NODE(FE_NODE_STAT_ELSE, NULL)
		_RULE_ADD_CHILD(node_stats_block)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END
_RULE(stat_if)	
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_IF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_IF, NULL)

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_stats_block != NULL) {
		_RULE_ADD_CHILD(node_stats_block)
	} else {
		_RULE_NOT_MATCHED
	}

	for (;;) {
		ParserASTNode *node_stat_elif = _RULE_NAME(stat_elif)(_RULE_PARSER_CTX);
		if (node_stat_elif != NULL) {
			_RULE_ADD_CHILD(node_stat_elif)
		} else {
			break;
		}
	}

	ParserASTNode *node_stat_else = _RULE_NAME(stat_else)(_RULE_PARSER_CTX);
	if (node_stat_else != NULL) {
		_RULE_ADD_CHILD(node_stat_else)
	}

_RULE_END

_RULE(stat_while)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_WHILE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_WHILE, NULL)

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_stats_block != NULL) {
		_RULE_ADD_CHILD(node_stats_block)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_do)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_DO) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_DO, NULL)

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_stats_block != NULL) {
		_RULE_ADD_CHILD(node_stats_block)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_WHILE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_for_init)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_var = _RULE_NAME(var)(_RULE_PARSER_CTX);

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_FOR_INIT, NULL)
	if (node_var != NULL) {
		_RULE_ADD_CHILD(node_var)
	}
_RULE_END
_RULE(stat_for_expr_cond)
	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_FOR_EXPR_COND, NULL)
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	}
_RULE_END
_RULE(stat_for_expr)
	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_FOR_EXPR, NULL)
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	}
_RULE_END
_RULE(stat_for)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_FOR) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_for_init = _RULE_NAME(stat_for_init)(_RULE_PARSER_CTX);
	if (node_for_init == NULL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_for_expr_cond = _RULE_NAME(stat_for_expr_cond)(_RULE_PARSER_CTX);
	if (node_for_expr_cond == NULL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_for_expr = _RULE_NAME(stat_for_expr)(_RULE_PARSER_CTX);
	if (node_for_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_stats_block = _RULE_NAME(stats_block)(_RULE_PARSER_CTX);
	if (node_stats_block == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_FOR, NULL)
	_RULE_ADD_CHILD(node_for_init)
	_RULE_ADD_CHILD(node_for_expr_cond)
	_RULE_ADD_CHILD(node_for_expr)
	_RULE_ADD_CHILD(node_stats_block)
_RULE_END

_RULE(stat_break)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_BREAK) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_BREAK, NULL)

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_continue)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_CONTINUE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_CONTINUE, NULL)

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_return)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_RETURN) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_RETURN, NULL)

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_goto)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_GOTO) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_GOTO, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_label)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_LABEL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_LABEL, NULL)

	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_label_simple)
	ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	if (node_identifier != NULL) {
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COLON) {
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_STAT_LABEL, NULL)
		_RULE_ADD_CHILD(node_identifier)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_asm_option)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_string = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
	if (node_string == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_ASM_OPTION, NULL)
	_RULE_ADD_CHILD(node_string)
	_RULE_ADD_CHILD(node_expr)
_RULE_END
_RULE(stat_asm)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_ASM) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_ASM, NULL)

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_string = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
	if (node_string != NULL) {
		_RULE_ADD_CHILD(node_string)
	} else {
		_RULE_NOT_MATCHED
	}

	for (;;) {
		ParserASTNode *node_asm_option = _RULE_NAME(stat_asm_option)(_RULE_PARSER_CTX);
		if (node_asm_option != NULL) {
			_RULE_ADD_CHILD(node_asm_option)
		} else {
			break;
		}
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_expr)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX))
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(stat_dummy)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_DUMMY, _RULE_TOKEN)
_RULE_END

_RULE(stat_va_start)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_VA_START) {
		_RULE_NOT_MATCHED
	}
	LexerToken *tk = _RULE_TOKEN;

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_VA_START, tk)
	_RULE_ADD_CHILD(node_expr)
_RULE_END

_RULE(stat_va_end)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_VA_END) {
		_RULE_NOT_MATCHED
	}
	LexerToken *tk = _RULE_TOKEN;

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_VA_END, tk)
	_RULE_ADD_CHILD(node_expr)
_RULE_END

_RULE(stat_va_copy)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_KEYWORD_VA_COPY) {
		_RULE_NOT_MATCHED
	}
	LexerToken *tk = _RULE_TOKEN;

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr_target = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr_target == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_expr_source = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_expr_source == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_SEMICOLON) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STAT_VA_COPY, tk)
	_RULE_ADD_CHILD(node_expr_target)
	_RULE_ADD_CHILD(node_expr_source)
_RULE_END

_RULE(stat)

	_RULE_NAME(attrs)(_RULE_PARSER_CTX);

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_var)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_if)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_while)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_do)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_for)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_break)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_continue)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_return)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_goto)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_label)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_label_simple)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_asm)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_expr)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_dummy)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_va_start)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_va_end)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_va_copy)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stats_block)(_RULE_PARSER_CTX))
	}

	_check_not_used_attrs(_RULE_PARSER_CTX);

_RULE_END

_RULE(stats_block)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACES_LEFT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(FE_NODE_STATS_BLOCK, NULL)

	for (;;) {
		ParserASTNode *node_stat = _RULE_NAME(stat)(_RULE_PARSER_CTX);
		if (node_stat != NULL) {
			_RULE_ADD_CHILD(node_stat)
		} else {
			break;
		}
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_BRACES_RIGHT) {
		_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
	}
_RULE_END




_RULE(expr_wrapper)
	ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_NODE(FE_NODE_EXPR, NULL)
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END
_RULE(expr)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_assign)(_RULE_PARSER_CTX))
_RULE_END

#define	_EXPR_BINARY_ITEM(type, expr_right_name)	\
	{	\
		ParserASTNode *node_left = _RULE_CURRENT_NODE;	\
		ParserASTNode *node_right = _RULE_NAME(expr_right_name)(_RULE_PARSER_CTX);	\
		if (node_right != NULL) {	\
			__node = _new_node(__ctx, type, #type, NULL);	\
			_RULE_ADD_CHILD(node_left)	\
			_RULE_ADD_CHILD(node_right)	\
		} else {	\
			_RULE_POP_LEXCTX	\
			break;	\
		}	\
	}

_RULE(expr_assign)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_cond)(_RULE_PARSER_CTX))

	do {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_ADD_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_ADD_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SUB_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SUB_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_MUL_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_MUL_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_DIV_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_DIV_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_MOD_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_MOD_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BAND_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BAND_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BOR_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BOR_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BXOR_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BXOR_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SHIFT_LEFT_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SHIFT_LEFT_ASSIGN, expr)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SHIFT_RIGHT_ASSIGN) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SHIFT_RIGHT_ASSIGN, expr)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	} while (0);
_RULE_END

_RULE(expr_cond)
	ParserASTNode *expr = _RULE_NAME(expr_or)(_RULE_PARSER_CTX);

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_QUESTION_MARK) {
		ParserASTNode *expr_true = _RULE_NAME(expr)(_RULE_PARSER_CTX);
		
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COLON) {
			_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
		}

		ParserASTNode *expr_false = _RULE_NAME(expr)(_RULE_PARSER_CTX);

		_RULE_NODE(FE_NODE_EXPR_COND, NULL)
		_RULE_ADD_CHILD(expr)
		_RULE_ADD_CHILD(expr_true)
		_RULE_ADD_CHILD(expr_false)
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(expr)
	}
_RULE_END

_RULE(expr_or)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_and)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_OR) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_OR, expr_and)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_and)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_bor)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_AND) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_AND, expr_bor)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_bor)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_bxor)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BOR) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BOR, expr_bxor)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_bxor)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_band)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BXOR) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BXOR, expr_band)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_band)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_eq)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BAND) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_BAND, expr_eq)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_eq)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_lt)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_EQUAL) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_EQ, expr_lt)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_NOT_EQUAL) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_NEQ, expr_lt)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_lt)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_shift)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_LT) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_LT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_LE) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_LE, expr_shift)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_GT) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_GT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_GE) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_GE, expr_shift)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_shift)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_add)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SHIFT_LEFT) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SHIFT_LEFT, expr_add)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SHIFT_RIGHT) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SHIFT_RIGHT, expr_add)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_add)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_mul)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_ADD) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_ADD, expr_mul)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SUB) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_SUB, expr_mul)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_mul)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_unary)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_MUL) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_MUL, expr_unary)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_DIV) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_DIV, expr_unary)
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_MOD) {
			_EXPR_BINARY_ITEM(FE_NODE_EXPR_MOD, expr_unary)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

#undef	_EXPR_BINARY_ITEM

_RULE(expr_unary)

	#define	_EXPR_UNARY_ITEM(type)	\
		{	\
			ParserASTNode *expr_unary = _RULE_NAME(expr_unary)(_RULE_PARSER_CTX);	\
			if (expr_unary != NULL) {	\
				__node = _new_node(__ctx, type, #type, NULL);	\
				_RULE_ADD_CHILD(expr_unary)	\
				_RULE_ABANDON_LEXCTX	\
			} else {	\
				_RULE_POP_LEXCTX	\
				_RULE_NOT_MATCHED	\
			}	\
		}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_NOT) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_NOT)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_ADD) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_POSITIVE)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_SUB) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_NEGATIVE)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BNOT) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_BNOT)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_INC) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_INC_LEFT)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_DEC) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_DEC_LEFT)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_SIZEOF) {
		LexerToken *tk = _RULE_TOKEN;

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *type_or_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
		if (type_or_expr == NULL) {
			type_or_expr = _RULE_NAME(type)(_RULE_PARSER_CTX);
		}
		if (type_or_expr == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_EXPR_SIZEOF, tk)
		_RULE_ADD_CHILD(type_or_expr)
		_RULE_ABANDON_LEXCTX
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_ALIGNOF) {
		LexerToken *tk = _RULE_TOKEN;

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *type_or_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
		if (type_or_expr == NULL) {
			type_or_expr = _RULE_NAME(type)(_RULE_PARSER_CTX);
		}
		if (type_or_expr == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_EXPR_ALIGNOF, tk)
		_RULE_ADD_CHILD(type_or_expr)
		_RULE_ABANDON_LEXCTX
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_CAST) {
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_LT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
		ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
		if (node_type == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_GT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
		ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
		if (node_expr == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_EXPR_CAST, NULL)
		_RULE_ADD_CHILD(node_type)
		_RULE_ADD_CHILD(node_expr)
		_RULE_ABANDON_LEXCTX
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BAND) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_REF)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_MUL) {
		_EXPR_UNARY_ITEM(FE_NODE_EXPR_DEREF)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_KEYWORD_VA_ARG) {
		LexerToken *tk = _RULE_TOKEN;

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
		if (node_expr == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *node_type = _RULE_NAME(type)(_RULE_PARSER_CTX);
		if (node_type == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NODE(FE_NODE_EXPR_VA_ARG, NULL)
		_RULE_ADD_CHILD(node_expr)
		_RULE_ADD_CHILD(node_type)
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(_RULE_NAME(expr_unary_postfix)(_RULE_PARSER_CTX))
	}

	#undef	_EXPR_UNARY_ITEM
_RULE_END

_RULE(expr_unary_postfix)
	ParserASTNode *expr = _RULE_NAME(expr_member_arridx_fncall)(_RULE_PARSER_CTX);

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_INC) {
		_RULE_NODE(FE_NODE_EXPR_INC_RIGHT, NULL)
		_RULE_ADD_CHILD(expr)
	} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_DEC) {
		_RULE_NODE(FE_NODE_EXPR_DEC_RIGHT, NULL)
		_RULE_ADD_CHILD(expr)
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(expr)
	}
_RULE_END

_RULE(expr_func_call_params)
	_RULE_NODE(FE_NODE_EXPR_FUNC_CALL_PARAMS, NULL)

	ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_ADD_CHILD(node_expr)

		for (;;) {
			_RULE_PUSH_LEXCTX

			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE != FE_TOKEN_PNCT_COMMA) {
				_RULE_POP_LEXCTX
				break;
			}

			node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
			if (node_expr == NULL) {
				_RULE_POP_LEXCTX
				_RULE_NOT_MATCHED
			}
			_RULE_ADD_CHILD(node_expr)

			_RULE_ABANDON_LEXCTX
		}
	}
_RULE_END
_RULE(expr_member_arridx_fncall)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_parenthesis)(_RULE_PARSER_CTX));

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_DOT) {
			ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
			if (node_identifier != NULL) {
				ParserASTNode *node_old = _RULE_CURRENT_NODE;
				_RULE_NODE(FE_NODE_EXPR_MEMBER, NULL)
				_RULE_ADD_CHILD(node_old)
				_RULE_ADD_CHILD(node_identifier)
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
				break;
			}
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_RIGHT_ARROW) {
			ParserASTNode *node_identifier = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
			if (node_identifier != NULL) {
				ParserASTNode *node_old = _RULE_CURRENT_NODE;
				_RULE_NODE(FE_NODE_EXPR_PMEMBER, NULL)
				_RULE_ADD_CHILD(node_old)
				_RULE_ADD_CHILD(node_identifier)
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
				break;
			}
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BRACKETS_LEFT) {
			ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
			if (node_expr != NULL) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_BRACKETS_RIGHT) {
					ParserASTNode *node_old = _RULE_CURRENT_NODE;
					_RULE_NODE(FE_NODE_EXPR_ARRAY_INDEX, NULL)
					_RULE_ADD_CHILD(node_old)
					_RULE_ADD_CHILD(node_expr)
				} else {
					_RULE_POP_LEXCTX
					break;
				}
			} else {
				_RULE_POP_LEXCTX
				break;
			}
		} else if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_PARENTHESES_LEFT) {
			ParserASTNode *node_func_call_params = _RULE_NAME(expr_func_call_params)(_RULE_PARSER_CTX);
			if (node_func_call_params != NULL) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
					ParserASTNode *node_old = _RULE_CURRENT_NODE;
					_RULE_NODE(FE_NODE_EXPR_FUNC_CALL, NULL)
					_RULE_ADD_CHILD(node_old)
					_RULE_ADD_CHILD(node_func_call_params)
					_RULE_ABANDON_LEXCTX
				} else {
					_RULE_POP_LEXCTX
					break;
				}
			} else {
				_RULE_POP_LEXCTX
				break;
			}
		} else {
			_RULE_POP_LEXCTX
			break;
		}
	}
_RULE_END

_RULE(expr_parenthesis)
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_RETURNED_NODE(_RULE_NAME(expr)(_RULE_PARSER_CTX))
		if (_RULE_CURRENT_NODE != NULL) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == FE_TOKEN_PNCT_PARENTHESES_RIGHT) {
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
				_RULE_NOT_MATCHED
			}
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(_RULE_NAME(expr_atom)(_RULE_PARSER_CTX));
	}
_RULE_END

_RULE(expr_atom)
	ParserASTNode *node = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);

	if (node == NULL) {
		node = _RULE_NAME(literal_real)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_char)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	}

	if (node != NULL) {
		_RULE_NODE(FE_NODE_EXPR_ATOM, NULL);

		_RULE_ADD_CHILD(node)
	} else {
		_RULE_NOT_MATCHED
	}

_RULE_END

_RULE(module_item)

	_RULE_NAME(attrs)(_RULE_PARSER_CTX);

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(stat_var)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(struct)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(func)(_RULE_PARSER_CTX))
	}

	_check_not_used_attrs(_RULE_PARSER_CTX);

_RULE_END
_RULE(module)
	_RULE_NODE(FE_NODE_MODULE, NULL)

	for (;;) {
		ParserASTNode *node_module_item = _RULE_NAME(module_item)(_RULE_PARSER_CTX);
		if (node_module_item != NULL) {
			_RULE_ADD_CHILD(node_module_item)
		} else {
			if (_RULE_LEXER_CTX->current != NULL) {
				_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
			}
			break;
		}
	}
_RULE_END




ParserContext * fe_parser_new_context(
	const char *file,
	const char *source,
	int len,
	int arch
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	LexerContext *lexctx = fe_lexer_new_context(file, source, len);
	lexer_parse(lexctx);

	FeParserContextData data;
	data.align_node = NULL;
	data.packed_node = NULL;

	ParserContext *ctx = parser_new_context_with_data(
		lexctx, sizeof(data), &data
	);
	ctx->arch = arch;

	return ctx;
}

void fe_parser_free_context(ParserContext * ctx) {
	assert(ctx);

	fe_lexer_free_context(ctx->lexctx);
	parser_free_context(ctx);
}

void fe_parser_parse(ParserContext * ctx) {
	assert(ctx);
	assert(ctx->ast == NULL);

	ctx->ast = _rule_module(ctx);
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

	for (int i = 0; i < node->nchilds; i++) {
		_print_ast(node->childs[i], file, level + 1, padchr);
	}
}

void fe_parser_print_ast(ParserContext * ctx, FILE *file) {
	assert(ctx);
	assert(ctx->ast);
	assert(file);

	_print_ast(ctx->ast, file, 0, '\t');
}

uint32_t fe_parser_get_uint32_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_UINT);

	return fe_lexer_get_uint32_val(ctx->lexctx, node->token);
}

uint64_t fe_parser_get_uint64_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_UINT);

	return fe_lexer_get_uint64_val(ctx->lexctx, node->token);
}

uint64_t fe_parser_get_uint_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_UINT);

	int arch = ctx->arch;
	if (arch == FE_ARCH_32) {
		return fe_parser_get_uint32_val(ctx, node);
	} else if (arch == FE_ARCH_64) {
		return fe_parser_get_uint64_val(ctx, node);
	} else {
		assert(0);
	}
}

float fe_parser_get_float_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_REAL);

	return fe_lexer_get_float_val(ctx->lexctx, node->token);
}

double fe_parser_get_double_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_REAL);

	return fe_lexer_get_double_val(ctx->lexctx, node->token);
}

char fe_parser_get_char_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == FE_NODE_LITERAL_CHAR);

	return fe_lexer_get_char_val(ctx->lexctx, node->token);
}




void _var_symbol_releaser(ParserSymbol *symbol) {
	assert(symbol);

	if (FE_VAR_SYMBOL_GET_HAS_CODE_GEN_NAME(symbol)) {
		rstr_free(FE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol));
	}
}

ParserSymbol * fe_parser_add_var_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node
) {
	assert(ctx);
	assert(node);
	assert(token);
	assert(var_type > 0);
	assert(var_type_node);

	FeParserVarSymbolData data;
	data.type = var_type;
	data.type_node = var_type_node;
	data.func_symbol = NULL;
	data.has_code_gen_name = false;

	ParserSymbol *symbol = parser_add_symbol_to_node(
		ctx,
		node,
		FE_SYM_VAR,
		token,
		sizeof(data),
		&data
	);
	symbol->releaser = _var_symbol_releaser;

	return symbol;
}

ParserSymbol * fe_parser_add_struct_member_var_symbol_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	LexerToken *token,
	uint8_t var_type,
	ParserASTNode *var_type_node
) {
	assert(ctx);
	assert(symbol);
	assert(token);
	assert(var_type > 0);
	assert(var_type_node);

	FeParserStructMemberVarSymbolData data;
	data.type = var_type;
	data.type_node = var_type_node;

	return parser_add_symbol_to_symbol(
		ctx,
		symbol,
		FE_SYM_STRUCT_MEMBER_VAR,
		token,
		sizeof(data),
		&data
	);
}

ParserSymbol * fe_parser_new_struct_symbol(
	ParserContext *ctx,
	LexerToken *token,
	ParserASTNode *struct_node,
	ParserASTNode *body_node,
	bool dummy
) {
	assert(ctx);
	assert(token);
	assert(body_node);

	FeParserStructSymbolData data;
	data.struct_node = struct_node;
	data.body_node = body_node;
	data.dummy = dummy;

	return parser_new_symbol(
		ctx,
		FE_SYM_STRUCT,
		token,
		sizeof(data),
		&data
	);
}

ParserSymbol * fe_parser_add_struct_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	ParserASTNode *body_node,
	bool dummy
) {
	assert(ctx);
	assert(node);
	assert(token);
	assert(body_node);

	FeParserStructSymbolData data;
	data.body_node = body_node;
	data.dummy = dummy;

	return parser_add_symbol_to_node(
		ctx,
		node,
		FE_SYM_STRUCT,
		token,
		sizeof(data),
		&data
	);
}

ParserSymbol * fe_parser_add_func_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	LexerToken *token,
	ParserASTNode *params_node,
	ParserASTNode *return_type_node,
	bool dummy
) {
	assert(ctx);
	assert(node);
	assert(token);
	assert(params_node);

	FeParserFuncSymbolData data;
	data.params_node = params_node;
	data.return_type_node = return_type_node;
	data.dummy = dummy;
	data.counter = 0;

	ParserASTNode *func_type_node = _NEW_NODE(ctx, FE_NODE_TYPE_FUNC, NULL);
	parser_add_child(ctx, func_type_node, params_node);
	if (return_type_node != NULL) {
		parser_add_child(ctx, func_type_node, return_type_node);
	}

	data.func_type_node = func_type_node;

	ParserASTNode *func_pointer_type_node = _NEW_NODE(ctx, FE_NODE_TYPE_POINTER, NULL);
	parser_add_child(ctx, func_pointer_type_node, func_type_node);

	data.func_pointer_type_node = func_pointer_type_node;

	return parser_add_symbol_to_node(
		ctx,
		node,
		FE_SYM_FUNC,
		token,
		sizeof(data),
		&data
	);
}
