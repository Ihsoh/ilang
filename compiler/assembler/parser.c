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
#include "ins.h"

#include "../../map.h"

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

	if (type == ASM_NODE_INSTRUCTION) {
		AsmParserInsASTNodeData data;
		memset(&data, 0, sizeof(data));

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserInsASTNodeData), &data
		);
	} else if (type == ASM_NODE_EXPR) {
		AsmParserExprASTNodeData data;
		data.result.type = ASM_EXPR_EVAL_RESULT_TYPE_NULL;

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserExprASTNodeData), &data
		);
	} else if (type == ASM_NODE_REG) {
		AsmParserRegASTNodeData data;
		memset(&data, 0, sizeof(data));

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserRegASTNodeData), &data
		);
	} else if (type == ASM_NODE_MEM) {
		AsmParserMemASTNodeData data;
		memset(&data, 0, sizeof(data));

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserMemASTNodeData), &data
		);
	} else if (type == ASM_NODE_DIRECT_ADDRESS) {
		AsmParserDirectAddressASTNodeData data;
		memset(&data, 0, sizeof(data));

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserDirectAddressASTNodeData), &data
		);
	} else {
		return parser_new_node(
			ctx, type, type_name, token, 0, NULL
		);
	}
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

#define	_RULE_FUNC_DECL_1P(name, p1)	\
	static ParserASTNode * _RULE_NAME(name)(ParserContext *__ctx, p1)

#define	_RULE_1P(name, p1)	\
	_RULE_FUNC_DECL_1P(name, p1) {	\
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
#define	_RULE_TOKEN_TYPE	(__token == NULL ? ASM_TOKEN_INVALID : __token->type)

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








_RULE_FUNC_DECL(expr);
_RULE_FUNC_DECL(expr_wrapper);




_RULE(identifier)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_IDENTIFIER) {
		_RULE_NODE(ASM_NODE_IDENTIFIER, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_uint)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case ASM_TOKEN_LITERAL_UINT_BIN:
		case ASM_TOKEN_LITERAL_UINT_OCT:
		case ASM_TOKEN_LITERAL_UINT_DEC:
		case ASM_TOKEN_LITERAL_UINT_HEX: {
			_RULE_NODE(ASM_NODE_LITERAL_UINT, _RULE_TOKEN)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(literal_real)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_LITERAL_REAL) {
		_RULE_NODE(ASM_NODE_LITERAL_REAL, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_char)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_LITERAL_CHAR) {
		_RULE_NODE(ASM_NODE_LITERAL_CHAR, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_string)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_LITERAL_STRING) {
		_RULE_NODE(ASM_NODE_LITERAL_STRING, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(expr_atom)
	ParserASTNode *node = _RULE_NAME(identifier)(_RULE_PARSER_CTX);

	if (node == NULL) {
		node = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_real)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_char)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
	}

	if (node != NULL) {
		_RULE_NODE(ASM_NODE_EXPR_ATOM, NULL);

		_RULE_ADD_CHILD(node)
	} else {
		_RULE_NOT_MATCHED
	}

_RULE_END

_RULE(expr_parenthesis)
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_RETURNED_NODE(_RULE_NAME(expr)(_RULE_PARSER_CTX))
		if (_RULE_CURRENT_NODE != NULL) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_PARENTHESES_RIGHT) {
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
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_NOT) {
		_EXPR_UNARY_ITEM(ASM_NODE_EXPR_NOT)
	} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		_EXPR_UNARY_ITEM(ASM_NODE_EXPR_POSITIVE)
	} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_SUB) {
		_EXPR_UNARY_ITEM(ASM_NODE_EXPR_NEGATIVE)
	} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_BNOT) {
		_EXPR_UNARY_ITEM(ASM_NODE_EXPR_BNOT)
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(_RULE_NAME(expr_parenthesis)(_RULE_PARSER_CTX))
	}

	#undef	_EXPR_UNARY_ITEM
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

_RULE(expr_mul)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_unary)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_MUL) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_MUL, expr_unary)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_DIV) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_DIV, expr_unary)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_MOD) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_MOD, expr_unary)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_ADD, expr_mul)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_SUB) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_SUB, expr_mul)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_SHIFT_LEFT) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_SHIFT_LEFT, expr_add)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_SHIFT_RIGHT) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_SHIFT_RIGHT, expr_add)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_LT) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_LT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_LE) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_LE, expr_shift)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_GT) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_GT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_GE) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_GE, expr_shift)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_EQUAL) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_EQ, expr_lt)
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_NOT_EQUAL) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_NEQ, expr_lt)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_BAND) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_BAND, expr_eq)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_BXOR) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_BXOR, expr_band)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_BOR) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_BOR, expr_bxor)
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
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_AND) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_AND, expr_bor)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

_RULE(expr_or)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_and)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_OR) {
			_EXPR_BINARY_ITEM(ASM_NODE_EXPR_OR, expr_and)
		} else {
			_RULE_POP_LEXCTX
			break;
		}
		_RULE_ABANDON_LEXCTX
	}
_RULE_END

#undef	_EXPR_BINARY_ITEM

_RULE(expr)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_or)(_RULE_PARSER_CTX))
_RULE_END

_RULE(expr_wrapper)
	ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_NODE(ASM_NODE_EXPR, NULL)
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(reg)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_KEYWORD_REGISTER) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token = _RULE_TOKEN;

	_RULE_NODE(ASM_NODE_REG, token)

	InsRegisterIterator iter;
	ins_reg_iter_init(&iter);
	for (InsRegister *reg = ins_reg_iter_next(&iter); reg != NULL; reg = ins_reg_iter_next(&iter)) {
		assert(reg->name);

		if (strlen(reg->name) == token->len
				&& strncmp(reg->name, token->content, token->len) == 0) {
			ASM_REG_AST_NODE_SET_REG(_RULE_CURRENT_NODE, reg);
		}
	}
	ins_reg_iter_free(&iter);

	assert(ASM_REG_AST_NODE_GET_REG(_RULE_CURRENT_NODE));
_RULE_END

static InsRegister * _get_ins_reg_by_token(
	LexerToken *token
) {
	assert(token);

	ResizableString rstr_name;
	rstr_init_with_raw(&rstr_name, token->content, token->len);
	InsRegister *reg = ins_reg_get_by_name(RSTR_CSTR(&rstr_name));
	rstr_free(&rstr_name);

	return reg;
}

static int _get_seg_reg_id(
	LexerToken *token
) {
	assert(token);

	InsRegister *reg = _get_ins_reg_by_token(token);
	
	if (reg != NULL && reg->type == INS_REGISTER_SEG) {
		return reg->id;
	} else {
		return 0;
	}
}

static bool _is_reg(
	LexerToken *token,
	int id
) {
	assert(token);

	InsRegister *reg = _get_ins_reg_by_token(token);

	return reg != NULL && reg->id == id;
}

_RULE(mem16_X_X_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;
	
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ADD) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	LexerToken *token_reg2 = _RULE_TOKEN;

	// [BX+SI]
	// [BX+DI]
	// [BP+SI]
	// [BP+DI]

	int reg1 = 0;
	int reg2 = 0;
	if (_is_reg(token_reg1, INS_AM_BX) && _is_reg(token_reg2, INS_AM_SI)) {
		reg1 = INS_AM_BX;
		reg2 = INS_AM_SI;
	} else if (_is_reg(token_reg1, INS_AM_BX) && _is_reg(token_reg2, INS_AM_DI)) {
		reg1 = INS_AM_BX;
		reg2 = INS_AM_DI;
	} else if (_is_reg(token_reg1, INS_AM_BP) && _is_reg(token_reg2, INS_AM_SI)) {
		reg1 = INS_AM_BP;
		reg2 = INS_AM_SI;
	} else if (_is_reg(token_reg1, INS_AM_BP) && _is_reg(token_reg2, INS_AM_DI)) {
		reg1 = INS_AM_BP;
		reg2 = INS_AM_DI;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem16_X_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;

	// [SI]
	// [DI]
	// [BP]
	// [BX]

	int reg1 = 0;
	int reg2 = INS_OPRD_NONE;
	if (_is_reg(token_reg1, INS_AM_SI)) {
		reg1 = INS_AM_SI;
	} else if (_is_reg(token_reg1, INS_AM_DI)) {
		reg1 = INS_AM_DI;
	} else if (_is_reg(token_reg1, INS_AM_BP)) {
		reg1 = INS_AM_BP;
	} else if (_is_reg(token_reg1, INS_AM_BX)) {
		reg1 = INS_AM_BX;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem16_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	// disp16

	ParserASTNode *node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_disp == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE_1P(_mem16, bool only_disp)
	int type = 0;
	int seg = 0;

	if (ASM_PARSER_CONTEXT_DATA_GET_DEFAULT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		type = ASM_MEM_TYPE_WORD;
	}

	if (!ASM_PARSER_CONTEXT_DATA_GET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_BYTE) {
			type = ASM_MEM_TYPE_BYTE;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_WORD) {
			type = ASM_MEM_TYPE_WORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_DWORD) {
			type = ASM_MEM_TYPE_DWORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_QWORD) {
			type = ASM_MEM_TYPE_QWORD;
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
		}
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_ADDR16) {
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_REGISTER) {
		seg = _get_seg_reg_id(_RULE_TOKEN);
		if (seg != 0) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_COLON) {
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
			}
		} else {
			_RULE_POP_LEXCTX
		}
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node = NULL;

	if (only_disp) {
		node = _RULE_NAME(mem16_disp)(_RULE_PARSER_CTX);
	} else {
		node = _RULE_NAME(mem16_X_X_disp)(_RULE_PARSER_CTX);

		if (node == NULL) {
			node = _RULE_NAME(mem16_X_disp)(_RULE_PARSER_CTX);
		}

		if (node == NULL) {
			node = _RULE_NAME(mem16_disp)(_RULE_PARSER_CTX);
		}
	}

	if (node != NULL) {
		ASM_MEM_AST_NODE_SET_ADDR_SIZE(node, ASM_MEM_ADDR_SIZE_16);
		ASM_MEM_AST_NODE_SET_TYPE(node, type);
		ASM_MEM_AST_NODE_SET_SEG(node, seg);
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_RETURNED_NODE(node)
_RULE_END

_RULE(mem16)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem16)(_RULE_PARSER_CTX, false))
_RULE_END

_RULE(mem16_only_disp)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem16)(_RULE_PARSER_CTX, true))
_RULE_END

_RULE(mem32_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_disp == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, NULL);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem32_base_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = 0;
	int reg2 = INS_OPRD_NONE;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;
	
	if (_is_reg(token_reg1, INS_AM_EAX)) {
		reg1 = INS_AM_EAX;
	} else if (_is_reg(token_reg1, INS_AM_ECX)) {
		reg1 = INS_AM_ECX;
	} else if (_is_reg(token_reg1, INS_AM_EDX)) {
		reg1 = INS_AM_EDX;
	} else if (_is_reg(token_reg1, INS_AM_EBX)) {
		reg1 = INS_AM_EBX;
	} else if (_is_reg(token_reg1, INS_AM_ESP)) {
		reg1 = INS_AM_ESP;
	} else if (_is_reg(token_reg1, INS_AM_EBP)) {
		reg1 = INS_AM_EBP;
	} else if (_is_reg(token_reg1, INS_AM_ESI)) {
		reg1 = INS_AM_ESI;
	} else if (_is_reg(token_reg1, INS_AM_EDI)) {
		reg1 = INS_AM_EDI;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, NULL);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem32_index_scale_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = INS_OPRD_NONE;
	int reg2 = 0;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg2 = _RULE_TOKEN;
	
	if (_is_reg(token_reg2, INS_AM_EAX)) {
		reg2 = INS_AM_EAX;
	} else if (_is_reg(token_reg2, INS_AM_ECX)) {
		reg2 = INS_AM_ECX;
	} else if (_is_reg(token_reg2, INS_AM_EDX)) {
		reg2 = INS_AM_EDX;
	} else if (_is_reg(token_reg2, INS_AM_EBX)) {
		reg2 = INS_AM_EBX;
	} else if (_is_reg(token_reg2, INS_AM_ESP)) {
		reg2 = INS_AM_ESP;
	} else if (_is_reg(token_reg2, INS_AM_EBP)) {
		reg2 = INS_AM_EBP;
	} else if (_is_reg(token_reg2, INS_AM_ESI)) {
		reg2 = INS_AM_ESI;
	} else if (_is_reg(token_reg2, INS_AM_EDI)) {
		reg2 = INS_AM_EDI;
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_MUL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_scale = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_scale == NULL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, node_scale);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem32_base_index_scale_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = 0;
	int reg2 = 0;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;

	if (_is_reg(token_reg1, INS_AM_EAX)) {
		reg1 = INS_AM_EAX;
	} else if (_is_reg(token_reg1, INS_AM_ECX)) {
		reg1 = INS_AM_ECX;
	} else if (_is_reg(token_reg1, INS_AM_EDX)) {
		reg1 = INS_AM_EDX;
	} else if (_is_reg(token_reg1, INS_AM_EBX)) {
		reg1 = INS_AM_EBX;
	} else if (_is_reg(token_reg1, INS_AM_ESP)) {
		reg1 = INS_AM_ESP;
	} else if (_is_reg(token_reg1, INS_AM_EBP)) {
		reg1 = INS_AM_EBP;
	} else if (_is_reg(token_reg1, INS_AM_ESI)) {
		reg1 = INS_AM_ESI;
	} else if (_is_reg(token_reg1, INS_AM_EDI)) {
		reg1 = INS_AM_EDI;
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ADD) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	LexerToken *token_reg2 = _RULE_TOKEN;
	
	if (_is_reg(token_reg2, INS_AM_EAX)) {
		reg2 = INS_AM_EAX;
	} else if (_is_reg(token_reg2, INS_AM_ECX)) {
		reg2 = INS_AM_ECX;
	} else if (_is_reg(token_reg2, INS_AM_EDX)) {
		reg2 = INS_AM_EDX;
	} else if (_is_reg(token_reg2, INS_AM_EBX)) {
		reg2 = INS_AM_EBX;
	} else if (_is_reg(token_reg2, INS_AM_ESP)) {
		reg2 = INS_AM_ESP;
	} else if (_is_reg(token_reg2, INS_AM_EBP)) {
		reg2 = INS_AM_EBP;
	} else if (_is_reg(token_reg2, INS_AM_ESI)) {
		reg2 = INS_AM_ESI;
	} else if (_is_reg(token_reg2, INS_AM_EDI)) {
		reg2 = INS_AM_EDI;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_scale = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_MUL) {
		node_scale = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_scale != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, node_scale);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE_1P(_mem32, bool only_disp)
	int type = 0;
	int seg = 0;

	if (ASM_PARSER_CONTEXT_DATA_GET_DEFAULT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		type = ASM_MEM_TYPE_DWORD;
	}

	if (!ASM_PARSER_CONTEXT_DATA_GET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_BYTE) {
			type = ASM_MEM_TYPE_BYTE;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_WORD) {
			type = ASM_MEM_TYPE_WORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_DWORD) {
			type = ASM_MEM_TYPE_DWORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_QWORD) {
			type = ASM_MEM_TYPE_QWORD;
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
		}
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_ADDR32) {
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_REGISTER) {
		seg = _get_seg_reg_id(_RULE_TOKEN);
		if (seg != 0) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_COLON) {
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
			}
		} else {
			_RULE_POP_LEXCTX
		}
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node = NULL;

	if (only_disp) {
		node = _RULE_NAME(mem32_disp)(_RULE_PARSER_CTX);
	} else {
		node = _RULE_NAME(mem32_disp)(_RULE_PARSER_CTX);

		if (node == NULL) {
			node = _RULE_NAME(mem32_base_disp)(_RULE_PARSER_CTX);
		}

		if (node == NULL) {
			node = _RULE_NAME(mem32_index_scale_disp)(_RULE_PARSER_CTX);
		}

		if (node == NULL) {
			node = _RULE_NAME(mem32_base_index_scale_disp)(_RULE_PARSER_CTX);
		}
	}

	if (node != NULL) {
		ASM_MEM_AST_NODE_SET_ADDR_SIZE(node, ASM_MEM_ADDR_SIZE_32);
		ASM_MEM_AST_NODE_SET_TYPE(node, type);
		ASM_MEM_AST_NODE_SET_SEG(node, seg);
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_RETURNED_NODE(node)
_RULE_END

_RULE(mem32)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem32)(_RULE_PARSER_CTX, false))
_RULE_END

_RULE(mem32_only_disp)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem32)(_RULE_PARSER_CTX, true))
_RULE_END

_RULE(mem64_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_disp == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, INS_OPRD_NONE);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, NULL);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem64_base_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = 0;
	int reg2 = INS_OPRD_NONE;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;
	
	if (_is_reg(token_reg1, INS_AM_RAX)) {
		reg1 = INS_AM_RAX;
	} else if (_is_reg(token_reg1, INS_AM_RCX)) {
		reg1 = INS_AM_RCX;
	} else if (_is_reg(token_reg1, INS_AM_RDX)) {
		reg1 = INS_AM_RDX;
	} else if (_is_reg(token_reg1, INS_AM_RBX)) {
		reg1 = INS_AM_RBX;
	} else if (_is_reg(token_reg1, INS_AM_RSP)) {
		reg1 = INS_AM_RSP;
	} else if (_is_reg(token_reg1, INS_AM_RBP)) {
		reg1 = INS_AM_RBP;
	} else if (_is_reg(token_reg1, INS_AM_RSI)) {
		reg1 = INS_AM_RSI;
	} else if (_is_reg(token_reg1, INS_AM_RDI)) {
		reg1 = INS_AM_RDI;
	} else if (_is_reg(token_reg1, INS_AM_r8)) {
		reg1 = INS_AM_r8;
	} else if (_is_reg(token_reg1, INS_AM_r9)) {
		reg1 = INS_AM_r9;
	} else if (_is_reg(token_reg1, INS_AM_r10)) {
		reg1 = INS_AM_r10;	
	} else if (_is_reg(token_reg1, INS_AM_r11)) {
		reg1 = INS_AM_r11;
	} else if (_is_reg(token_reg1, INS_AM_r12)) {
		reg1 = INS_AM_r12;
	} else if (_is_reg(token_reg1, INS_AM_r13)) {
		reg1 = INS_AM_r13;
	} else if (_is_reg(token_reg1, INS_AM_r14)) {
		reg1 = INS_AM_r14;
	} else if (_is_reg(token_reg1, INS_AM_r15)) {
		reg1 = INS_AM_r15;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, NULL);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem64_index_scale_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = INS_OPRD_NONE;
	int reg2 = 0;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg2 = _RULE_TOKEN;
	
	if (_is_reg(token_reg2, INS_AM_RAX)) {
		reg2 = INS_AM_RAX;
	} else if (_is_reg(token_reg2, INS_AM_RCX)) {
		reg2 = INS_AM_RCX;
	} else if (_is_reg(token_reg2, INS_AM_RDX)) {
		reg2 = INS_AM_RDX;
	} else if (_is_reg(token_reg2, INS_AM_RBX)) {
		reg2 = INS_AM_RBX;
	} else if (_is_reg(token_reg2, INS_AM_RSP)) {
		reg2 = INS_AM_RSP;
	} else if (_is_reg(token_reg2, INS_AM_RBP)) {
		reg2 = INS_AM_RBP;
	} else if (_is_reg(token_reg2, INS_AM_RSI)) {
		reg2 = INS_AM_RSI;
	} else if (_is_reg(token_reg2, INS_AM_RDI)) {
		reg2 = INS_AM_RDI;
	} else if (_is_reg(token_reg2, INS_AM_r8)) {
		reg2 = INS_AM_r8;
	} else if (_is_reg(token_reg2, INS_AM_r9)) {
		reg2 = INS_AM_r9;
	} else if (_is_reg(token_reg2, INS_AM_r10)) {
		reg2 = INS_AM_r10;	
	} else if (_is_reg(token_reg2, INS_AM_r11)) {
		reg2 = INS_AM_r11;
	} else if (_is_reg(token_reg2, INS_AM_r12)) {
		reg2 = INS_AM_r12;
	} else if (_is_reg(token_reg2, INS_AM_r13)) {
		reg2 = INS_AM_r13;
	} else if (_is_reg(token_reg2, INS_AM_r14)) {
		reg2 = INS_AM_r14;
	} else if (_is_reg(token_reg2, INS_AM_r15)) {
		reg2 = INS_AM_r15;
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_MUL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_scale = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_scale == NULL) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, node_scale);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE(mem64_base_index_scale_disp)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_LEFT) {
		_RULE_NOT_MATCHED
	}

	int reg1 = 0;
	int reg2 = 0;

	_RULE_NEXT_TOKEN
	LexerToken *token_reg1 = _RULE_TOKEN;

	if (_is_reg(token_reg1, INS_AM_RAX)) {
		reg1 = INS_AM_RAX;
	} else if (_is_reg(token_reg1, INS_AM_RCX)) {
		reg1 = INS_AM_RCX;
	} else if (_is_reg(token_reg1, INS_AM_RDX)) {
		reg1 = INS_AM_RDX;
	} else if (_is_reg(token_reg1, INS_AM_RBX)) {
		reg1 = INS_AM_RBX;
	} else if (_is_reg(token_reg1, INS_AM_RSP)) {
		reg1 = INS_AM_RSP;
	} else if (_is_reg(token_reg1, INS_AM_RBP)) {
		reg1 = INS_AM_RBP;
	} else if (_is_reg(token_reg1, INS_AM_RSI)) {
		reg1 = INS_AM_RSI;
	} else if (_is_reg(token_reg1, INS_AM_RDI)) {
		reg1 = INS_AM_RDI;
	} else if (_is_reg(token_reg1, INS_AM_r8)) {
		reg1 = INS_AM_r8;
	} else if (_is_reg(token_reg1, INS_AM_r9)) {
		reg1 = INS_AM_r9;
	} else if (_is_reg(token_reg1, INS_AM_r10)) {
		reg1 = INS_AM_r10;	
	} else if (_is_reg(token_reg1, INS_AM_r11)) {
		reg1 = INS_AM_r11;
	} else if (_is_reg(token_reg1, INS_AM_r12)) {
		reg1 = INS_AM_r12;
	} else if (_is_reg(token_reg1, INS_AM_r13)) {
		reg1 = INS_AM_r13;
	} else if (_is_reg(token_reg1, INS_AM_r14)) {
		reg1 = INS_AM_r14;
	} else if (_is_reg(token_reg1, INS_AM_r15)) {
		reg1 = INS_AM_r15;
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ADD) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	LexerToken *token_reg2 = _RULE_TOKEN;
	
	if (_is_reg(token_reg2, INS_AM_RAX)) {
		reg2 = INS_AM_RAX;
	} else if (_is_reg(token_reg2, INS_AM_RCX)) {
		reg2 = INS_AM_RCX;
	} else if (_is_reg(token_reg2, INS_AM_RDX)) {
		reg2 = INS_AM_RDX;
	} else if (_is_reg(token_reg2, INS_AM_RBX)) {
		reg2 = INS_AM_RBX;
	} else if (_is_reg(token_reg2, INS_AM_RSP)) {
		reg2 = INS_AM_RSP;
	} else if (_is_reg(token_reg2, INS_AM_RBP)) {
		reg2 = INS_AM_RBP;
	} else if (_is_reg(token_reg2, INS_AM_RSI)) {
		reg2 = INS_AM_RSI;
	} else if (_is_reg(token_reg2, INS_AM_RDI)) {
		reg2 = INS_AM_RDI;
	} else if (_is_reg(token_reg2, INS_AM_r8)) {
		reg2 = INS_AM_r8;
	} else if (_is_reg(token_reg2, INS_AM_r9)) {
		reg2 = INS_AM_r9;
	} else if (_is_reg(token_reg2, INS_AM_r10)) {
		reg2 = INS_AM_r10;	
	} else if (_is_reg(token_reg2, INS_AM_r11)) {
		reg2 = INS_AM_r11;
	} else if (_is_reg(token_reg2, INS_AM_r12)) {
		reg2 = INS_AM_r12;
	} else if (_is_reg(token_reg2, INS_AM_r13)) {
		reg2 = INS_AM_r13;
	} else if (_is_reg(token_reg2, INS_AM_r14)) {
		reg2 = INS_AM_r14;
	} else if (_is_reg(token_reg2, INS_AM_r15)) {
		reg2 = INS_AM_r15;
	} else {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_scale = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_MUL) {
		node_scale = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_scale != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node_disp = NULL;
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_ADD) {
		node_disp = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
		if (node_disp != NULL) {
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM, NULL)
	ASM_MEM_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM_AST_NODE_SET_NODE_SCALE(_RULE_CURRENT_NODE, node_scale);
	ASM_MEM_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END

_RULE_1P(_mem64, bool only_disp)
	int type = 0;
	int seg = 0;

	if (ASM_PARSER_CONTEXT_DATA_GET_DEFAULT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		type = ASM_MEM_TYPE_QWORD;
	}

	if (!ASM_PARSER_CONTEXT_DATA_GET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX)) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_BYTE) {
			type = ASM_MEM_TYPE_BYTE;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_WORD) {
			type = ASM_MEM_TYPE_WORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_DWORD) {
			type = ASM_MEM_TYPE_DWORD;
			_RULE_ABANDON_LEXCTX
		} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_QWORD) {
			type = ASM_MEM_TYPE_QWORD;
			_RULE_ABANDON_LEXCTX
		} else {
			_RULE_POP_LEXCTX
		}
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_ADDR64) {
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_REGISTER) {
		seg = _get_seg_reg_id(_RULE_TOKEN);
		if (seg != 0) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_COLON) {
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
			}
		} else {
			_RULE_POP_LEXCTX
		}
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node = NULL;

	if (only_disp) {
		node = _RULE_NAME(mem64_disp)(_RULE_PARSER_CTX);
	} else {
		node = _RULE_NAME(mem64_disp)(_RULE_PARSER_CTX);

		if (node == NULL) {
			node = _RULE_NAME(mem64_base_disp)(_RULE_PARSER_CTX);
		}

		if (node == NULL) {
			node = _RULE_NAME(mem64_index_scale_disp)(_RULE_PARSER_CTX);
		}

		if (node == NULL) {
			node = _RULE_NAME(mem64_base_index_scale_disp)(_RULE_PARSER_CTX);
		}
	}

	if (node != NULL) {
		ASM_MEM_AST_NODE_SET_ADDR_SIZE(node, ASM_MEM_ADDR_SIZE_64);
		ASM_MEM_AST_NODE_SET_TYPE(node, type);
		ASM_MEM_AST_NODE_SET_SEG(node, seg);
	} else {
		_RULE_NOT_MATCHED
	}

	_RULE_RETURNED_NODE(node)
_RULE_END

_RULE(mem64)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem64)(_RULE_PARSER_CTX, false))
_RULE_END

_RULE(mem64_only_disp)
	_RULE_RETURNED_NODE(_RULE_NAME(_mem64)(_RULE_PARSER_CTX, true))
_RULE_END

_RULE(mem)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem16)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem32)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem64)(_RULE_PARSER_CTX))
	}
_RULE_END

_RULE(direct_address)
	int addr_size = ASM_MEM_ADDR_SIZE_UNKNOWN;

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_ADDR16) {
		addr_size = ASM_MEM_ADDR_SIZE_16;
		_RULE_ABANDON_LEXCTX
	} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_ADDR32) {
		addr_size = ASM_MEM_ADDR_SIZE_32;
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
	}

	ParserASTNode *node_base = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_base == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_COLON) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_offset = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
	if (node_offset == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_DIRECT_ADDRESS, NULL)
	ASM_DIRECT_ADDRESS_AST_NODE_SET_ADDR_SIZE(_RULE_CURRENT_NODE, addr_size);
	ASM_DIRECT_ADDRESS_AST_NODE_SET_NODE_BASE(_RULE_CURRENT_NODE, node_base);
	ASM_DIRECT_ADDRESS_AST_NODE_SET_NODE_OFFSET(_RULE_CURRENT_NODE, node_offset);
_RULE_END

_RULE(mem_only_disp)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem16_only_disp)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem32_only_disp)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(mem64_only_disp)(_RULE_PARSER_CTX))
	}
_RULE_END

static bool _is_expr_oprd(
	uint16_t oprd_type
) {
	return oprd_type == INS_DIRECTIVE_OPRD_INT
			|| oprd_type == INS_DIRECTIVE_OPRD_REAL
			|| oprd_type == INS_DIRECTIVE_OPRD_STRING
			|| oprd_type == INS_DIRECTIVE_OPRD_EXPR;
}

static bool _is_id_oprd(
	uint16_t oprd_type
) {
	return oprd_type == INS_DIRECTIVE_OPRD_ID;
}

static bool _is_Eb_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_E | INS_OT_b);
}

static bool _is_Gb_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_G | INS_OT_b);
}

static bool _is_Ew_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_E | INS_OT_w);
}

static bool _is_Gw_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_G | INS_OT_w);
}

static bool _is_Ev_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_E | INS_OT_v);
}

static bool _is_M_oprd(
	uint16_t oprd_type
) {
	return (oprd_type & 0xff00) == INS_AM_M;
}

static bool _is_Gv_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_G | INS_OT_v);
}

static bool _is_Rv_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_R | INS_OT_v);
}

static bool _is_Gz_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_G | INS_OT_z);
}

static bool _is_AL_oprd(
	uint16_t oprd_type
) {
	return oprd_type == INS_AM_AL;
}

static bool _is_Ib_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_I | INS_OT_b);
}

static bool _is_Iv_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_I | INS_OT_v);
}

static bool _is_Iz_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_I | INS_OT_z);
}

static bool _is_Iw_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_I | INS_OT_w);
}

static bool _is_seg_oprd(
	uint16_t oprd_type
) {
	return oprd_type == INS_AM_CS
			|| oprd_type == INS_AM_SS
			|| oprd_type == INS_AM_DS
			|| oprd_type == INS_AM_ES
			|| oprd_type == INS_AM_FS
			|| oprd_type == INS_AM_GS;
}

static bool _is_Ob_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_O | INS_OT_b);
}

static bool _is_Ov_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_O | INS_OT_v);
}

static bool _is_Jb_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_J | INS_OT_b);
}

static bool _is_Jz_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_J | INS_OT_z);
}

static bool _is_Sw_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_S | INS_OT_w);
}

static bool _is_Ap_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_A | INS_OT_p);
}

static bool _is_Ep_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_E | INS_OT_p);
}

static bool _is_Mp_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_M | INS_OT_p);
}

static bool _is_Mw_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_M | INS_OT_w);
}

static bool _is_Mb_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_M | INS_OT_b);
}

static bool _is_Ms_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_M | INS_OT_s);
}

_RULE(ins)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_KEYWORD_INSTRUCTION) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token = _RULE_TOKEN;

	LexerToken * last_pos = NULL;

	bool new_node = false;
	bool add_child = true;

	#define	_INS_RULE_ADD_CHILD(v)	\
		{ if (add_child) { _RULE_ADD_CHILD((v)); } }

	InstructionIterator iter;
	ins_iter_init(&iter);
	for (Instruction *ins = ins_iter_next(&iter); ins != NULL; ins = ins_iter_next(&iter)) {
		assert(ins->mnemonic);

		if (strlen(ins->mnemonic) == token->len
				&& strncmp(ins->mnemonic, token->content, token->len) == 0) {
			_RULE_PUSH_LEXCTX
			
			if (!new_node) {
				_RULE_NODE(ASM_NODE_INSTRUCTION, token)
				new_node = true;
			}

			uint16_t oprd_type[3] = {ins->oprd.o1, ins->oprd.o2, ins->oprd.o3};
			
			for (int i = 0; i < 3; i++) {
				uint16_t ot = oprd_type[i];

				if (ot != 0) {
					// 检测逗号。
					if (i > 0) {
						_RULE_NEXT_TOKEN
						if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_COMMA) {
							goto not_matched;
						}
					}

					#define	_A(_a, _b) {	\
						ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);	\
						if (node_reg == NULL) {	\
							goto not_matched;	\
						}	\
						InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);	\
						assert(reg);	\
						if (reg->id == (_a)	\
								|| reg->id == (_b)) {	\
							_INS_RULE_ADD_CHILD(node_reg)	\
						} else {	\
							goto not_matched;	\
						}	\
					}

					#define	_B(_a, _b, _c) {	\
						ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);	\
						if (node_reg == NULL) {	\
							goto not_matched;	\
						}	\
						InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);	\
						assert(reg);	\
						if (reg->id == (_a)	\
								|| reg->id == (_b)	\
								|| reg->id == (_c)) {	\
							_INS_RULE_ADD_CHILD(node_reg)	\
						} else {	\
							goto not_matched;	\
						}	\
					}

					#define	_C(_a) {	\
						ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);	\
						if (node_reg == NULL) {	\
							goto not_matched;	\
						}	\
						InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);	\
						assert(reg);	\
						if (reg->id == (_a)) {	\
							_INS_RULE_ADD_CHILD(node_reg)	\
						} else {	\
							goto not_matched;	\
						}	\
					}

					if (ins->superscript & INS_SS_DIRECTIVE) {
						if (_is_expr_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_oprd)
						} else if (_is_id_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_oprd)
						} else {
							goto not_matched;
						}
					} else {
						if (_is_Eb_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg != NULL) {
								InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
								assert(reg);
								if (reg->type != INS_REGISTER_GENERAL_1BYTE) {
									goto not_matched;
								}
								_INS_RULE_ADD_CHILD(node_reg)
							} else {
								ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);

								if (node_mem == NULL) {
									goto not_matched;
								}
								
								if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_BYTE) {
									goto not_matched;
								}

								_INS_RULE_ADD_CHILD(node_mem)
							}
						} else if (_is_Gb_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->type != INS_REGISTER_GENERAL_1BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_reg)
						} else if (_is_Ew_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg != NULL) {
								InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
								assert(reg);
								if (reg->type != INS_REGISTER_GENERAL_2BYTE) {
									goto not_matched;
								}
								_INS_RULE_ADD_CHILD(node_reg)
							} else {
								ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);

								if (node_mem == NULL) {
									goto not_matched;
								}
								
								if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_WORD) {
									goto not_matched;
								}

								_INS_RULE_ADD_CHILD(node_mem)
							}
						} else if (_is_Gw_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->type != INS_REGISTER_GENERAL_2BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_reg)
						} else if (_is_Ev_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg != NULL) {
								InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
								assert(reg);
								if (reg->type != INS_REGISTER_GENERAL_2BYTE
										&& reg->type != INS_REGISTER_GENERAL_4BYTE
										&& reg->type != INS_REGISTER_GENERAL_8BYTE) {
									goto not_matched;
								}
								_INS_RULE_ADD_CHILD(node_reg)
							} else {
								ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);

								if (node_mem == NULL) {
									goto not_matched;
								}
								
								if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_WORD
										&& ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_DWORD
										&& ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_QWORD) {
									goto not_matched;
								}

								_INS_RULE_ADD_CHILD(node_mem)
							}
						} else if (_is_Ep_oprd(ot) || _is_Mp_oprd(ot)) {
							ASM_PARSER_CONTEXT_DATA_SET_DEFAULT_OPRD_SIZE(_RULE_PARSER_CTX, true);
							ASM_PARSER_CONTEXT_DATA_SET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX, true);
							
							ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);


							ASM_PARSER_CONTEXT_DATA_SET_DEFAULT_OPRD_SIZE(_RULE_PARSER_CTX, false);
							ASM_PARSER_CONTEXT_DATA_SET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX, false);

							if (node_mem == NULL) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						} else if (_is_Mw_oprd(ot)) {
							ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);

							if (node_mem == NULL) {
								goto not_matched;
							}

							if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_WORD) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						} else if (_is_Mb_oprd(ot)) {
							ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);

							if (node_mem == NULL) {
								goto not_matched;
							}

							if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						} else if (_is_Ms_oprd(ot) || _is_M_oprd(ot)) {
							ASM_PARSER_CONTEXT_DATA_SET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX, true);
							ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);
							ASM_PARSER_CONTEXT_DATA_SET_MEM_WITHOUT_OPRD_SIZE(_RULE_PARSER_CTX, false);

							if (node_mem == NULL) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						} else if (_is_Gv_oprd(ot) || _is_Rv_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->type != INS_REGISTER_GENERAL_2BYTE
									&& reg->type != INS_REGISTER_GENERAL_4BYTE
									&& reg->type != INS_REGISTER_GENERAL_8BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_reg)
						} else if (_is_Gz_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->type != INS_REGISTER_GENERAL_2BYTE
									&& reg->type != INS_REGISTER_GENERAL_4BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_reg)
						} else if (_is_Ob_oprd(ot)) {
							ParserASTNode *node_mem = _RULE_NAME(mem_only_disp)(_RULE_PARSER_CTX);

							if (node_mem == NULL) {
								goto not_matched;
							}
							
							if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_BYTE) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						} else if (_is_Ov_oprd(ot)) {
							ParserASTNode *node_mem = _RULE_NAME(mem_only_disp)(_RULE_PARSER_CTX);

							if (node_mem == NULL) {
								goto not_matched;
							}
							
							if (ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_WORD
									&& ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_DWORD
									&& ASM_MEM_AST_NODE_GET_TYPE(node_mem) != ASM_MEM_TYPE_QWORD) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_mem)
						/* AL ~ r8l */	
						} else if (ot == INS_AM_AL) {
							_C(INS_AM_AL)
						} else if (ot == INS_AM_CL) {
							_C(INS_AM_CL)
						} else if (ot == INS_AM_DL) {
							_C(INS_AM_DL)
						} else if (ot == INS_AM_BL) {
							_C(INS_AM_BL)
						} else if (ot == INS_AM_AH) {
							_C(INS_AM_AH)
						} else if (ot == INS_AM_CH) {
							_C(INS_AM_CH)
						} else if (ot == INS_AM_DH) {
							_C(INS_AM_DH)
						} else if (ot == INS_AM_BH) {
							_C(INS_AM_BH)
						} else if (ot == INS_AM_r8l) {
							_C(INS_AM_r8l)
						} else if (ot == INS_AM_r9l) {
							_C(INS_AM_r9l)
						} else if (ot == INS_AM_r10l) {
							_C(INS_AM_r10l)
						} else if (ot == INS_AM_r11l) {
							_C(INS_AM_r11l)
						} else if (ot == INS_AM_r12l) {
							_C(INS_AM_r12l)
						} else if (ot == INS_AM_r13l) {
							_C(INS_AM_r13l)
						} else if (ot == INS_AM_r14l) {
							_C(INS_AM_r14l)
						} else if (ot == INS_AM_r15l) {
							_C(INS_AM_r15l)
						/* AX ~ R15W */
						} else if (ot == INS_AM_AX) {
							_C(INS_AM_AX)
						} else if (ot == INS_AM_CX) {
							_C(INS_AM_CX)
						} else if (ot == INS_AM_DX) {
							_C(INS_AM_DX)
						} else if (ot == INS_AM_BX) {
							_C(INS_AM_BX)
						} else if (ot == INS_AM_SP) {
							_C(INS_AM_SP)
						} else if (ot == INS_AM_BP) {
							_C(INS_AM_BP)
						} else if (ot == INS_AM_SI) {
							_C(INS_AM_SI)
						} else if (ot == INS_AM_DI) {
							_C(INS_AM_DI)
						} else if (ot == INS_AM_r8w) {
							_C(INS_AM_r8w)
						} else if (ot == INS_AM_r9w) {
							_C(INS_AM_r9w)
						} else if (ot == INS_AM_r10w) {
							_C(INS_AM_r10w)
						} else if (ot == INS_AM_r11w) {
							_C(INS_AM_r11w)
						} else if (ot == INS_AM_r12w) {
							_C(INS_AM_r12w)
						} else if (ot == INS_AM_r13w) {
							_C(INS_AM_r13w)
						} else if (ot == INS_AM_r14w) {
							_C(INS_AM_r14w)
						} else if (ot == INS_AM_r15w) {
							_C(INS_AM_r15w)
						/* eAX ~ eDI */	
						} else if (ot == INS_AM_eAX) {
							_A(INS_AM_AX, INS_AM_EAX)
						} else if (ot == INS_AM_eCX) {
							_A(INS_AM_CX, INS_AM_ECX)
						} else if (ot == INS_AM_eDX) {
							_A(INS_AM_DX, INS_AM_EDX)
						} else if (ot == INS_AM_eBX) {
							_A(INS_AM_BX, INS_AM_EBX)
						} else if (ot == INS_AM_eSP) {
							_A(INS_AM_SP, INS_AM_ESP)
						} else if (ot == INS_AM_eBP) {
							_A(INS_AM_BP, INS_AM_EBP)
						} else if (ot == INS_AM_eSI) {
							_A(INS_AM_SI, INS_AM_ESI)
						} else if (ot == INS_AM_eDI) {
							_A(INS_AM_DI, INS_AM_EDI)
						/* rAX ~ r15 */
						} else if (ot == INS_AM_rAX) {
							_B(INS_AM_AX, INS_AM_EAX, INS_AM_RAX)
						} else if (ot == INS_AM_rCX) {
							_B(INS_AM_CX, INS_AM_ECX, INS_AM_RCX)
						} else if (ot == INS_AM_rDX) {
							_B(INS_AM_DX, INS_AM_EDX, INS_AM_RDX)
						} else if (ot == INS_AM_rBX) {
							_B(INS_AM_BX, INS_AM_EBX, INS_AM_RBX)
						} else if (ot == INS_AM_rSP) {
							_B(INS_AM_SP, INS_AM_ESP, INS_AM_RSP)
						} else if (ot == INS_AM_rBP) {
							_B(INS_AM_BP, INS_AM_EBP, INS_AM_RBP)
						} else if (ot == INS_AM_rSI) {
							_B(INS_AM_SI, INS_AM_ESI, INS_AM_RSI)
						} else if (ot == INS_AM_rDI) {
							_B(INS_AM_DI, INS_AM_EDI, INS_AM_RDI)
						} else if (ot == INS_AM_r8) {
							_B(INS_AM_r8w, INS_AM_r8d, INS_AM_r8)
						} else if (ot == INS_AM_r9) {
							_B(INS_AM_r9w, INS_AM_r9d, INS_AM_r9)
						} else if (ot == INS_AM_r10) {
							_B(INS_AM_r10w, INS_AM_r10d, INS_AM_r10)
						} else if (ot == INS_AM_r11) {
							_B(INS_AM_r11w, INS_AM_r11d, INS_AM_r11)
						} else if (ot == INS_AM_r12) {
							_B(INS_AM_r12w, INS_AM_r12d, INS_AM_r12)
						} else if (ot == INS_AM_r13) {
							_B(INS_AM_r13w, INS_AM_r13d, INS_AM_r13)
						} else if (ot == INS_AM_r14) {
							_B(INS_AM_r14w, INS_AM_r14d, INS_AM_r14)
						} else if (ot == INS_AM_r15) {
							_B(INS_AM_r15w, INS_AM_r15d, INS_AM_r15)
						} else if (_is_Ib_oprd(ot) || _is_Iz_oprd(ot) || _is_Iw_oprd(ot) || _is_Iv_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_oprd)
						} else if (_is_Ap_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(direct_address)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_oprd)
						} else if (ot == INS_AM_CONST_1) {
							ParserASTNode *node_oprd = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_oprd);
							if (result->type != ASM_EXPR_EVAL_RESULT_TYPE_UINT64
									|| result->value.u64 != 1) {
								goto not_matched;
							}

							_INS_RULE_ADD_CHILD(node_oprd)
						} else if (_is_seg_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->id == INS_AM_CS
									|| reg->id == INS_AM_SS
									|| reg->id == INS_AM_DS
									|| reg->id == INS_AM_ES
									|| reg->id == INS_AM_FS
									|| reg->id == INS_AM_GS) {
								_INS_RULE_ADD_CHILD(node_reg)
							} else {
								goto not_matched;
							}
						} else if (ot == INS_AM_LABEL) {
							ParserASTNode *node_label = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
							if (node_label == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_label)
						} else if (_is_Jb_oprd(ot) || _is_Jz_oprd(ot)) {
							ParserASTNode *node_label = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
							if (node_label == NULL) {
								goto not_matched;
							}
							_INS_RULE_ADD_CHILD(node_label)
						} else if (_is_Sw_oprd(ot)) {
							ParserASTNode *node_reg = _RULE_NAME(reg)(_RULE_PARSER_CTX);
							if (node_reg == NULL) {
								goto not_matched;
							}

							InsRegister *reg = ASM_REG_AST_NODE_GET_REG(node_reg);
							assert(reg);
							if (reg->id == INS_AM_CS
									|| reg->id == INS_AM_SS
									|| reg->id == INS_AM_DS
									|| reg->id == INS_AM_ES
									|| reg->id == INS_AM_FS
									|| reg->id == INS_AM_GS) {
								_INS_RULE_ADD_CHILD(node_reg)
							} else {
								goto not_matched;
							}
						} else {
							goto not_matched;
						}
					}

					#undef	_A
					#undef	_B
				}
			}

			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_SEMICOLON) {
				goto not_matched;
			}

			last_pos = _RULE_TOKEN->next;

			ASM_INS_AST_NODE_ADD_INS(_RULE_CURRENT_NODE, ins);

			add_child = false;

			_RULE_POP_LEXCTX
			continue;
not_matched:
			_RULE_POP_LEXCTX
			if (add_child) {
				_RULE_CURRENT_NODE->nchilds = 0;
			}
		}
	}
	ins_iter_free(&iter);

	lexer_set_current_pos(_RULE_LEXER_CTX, last_pos);
_RULE_END

_RULE(module_item)
	uint8_t opcode_o1 = 0;
	uint8_t opcode_o2 = 0;
	uint8_t opcode_o3 = 0;
	uint8_t opcode_len = 0;

	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_HINT) {
		_RULE_ABANDON_LEXCTX

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_NOT_MATCHED
		}

		for (;;) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_OPCODE_O1) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ASSIGN) {
					_RULE_NOT_MATCHED
				}

				ParserASTNode *node_value = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
				if (node_value == NULL) {
					_RULE_NOT_MATCHED
				}

				opcode_o1 = (uint8_t) asm_parser_get_uint32_val(_RULE_PARSER_CTX, node_value);
			} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_OPCODE_O2) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ASSIGN) {
					_RULE_NOT_MATCHED
				}

				ParserASTNode *node_value = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
				if (node_value == NULL) {
					_RULE_NOT_MATCHED
				}

				opcode_o2 = (uint8_t) asm_parser_get_uint32_val(_RULE_PARSER_CTX, node_value);
			} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_OPCODE_O3) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ASSIGN) {
					_RULE_NOT_MATCHED
				}

				ParserASTNode *node_value = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
				if (node_value == NULL) {
					_RULE_NOT_MATCHED
				}

				opcode_o3 = (uint8_t) asm_parser_get_uint32_val(_RULE_PARSER_CTX, node_value);
			} else if (_RULE_TOKEN_TYPE == ASM_TOKEN_KEYWORD_OPCODE_LEN) {
				_RULE_NEXT_TOKEN
				if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_ASSIGN) {
					_RULE_NOT_MATCHED
				}

				ParserASTNode *node_value = _RULE_NAME(literal_uint)(_RULE_PARSER_CTX);
				if (node_value == NULL) {
					_RULE_NOT_MATCHED
				}

				opcode_len = (uint8_t) asm_parser_get_uint32_val(_RULE_PARSER_CTX, node_value);
			} else {
				_RULE_NOT_MATCHED
			}

			_RULE_PUSH_LEXCTX
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == ASM_TOKEN_PNCT_COMMA) {
				_RULE_ABANDON_LEXCTX
			} else {
				_RULE_POP_LEXCTX
				break;
			}
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_NOT_MATCHED
		}

	} else {
		_RULE_POP_LEXCTX
	}

	if (_RULE_CURRENT_NODE == NULL) {
		ParserASTNode *node = _RULE_NAME(ins)(_RULE_PARSER_CTX);

		if (node != NULL) {
			ASM_INS_AST_NODE_SET_OPCODE_LEN(node, opcode_len);
			ASM_INS_AST_NODE_SET_OPCODE_O1(node, opcode_o1);
			ASM_INS_AST_NODE_SET_OPCODE_O2(node, opcode_o2);
			ASM_INS_AST_NODE_SET_OPCODE_O3(node, opcode_o3);
		}

		_RULE_RETURNED_NODE(node)
	}
_RULE_END
_RULE(module)
	_RULE_NODE(ASM_NODE_MODULE, NULL)

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

ParserContext * asm_parser_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	LexerContext *lexctx = asm_lexer_new_context(file, source, len);
	lexer_parse(lexctx);

	AsmParserContextData data;
	map_init(
		&data.symtable,
		map_rstr_comparer, NULL,
		(MapReleaser) rstr_delete, (MapReleaser) map_primitive_box_free
	);
	data.arch = ASM_ARCH_BIT16;
	data.out = NULL;
	data.address_counter = 0;
	data.step = ASM_STEP_SCAN;

	data.mem_without_oprd_size = false;

	data.default_oprd_size = false;

	ParserContext *ctx = parser_new_context_with_data(
		lexctx, sizeof(data), &data
	);

	return ctx;
}

void asm_parser_free_context(
	ParserContext * ctx
) {
	assert(ctx);

	//
	// TODO: BUG!!!!!!
	//

	// map_free(
	// 	&ASM_PARSER_CONTEXT_DATA_GET_SYMTABLE(ctx), true, false
	// );

	// asm_lexer_free_context(ctx->lexctx);
	// parser_free_context(ctx);
}

void asm_parser_parse(
	ParserContext * ctx
) {
	assert(ctx);
	assert(ctx->ast == NULL);

	ctx->ast = _rule_module(ctx);
}

static void _print_ast(
	ParserASTNode *node,
	FILE *file,
	int level,
	char padchr
) {
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

	if (node->type == ASM_NODE_MEM) {
		for (int i = 0; i < level; i++) {
			fputc(padchr, file);
		}
		fputs("|MemAddrSize: ", file);
		switch (ASM_MEM_AST_NODE_GET_ADDR_SIZE(node)) {
			case ASM_MEM_ADDR_SIZE_16: {
				fputs("ADDR16", file);
				break;
			}
			case ASM_MEM_ADDR_SIZE_32: {
				fputs("ADDR32", file);
				break;
			}
			case ASM_MEM_ADDR_SIZE_64: {
				fputs("ADDR64", file);
				break;
			}
			default: {
				assert(0);
				break;
			}
		}
		fputc('\n', file);

		for (int i = 0; i < level; i++) {
			fputc(padchr, file);
		}
		fputs("|MemSize: ", file);
		switch (ASM_MEM_AST_NODE_GET_TYPE(node)) {
			case ASM_MEM_TYPE_BYTE: {
				fputs("BYTE", file);
				break;
			}
			case ASM_MEM_TYPE_WORD: {
				fputs("WORD", file);
				break;
			}
			case ASM_MEM_TYPE_DWORD: {
				fputs("DWORD", file);
				break;
			}
			case ASM_MEM_TYPE_QWORD: {
				fputs("QWORD", file);
				break;
			}
			case ASM_MEM_TYPE_UNKNOWN: {
				fputs("UNKNOWN", file);
				break;
			}
			default: {
				assert(0);
				break;
			}
		}
		fputc('\n', file);
	}

	for (int i = 0; i < node->nchilds; i++) {
		_print_ast(node->childs[i], file, level + 1, padchr);
	}
}

void asm_parser_print_ast(
	ParserContext * ctx,
	FILE *file
) {
	assert(ctx);
	assert(ctx->ast);
	assert(file);

	_print_ast(ctx->ast, file, 0, '\t');
}

void asm_parser_set_symbol_by_rstr_key(
	ParserContext *ctx,
	ResizableString *key,
	uint64_t value
) {
	assert(ctx);
	assert(key);

	Map *symtable = &ASM_PARSER_CONTEXT_DATA_GET_SYMTABLE(ctx);

	MapPrimitiveBox *box = map_primitive_box_uint64(value);

	map_set(symtable, key, box, true);
}

void asm_parser_set_symbol_by_token_key(
	ParserContext *ctx,
	LexerToken *key,
	uint64_t value
) {
	assert(ctx);
	assert(key);

	ResizableString *rstr_key = rstr_new_with_raw(key->content, key->len);
	asm_parser_set_symbol_by_rstr_key(ctx, rstr_key, value);
}

uint64_t asm_parser_get_symbol_by_rstr_key(
	ParserContext *ctx,
	ResizableString *key
) {
	assert(ctx);
	assert(key);

	Map *symtable = &ASM_PARSER_CONTEXT_DATA_GET_SYMTABLE(ctx);

	MapPrimitiveBox *box = (MapPrimitiveBox *) map_get(symtable, key);
	if (box != NULL) {
		assert(box->type == MAP_PRIMITIVE_BOX_TYPE_UINT64);
		return box->value.u64;
	} else {
		return 0;
	}
}

uint64_t asm_parser_get_symbol_by_token_key(
	ParserContext *ctx,
	LexerToken *key
) {
	assert(ctx);
	assert(key);

	ResizableString rstr_key;
	rstr_init_with_raw(&rstr_key, key->content, key->len);
	uint64_t value = asm_parser_get_symbol_by_rstr_key(ctx, &rstr_key);
	rstr_free(&rstr_key);

	return value;
}

uint32_t asm_parser_get_uint32_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_UINT);

	return asm_lexer_get_uint32_val(ctx->lexctx, node->token);
}

uint64_t asm_parser_get_uint64_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_UINT);

	return asm_lexer_get_uint64_val(ctx->lexctx, node->token);
}

float asm_parser_get_float_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_REAL);

	return asm_lexer_get_float_val(ctx->lexctx, node->token);
}

double asm_parser_get_double_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_REAL);

	return asm_lexer_get_double_val(ctx->lexctx, node->token);
}

char asm_parser_get_char_val(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_CHAR);

	return asm_lexer_get_char_val(ctx->lexctx, node->token);
}

bool asm_parser_has_unsigned_mark(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_UINT);

	return asm_lexer_has_unsigned_mark(ctx->lexctx, node->token);
}

bool asm_parser_has_float_mark(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_LITERAL_REAL);

	return asm_lexer_has_float_mark(ctx->lexctx, node->token);
}
