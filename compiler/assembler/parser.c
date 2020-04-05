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
		data.ins = NULL;

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
		data.reg = NULL;

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserRegASTNodeData), &data
		);
	} else if (type == ASM_NODE_MEM16) {
		AsmParserMem16ASTNodeData data;
		memset(&data, 0, sizeof(data));

		return parser_new_node(
			ctx, type, type_name, token, sizeof(AsmParserMem16ASTNodeData), &data
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
			_RULE_NOT_MATCHED
		}
	} else {
		_RULE_POP_LEXCTX
	}

	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_BRACKETS_RIGHT) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(ASM_NODE_MEM16, NULL)
	ASM_MEM16_AST_NODE_SET_REG1(_RULE_CURRENT_NODE, reg1);
	ASM_MEM16_AST_NODE_SET_REG2(_RULE_CURRENT_NODE, reg2);
	ASM_MEM16_AST_NODE_SET_NODE_DISP(_RULE_CURRENT_NODE, node_disp);
_RULE_END




_RULE(mem16)
	int type = 0;
	int seg = 0;

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

	if (node == NULL) {
		node = _RULE_NAME(mem16_X_X_disp)(_RULE_PARSER_CTX);
	}




	if (node != NULL) {
		ASM_MEM16_AST_NODE_SET_TYPE(node, type);
		ASM_MEM16_AST_NODE_SET_SEG(node, seg);
	} else {
		assert(0);
	}

	_RULE_RETURNED_NODE(node)
_RULE_END

_RULE(mem32)

_RULE_END

_RULE(mem64)

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

static bool _is_Ev_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_E | INS_OT_v);
}

static bool _is_Gv_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_G | INS_OT_v);
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

static bool _is_rAX_oprd(
	uint16_t oprd_type
) {
	return oprd_type == INS_AM_rAX;
}

static bool _is_Iz_oprd(
	uint16_t oprd_type
) {
	return oprd_type == (INS_AM_I | INS_OT_z);
}

_RULE(ins)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != ASM_TOKEN_KEYWORD_INSTRUCTION) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token = _RULE_TOKEN;

	InstructionIterator iter;
	ins_iter_init(&iter);
	for (Instruction *ins = ins_iter_next(&iter); ins != NULL; ins = ins_iter_next(&iter)) {
		assert(ins->mnemonic);

		if (strlen(ins->mnemonic) == token->len
				&& strncmp(ins->mnemonic, token->content, token->len) == 0) {
			_RULE_PUSH_LEXCTX
			
			_RULE_NODE(ASM_NODE_INSTRUCTION, token)

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

					if (ins->superscript & INS_SS_DIRECTIVE) {
						if (_is_expr_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(expr_wrapper)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_RULE_ADD_CHILD(node_oprd)
						} else if (_is_id_oprd(ot)) {
							ParserASTNode *node_oprd = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
							if (node_oprd == NULL) {
								goto not_matched;
							}
							_RULE_ADD_CHILD(node_oprd)
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
								_RULE_ADD_CHILD(node_reg)
							} else {
								ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);
								if (node_mem != NULL) {
									_RULE_ADD_CHILD(node_mem)
								} else {
									goto not_matched;
								}
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

							_RULE_ADD_CHILD(node_reg)
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
								_RULE_ADD_CHILD(node_reg)
							} else {
								ParserASTNode *node_mem = _RULE_NAME(mem)(_RULE_PARSER_CTX);
								if (node_mem != NULL) {
									_RULE_ADD_CHILD(node_mem)
								} else {
									goto not_matched;
								}
							}
						} else if (_is_Gv_oprd(ot)) {
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

							_RULE_ADD_CHILD(node_reg)
						} else if (_is_AL_oprd(ot)) {
							goto not_matched;
						} else if (_is_Ib_oprd(ot)) {
							goto not_matched;
						} else if (_is_rAX_oprd(ot)) {
							goto not_matched;
						} else if (_is_Iz_oprd(ot)) {
							goto not_matched;
						} else {
							goto not_matched;
						}
					}
				}
			}

			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE != ASM_TOKEN_PNCT_SEMICOLON) {
				goto not_matched;
			}

			ASM_INS_AST_NODE_SET_INS(_RULE_CURRENT_NODE, ins);

			_RULE_ABANDON_LEXCTX
			break;
not_matched:
			_RULE_POP_LEXCTX
		}
	}
	ins_iter_free(&iter);
_RULE_END

_RULE(module_item)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(ins)(_RULE_PARSER_CTX))
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
