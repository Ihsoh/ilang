#include "evaluator.h"

#include "../../lexer.h"
#include "../../parser.h"
#include "../../util.h"

#include "line.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/*
	a, b => uint_bin, uint_oct, uint_dec, uint_hex, char, identifier

	a || b
	a && b
	a | b
	a ^ b
	a & b
	a == b, a != b
	a < b, a <= b, a > b, a >= b
	a << b, a >> b
	a + b, a - b
	a * b, a / b, a % b
	!a, +a, -a, ~a, definded(a)
	(a)
*/

/*======================================================================
							词法解析
======================================================================*/

#define	_TOKEN_INVALID				0xffff

#define	_TOKEN_SPACE				0x0000

#define	_TOKEN_LITERAL_UINT_BIN		0x1000
#define	_TOKEN_LITERAL_UINT_OCT		0x1001
#define	_TOKEN_LITERAL_UINT_DEC		0x1002
#define	_TOKEN_LITERAL_UINT_HEX		0x1003
#define	_TOKEN_LITERAL_CHAR			0x1004
#define	_TOKEN_LITERAL_STRING		0x1005

#define	_TOKEN_IDENTIFIER			0x2000

#define	_TOKEN_KEYWORD_DEFINED		0x3000
#define	_TOKEN_KEYWORD_STREQ		0x3001

#define	_TOKEN_PNCT_PARENTHESES_LEFT	0x4000
#define	_TOKEN_PNCT_PARENTHESES_RIGHT	0x4001
#define	_TOKEN_PNCT_OR					0x4002
#define	_TOKEN_PNCT_AND					0x4003
#define	_TOKEN_PNCT_BOR					0x4004
#define	_TOKEN_PNCT_BXOR				0x4005
#define	_TOKEN_PNCT_BAND				0x4006
#define	_TOKEN_PNCT_EQUAL				0x4007
#define	_TOKEN_PNCT_NOT_EQUAL			0x4008
#define	_TOKEN_PNCT_LT					0x4009
#define	_TOKEN_PNCT_LE					0x400a
#define	_TOKEN_PNCT_GT					0x400b
#define	_TOKEN_PNCT_GE					0x400c
#define	_TOKEN_PNCT_SHIFT_LEFT			0x400d
#define	_TOKEN_PNCT_SHIFT_RIGHT			0x400e
#define	_TOKEN_PNCT_ADD					0x400f
#define	_TOKEN_PNCT_SUB					0x4010
#define	_TOKEN_PNCT_MUL					0x4011
#define	_TOKEN_PNCT_DIV					0x4012
#define	_TOKEN_PNCT_MOD					0x4013
#define	_TOKEN_PNCT_NOT					0x4014
#define	_TOKEN_PNCT_BNOT				0x4015
#define	_TOKEN_PNCT_COMMA				0x4016




static int _is_delimiter(int chr) {
	return chr == -1 || !isalpha(chr) || chr != '_';
}

/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	*abandon = 1;

	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, _TOKEN_SPACE);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
LEXER_MATCHER(literal_uint_bin)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_UINT_BIN);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'b', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1' || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (_is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获八进制数字字面量。
*/
LEXER_MATCHER(literal_uint_oct)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_UINT_OCT);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'o', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr >= '0' && chr <= '7', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '7') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (_is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_UINT_DEC);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (_is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获十六进制数字字面量。
*/
LEXER_MATCHER(literal_uint_hex)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_UINT_HEX);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'x', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		#define	_IS_HEX_DIGIT_CHAR(chr)	(((chr) >= '0' && (chr) <= '9') || ((chr) >= 'a' && (chr) <= 'f') || ((chr) >= 'A' && (chr) <= 'F'))

		LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (_is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		#undef	_IS_HEX_DIGIT_CHAR

		assert(0);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“'”。
	if (chr == '\'') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_CHAR);

		// 捕获了一个不是“'”的字符。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		// 如果“'”之后的第一个字符是“\”，则继续捕获一个字符。
		if (chr == '\\') {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_char_esc(chr), {
				// MATCHED
			}, {
				// NOT MATCHED
				return NULL;
			})
		} else {
			if (!util_is_literal_char_item(chr)) {
				return NULL;
			}
		}

		// 捕获“'”，字符字面量必须以“'”结束。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“"”。
	if (chr == '"') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_STRING);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_item(chr) && chr != '"' && chr != -1, {
				// MATCHED
				if (chr == '\\') {
					LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_esc(chr), {
						// MATCHED
					}, {
						// NOT MATCHED
						return NULL;
					})
				}
			}, {
				// NOT MATCHED
				if (chr == -1) {
					return NULL;
				}

				lexer_next_char(ctx);
				token.len++;
				return lexer_clone_token(ctx, &token);
			})
		}
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获关键字。
*/
LEXER_MATCHER(keyword)
	*abandon = 0;

	LexerToken token;

	#define	_KEYWORD(text, type)	{(text), sizeof((text)) - 1, (type)}

	static struct _Keyword {
		const char	*text;
		const int	len;
		const int	type;
	} keywords[] = {
		_KEYWORD("defined", _TOKEN_KEYWORD_DEFINED),
		_KEYWORD("streq", _TOKEN_KEYWORD_STREQ),

		{NULL, 0, _TOKEN_INVALID}
	};

	#undef	_KEYWORD

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_') {
		lexer_init_token(ctx, &token, _TOKEN_IDENTIFIER);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				break;
			})
		}

		for (struct _Keyword *keyword = keywords; keyword->text != NULL; keyword++) {
			if (keyword->len == token.len
					&& strncmp(keyword->text, token.content, keyword->len) == 0) {
				token.type = keyword->type;
				break;
			}
		}

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

LEXER_TEXT_MATCHER(pnct_parentheses_left, "(", _TOKEN_PNCT_PARENTHESES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_right, ")", _TOKEN_PNCT_PARENTHESES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_or, "||", _TOKEN_PNCT_OR, 0)
LEXER_TEXT_MATCHER(pnct_and, "&&", _TOKEN_PNCT_AND, 0)
LEXER_TEXT_MATCHER(pnct_bor, "|", _TOKEN_PNCT_BOR, 0)
LEXER_TEXT_MATCHER(pnct_bxor, "^", _TOKEN_PNCT_BXOR, 0)
LEXER_TEXT_MATCHER(pnct_band, "&", _TOKEN_PNCT_BAND, 0)
LEXER_TEXT_MATCHER(pnct_equal, "==", _TOKEN_PNCT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_not_equal, "!=", _TOKEN_PNCT_NOT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_lt, "<", _TOKEN_PNCT_LT, 0)
LEXER_TEXT_MATCHER(pnct_le, "<=", _TOKEN_PNCT_LE, 0)
LEXER_TEXT_MATCHER(pnct_gt, ">", _TOKEN_PNCT_GT, 0)
LEXER_TEXT_MATCHER(pnct_ge, ">=", _TOKEN_PNCT_GE, 0)
LEXER_TEXT_MATCHER(pnct_shift_left, "<<", _TOKEN_PNCT_SHIFT_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_shift_right, ">>", _TOKEN_PNCT_SHIFT_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_add, "+", _TOKEN_PNCT_ADD, 0)
LEXER_TEXT_MATCHER(pnct_sub, "-", _TOKEN_PNCT_SUB, 0)
LEXER_TEXT_MATCHER(pnct_mul, "*", _TOKEN_PNCT_MUL, 0)
LEXER_TEXT_MATCHER(pnct_div, "/", _TOKEN_PNCT_DIV, 0)
LEXER_TEXT_MATCHER(pnct_mod, "%", _TOKEN_PNCT_MOD, 0)
LEXER_TEXT_MATCHER(pnct_not, "!", _TOKEN_PNCT_NOT, 0)
LEXER_TEXT_MATCHER(pnct_bnot, "~", _TOKEN_PNCT_BNOT, 0)
LEXER_TEXT_MATCHER(pnct_comma, ",", _TOKEN_PNCT_COMMA, 0)

#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(space),

	_MATCHER_LIST_ITEM(literal_uint_bin),
	_MATCHER_LIST_ITEM(literal_uint_oct),
	_MATCHER_LIST_ITEM(literal_uint_dec),
	_MATCHER_LIST_ITEM(literal_uint_hex),
	_MATCHER_LIST_ITEM(literal_char),
	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(keyword),

	_MATCHER_LIST_ITEM(pnct_parentheses_left),
	_MATCHER_LIST_ITEM(pnct_parentheses_right),
	_MATCHER_LIST_ITEM(pnct_or),
	_MATCHER_LIST_ITEM(pnct_and),
	_MATCHER_LIST_ITEM(pnct_bor),
	_MATCHER_LIST_ITEM(pnct_bxor),
	_MATCHER_LIST_ITEM(pnct_band),
	_MATCHER_LIST_ITEM(pnct_equal),
	_MATCHER_LIST_ITEM(pnct_not_equal),
	_MATCHER_LIST_ITEM(pnct_lt),
	_MATCHER_LIST_ITEM(pnct_le),
	_MATCHER_LIST_ITEM(pnct_gt),
	_MATCHER_LIST_ITEM(pnct_ge),
	_MATCHER_LIST_ITEM(pnct_shift_left),
	_MATCHER_LIST_ITEM(pnct_shift_right),
	_MATCHER_LIST_ITEM(pnct_add),
	_MATCHER_LIST_ITEM(pnct_sub),
	_MATCHER_LIST_ITEM(pnct_mul),
	_MATCHER_LIST_ITEM(pnct_div),
	_MATCHER_LIST_ITEM(pnct_mod),
	_MATCHER_LIST_ITEM(pnct_not),
	_MATCHER_LIST_ITEM(pnct_bnot),
	_MATCHER_LIST_ITEM(pnct_comma),

	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM




/*======================================================================
							语法解析
======================================================================*/

#define	_NODE_INVALID				0x0000

#define	_NODE_IDENTIFIER			0x0200

#define	_NODE_LITERAL_UINT			0x0300
#define	_NODE_LITERAL_CHAR			0x0301
#define	_NODE_LITERAL_STRING		0x0302

#define	_NODE_EXPR					0x0e00

#define	_NODE_EXPR_OR				0x0e10

#define	_NODE_EXPR_AND				0x0e20

#define	_NODE_EXPR_BOR				0x0e30

#define	_NODE_EXPR_BXOR				0x0e40

#define	_NODE_EXPR_BAND				0x0e50

#define	_NODE_EXPR_EQ				0x0e60
#define	_NODE_EXPR_NEQ				0x0e61

#define	_NODE_EXPR_LT				0x0e70
#define	_NODE_EXPR_LE				0x0e71
#define	_NODE_EXPR_GT				0x0e72
#define	_NODE_EXPR_GE				0x0e73

#define	_NODE_EXPR_SHIFT_LEFT		0x0e80
#define	_NODE_EXPR_SHIFT_RIGHT		0x0e81

#define	_NODE_EXPR_ADD				0x0e90
#define	_NODE_EXPR_SUB				0x0e91

#define	_NODE_EXPR_MUL				0x0ea0
#define	_NODE_EXPR_DIV				0x0ea1
#define	_NODE_EXPR_MOD				0x0ea2

#define	_NODE_EXPR_NOT				0x0eb0
#define	_NODE_EXPR_BNOT				0x0eb1
#define	_NODE_EXPR_POSITIVE			0x0eb2
#define	_NODE_EXPR_NEGATIVE			0x0eb3
#define	_NODE_EXPR_DEFINED			0x0eb4
#define	_NODE_EXPR_STREQ			0x0eb5

#define	_NODE_EXPR_ATOM				0x0ec0




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

#define	_RULE_PARSER_CTX	(__ctx)
#define	_RULE_LEXER_CTX		(__ctx->lexctx)

#define	_RULE_TOKEN			(__token)
#define	_RULE_TOKEN_TYPE	(__token == NULL ? _TOKEN_INVALID : __token->type)

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

#define	_RULE_END	\
		}	\
		lexer_abandon_curstack_top(__ctx->lexctx);	\
		return __node;	\
	__not_matched:	\
		lexer_resume_current(__ctx->lexctx);	\
		return NULL;	\
	}


_RULE_FUNC_DECL(identifier);
_RULE_FUNC_DECL(literal_uint);
_RULE_FUNC_DECL(literal_char);

_RULE_FUNC_DECL(expr_wrapper);
_RULE_FUNC_DECL(expr);






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
_RULE_FUNC_DECL(expr_parenthesis);
_RULE_FUNC_DECL(expr_atom);

_RULE(identifier)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == _TOKEN_IDENTIFIER) {
		_RULE_NODE(_NODE_IDENTIFIER, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_uint)
	_RULE_NEXT_TOKEN
	switch (_RULE_TOKEN_TYPE) {
		case _TOKEN_LITERAL_UINT_BIN:
		case _TOKEN_LITERAL_UINT_OCT:
		case _TOKEN_LITERAL_UINT_DEC:
		case _TOKEN_LITERAL_UINT_HEX: {
			_RULE_NODE(_NODE_LITERAL_UINT, _RULE_TOKEN)
			break;
		}
		default: {
			_RULE_NOT_MATCHED
		}
	}
_RULE_END

_RULE(literal_char)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == _TOKEN_LITERAL_CHAR) {
		_RULE_NODE(_NODE_LITERAL_CHAR, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

_RULE(literal_string)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == _TOKEN_LITERAL_STRING) {
		_RULE_NODE(_NODE_LITERAL_STRING, _RULE_TOKEN)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END




_RULE(expr_wrapper)
	ParserASTNode *node_expr = _RULE_NAME(expr)(_RULE_PARSER_CTX);
	if (node_expr != NULL) {
		_RULE_NODE(_NODE_EXPR, NULL)
		_RULE_ADD_CHILD(node_expr)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END
_RULE(expr)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_or)(_RULE_PARSER_CTX))
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

_RULE(expr_or)
	_RULE_RETURNED_NODE(_RULE_NAME(expr_and)(_RULE_PARSER_CTX))

	for (;_RULE_CURRENT_NODE != NULL;) {
		_RULE_PUSH_LEXCTX
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_OR) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_OR, expr_and)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_AND) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_AND, expr_bor)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_BOR) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_BOR, expr_bxor)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_BXOR) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_BXOR, expr_band)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_BAND) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_BAND, expr_eq)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_EQUAL) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_EQ, expr_lt)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_NOT_EQUAL) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_NEQ, expr_lt)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_LT) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_LT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_LE) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_LE, expr_shift)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_GT) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_GT, expr_shift)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_GE) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_GE, expr_shift)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_SHIFT_LEFT) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_SHIFT_LEFT, expr_add)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_SHIFT_RIGHT) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_SHIFT_RIGHT, expr_add)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_ADD) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_ADD, expr_mul)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_SUB) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_SUB, expr_mul)
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
		if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_MUL) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_MUL, expr_unary)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_DIV) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_DIV, expr_unary)
		} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_MOD) {
			_EXPR_BINARY_ITEM(_NODE_EXPR_MOD, expr_unary)
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
	if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_NOT) {
		_EXPR_UNARY_ITEM(_NODE_EXPR_NOT)
	} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_BNOT) {
		_EXPR_UNARY_ITEM(_NODE_EXPR_BNOT)
	} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_ADD) {
		_EXPR_UNARY_ITEM(_NODE_EXPR_POSITIVE)
	} else if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_SUB) {
		_EXPR_UNARY_ITEM(_NODE_EXPR_NEGATIVE)
	} else if (_RULE_TOKEN_TYPE == _TOKEN_KEYWORD_DEFINED) {
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != _TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *id = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
		if (id == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != _TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		__node = _new_node(__ctx, _NODE_EXPR_DEFINED, "_NODE_EXPR_DEFINED", NULL);
		_RULE_ADD_CHILD(id)
		_RULE_ABANDON_LEXCTX
	} else if (_RULE_TOKEN_TYPE == _TOKEN_KEYWORD_STREQ) {
		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != _TOKEN_PNCT_PARENTHESES_LEFT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *param1 = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
		if (param1 == NULL) {
			param1 = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
		}
		if (param1 == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != _TOKEN_PNCT_COMMA) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		ParserASTNode *param2 = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
		if (param2 == NULL) {
			param2 = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
		}
		if (param2 == NULL) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		_RULE_NEXT_TOKEN
		if (_RULE_TOKEN_TYPE != _TOKEN_PNCT_PARENTHESES_RIGHT) {
			_RULE_POP_LEXCTX
			_RULE_NOT_MATCHED
		}

		__node = _new_node(__ctx, _NODE_EXPR_STREQ, "_NODE_EXPR_STREQ", NULL);
		_RULE_ADD_CHILD(param1)
		_RULE_ADD_CHILD(param2)
		_RULE_ABANDON_LEXCTX
	} else {
		_RULE_POP_LEXCTX
		_RULE_RETURNED_NODE(_RULE_NAME(expr_parenthesis)(_RULE_PARSER_CTX))
	}

	#undef	_EXPR_UNARY_ITEM
_RULE_END

_RULE(expr_parenthesis)
	_RULE_PUSH_LEXCTX
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_PARENTHESES_LEFT) {
		_RULE_RETURNED_NODE(_RULE_NAME(expr)(_RULE_PARSER_CTX))
		if (_RULE_CURRENT_NODE != NULL) {
			_RULE_NEXT_TOKEN
			if (_RULE_TOKEN_TYPE == _TOKEN_PNCT_PARENTHESES_RIGHT) {
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
		node = _RULE_NAME(literal_char)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(literal_string)(_RULE_PARSER_CTX);
	}

	if (node == NULL) {
		node = _RULE_NAME(identifier)(_RULE_PARSER_CTX);
	}

	if (node != NULL) {
		_RULE_NODE(_NODE_EXPR_ATOM, NULL);

		_RULE_ADD_CHILD(node);
	} else {
		_RULE_NOT_MATCHED
	}

_RULE_END




/*======================================================================
							计算表达式的AST
======================================================================*/

static int _eval_identifier(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == _NODE_IDENTIFIER);

	LexerToken *token = node->token;

	if (token->type == _TOKEN_IDENTIFIER) {
		int result;

		ResizableString rstr;
		rstr_init(&rstr);

		expand_macro_rstr(&rstr, genctx, token->content, token->len);

		const char *str = RSTR_CSTR(&rstr);
		int len = RSTR_LEN(&rstr);

		if (util_is_literal_char(str, len)) {
			result = util_get_uint_char_val(str, len);
		} else if (util_is_literal_uint_bin(str, len)) {
			result = util_get_uint_bin_val(str, len);
		} else if (util_is_literal_uint_oct(str, len)) {
			result = util_get_uint_oct_val(str, len);
		} else if (util_is_literal_uint_dec(str, len)) {
			result = util_get_uint_dec_val(str, len);
		} else if (util_is_literal_uint_hex(str, len)) {
			result = util_get_uint_hex_val(str, len);
		} else {
			result = 0;
		}

		rstr_free(&rstr);

		return result;
	} else {
		assert(0);
		return 0;
	}
}

static int _eval_literal_uint(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == _NODE_LITERAL_UINT);

	LexerToken *token = node->token;
	switch (token->type) {
		case _TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint_bin_val(token->content, token->len);
		}
		case _TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint_oct_val(token->content, token->len);
		}
		case _TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint_dec_val(token->content, token->len);
		}
		case _TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
			return 0;
		}
	}
}

static int _eval_literal_char(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == _NODE_LITERAL_CHAR);

	LexerToken *token = node->token;
	if (token->type == _TOKEN_LITERAL_CHAR) {
		return util_get_uint_char_val(token->content, token->len);
	} else {
		assert(0);
		return 0;
	}
}

static int _eval_literal_string(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == _NODE_LITERAL_STRING);

	LexerToken *token = node->token;
	ResizableString rstr;
	rstr_init(&rstr);
	util_get_str_val(&rstr, token->content, token->len);
	const char *cstr = RSTR_CSTR(&rstr);
	int cstr_len = RSTR_LEN(&rstr);
	int result = 0;
	if (util_is_literal_int_bin(cstr, cstr_len)) {
		result = util_get_int_bin_val(cstr, cstr_len);
	} else if (util_is_literal_int_oct(cstr, cstr_len)) {
		result = util_get_int_oct_val(cstr, cstr_len);
	} else if (util_is_literal_int_dec(cstr, cstr_len)) {
		result = util_get_int_dec_val(cstr, cstr_len);
	} else if (util_is_literal_int_hex(cstr, cstr_len)) {
		result = util_get_int_hex_val(cstr, cstr_len);
	} else {
		result = 0;
	}
	rstr_free(&rstr);

	return result;
}

static int _eval_expr_atom(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == _NODE_EXPR_ATOM);
	assert(node->nchilds == 1);

	ParserASTNode *child = node->childs[0];
	switch (child->type) {
		case _NODE_IDENTIFIER: {
			return _eval_identifier(genctx, child);
		}
		case _NODE_LITERAL_UINT: {
			return _eval_literal_uint(genctx, child);
		}
		case _NODE_LITERAL_CHAR: {
			return _eval_literal_char(genctx, child);
		}
		case _NODE_LITERAL_STRING: {
			return _eval_literal_string(genctx, child);
		}
		default: {
			assert(0);
			return 0;
		}
	}
}

static void _get_string_from_node(
	PPGeneratorContext *genctx,
	ResizableString *rstr,
	ParserASTNode *node
) {
	assert(rstr);
	assert(node);

	if (node->type == _NODE_IDENTIFIER) {
		ResizableString rstr_tmp;
		rstr_init(&rstr_tmp);

		ResizableString rstr_tmp_trimed;
		rstr_init(&rstr_tmp_trimed);

		expand_macro_rstr(
			&rstr_tmp,
			genctx,
			node->token->content,
			node->token->len
		);

		rstr_trim_ex(&rstr_tmp_trimed, &rstr_tmp, RSTR_SPACE_CHAR_COMPARER);

		if (util_is_literal_string(RSTR_CSTR(&rstr_tmp_trimed), RSTR_LEN(&rstr_tmp_trimed))) {
			util_get_str_val(
				rstr,
				RSTR_CSTR(&rstr_tmp_trimed),
				RSTR_LEN(&rstr_tmp_trimed)
			);
		} else {
			rstr_append_with_rstr(rstr, &rstr_tmp);
		}

		rstr_free(&rstr_tmp);
		rstr_free(&rstr_tmp_trimed);	
	} else if (node->type == _NODE_LITERAL_STRING) {
		util_get_str_val(
			rstr,
			node->token->content,
			node->token->len
		);
	} else {
		assert(0);
	}
}

static int _eval_expr(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);

	switch (node->type) {
		case _NODE_EXPR: {
			assert(node->nchilds == 1);
			return _eval_expr(genctx, node->childs[0]);
		}

		case _NODE_EXPR_OR: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				|| _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_AND: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				&& _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_BOR: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				| _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_BXOR: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				^ _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_BAND: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				& _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_EQ: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				== _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_NEQ: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				!= _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_LT: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				< _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_LE: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				<= _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_GT: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				> _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_GE: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				>= _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_SHIFT_LEFT: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				<< _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_SHIFT_RIGHT: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				>> _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_ADD: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				+ _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_SUB: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				- _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_MUL: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				* _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_DIV: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				/ _eval_expr(genctx, node->childs[1]);
		}
		case _NODE_EXPR_MOD: {
			assert(node->nchilds == 2);
			return _eval_expr(genctx, node->childs[0])
				% _eval_expr(genctx, node->childs[1]);
		}

		case _NODE_EXPR_NOT: {
			assert(node->nchilds == 1);
			return !_eval_expr(genctx, node->childs[0]);
		}
		case _NODE_EXPR_BNOT: {
			assert(node->nchilds == 1);
			return ~_eval_expr(genctx, node->childs[0]);
		}
		case _NODE_EXPR_POSITIVE: {
			assert(node->nchilds == 1);
			return +_eval_expr(genctx, node->childs[0]);
		}
		case _NODE_EXPR_NEGATIVE: {
			assert(node->nchilds == 1);
			return -_eval_expr(genctx, node->childs[0]);
		}
		case _NODE_EXPR_DEFINED: {
			assert(node->nchilds == 1);
			
			ParserASTNode *child = node->childs[0];
			assert(child->type == _NODE_IDENTIFIER);

			LexerToken *token = child->token;
			assert(token->type == _TOKEN_IDENTIFIER);

			int result;

			ResizableString key;
			rstr_init_with_raw(&key, token->content, token->len);
			if (map_exists_key(genctx->map_kv, &key)) {
				result = 1;
			} else {
				result = 0;
			}
			rstr_free(&key);

			return result;
		}
		case _NODE_EXPR_STREQ: {
			assert(node->nchilds == 2);

			ParserASTNode *node_param1 = node->childs[0];
			ResizableString rstr_param1;
			rstr_init(&rstr_param1);
			_get_string_from_node(genctx, &rstr_param1, node_param1);

			ParserASTNode *node_param2 = node->childs[1];
			ResizableString rstr_param2;
			rstr_init(&rstr_param2);
			_get_string_from_node(genctx, &rstr_param2, node_param2);

			int result = rstr_compare(&rstr_param1, &rstr_param2) ? 1 : 0;

			rstr_free(&rstr_param1);
			rstr_free(&rstr_param2);

			return result;
		}
		case _NODE_EXPR_ATOM: {
			return _eval_expr_atom(genctx, node);
		}

		default: {
			assert(0);
			return 0;
		}
	}
}


int pp_eval(
	PPGeneratorContext *genctx,
	const char *expr,
	int len
) {
	assert(genctx);
	assert(expr);

	int result;

	LexerContext *lexctx = lexer_new_context(
		genctx->psrctx->lexctx->file,
		expr,
		len,
		_matchers
	);
	lexer_parse(lexctx);

	ParserContext *psrctx = parser_new_context(lexctx);
	psrctx->arch = genctx->psrctx->arch;
	psrctx->ast = _rule_expr_wrapper(psrctx);

	result = _eval_expr(genctx, psrctx->ast);

	parser_free_context(psrctx);

	lexer_free_context(lexctx);

	return result;
}
