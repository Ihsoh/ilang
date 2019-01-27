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

	if (type == PP_NODE_DIRECTIVE_IF
			|| type == PP_NODE_DIRECTIVE_IFDEF
			|| type == PP_NODE_DIRECTIVE_IFNDEF) {
		PPParserIfxxxASTNodeData data;
		data.token_endif = NULL;

		return parser_new_node(ctx, type, type_name, token, sizeof(PPParserIfxxxASTNodeData), &data);
	} else {
		return parser_new_node(ctx, type, type_name, token, 0, NULL);
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

#define	_RULE_PARSER_CTX	(__ctx)
#define	_RULE_LEXER_CTX		(__ctx->lexctx)

#define	_RULE_TOKEN			(__token)
#define	_RULE_TOKEN_TYPE	(__token == NULL ? PP_TOKEN_INVALID : __token->type)

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




_RULE_FUNC_DECL(include);
_RULE_FUNC_DECL(define);
_RULE_FUNC_DECL(undef);
_RULE_FUNC_DECL(elif);
_RULE_FUNC_DECL(elifdef);
_RULE_FUNC_DECL(elifndef);
_RULE_FUNC_DECL(elifxxx);
_RULE_FUNC_DECL(else);
_RULE_FUNC_DECL(if);
_RULE_FUNC_DECL(ifdef);
_RULE_FUNC_DECL(ifndef);
_RULE_FUNC_DECL(line);
_RULE_FUNC_DECL(block_item);
_RULE_FUNC_DECL(block);
_RULE_FUNC_DECL(if_block);
_RULE_FUNC_DECL(module);




_RULE(space)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_SPACE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_SPACE, _RULE_TOKEN)
_RULE_END

_RULE(include)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_INCLUDE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_INCLUDE, _RULE_TOKEN)
_RULE_END

_RULE(define)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_DEFINE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_DEFINE, _RULE_TOKEN)
_RULE_END

_RULE(undef)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_UNDEF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_UNDEF, _RULE_TOKEN)
_RULE_END

_RULE(elif)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ELIF) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token_elif = _RULE_TOKEN;

	ParserASTNode *node_block = _RULE_NAME(if_block)(_RULE_PARSER_CTX);
	if (node_block == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_ELIF, token_elif)
	_RULE_ADD_CHILD(node_block)
_RULE_END

_RULE(elifdef)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ELIFDEF) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token_elifdef = _RULE_TOKEN;

	ParserASTNode *node_block = _RULE_NAME(if_block)(_RULE_PARSER_CTX);
	if (node_block == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_ELIFDEF, token_elifdef)
	_RULE_ADD_CHILD(node_block)
_RULE_END

_RULE(elifndef)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ELIFNDEF) {
		_RULE_NOT_MATCHED
	}

	LexerToken *token_elifndef = _RULE_TOKEN;

	ParserASTNode *node_block = _RULE_NAME(if_block)(_RULE_PARSER_CTX);
	if (node_block == NULL) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_ELIFNDEF, token_elifndef)
	_RULE_ADD_CHILD(node_block)
_RULE_END

_RULE(elifxxx)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(elif)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(elifdef)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(elifndef)(_RULE_PARSER_CTX))
	}
_RULE_END

_RULE(else)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ELSE) {
		_RULE_NOT_MATCHED
	}

	ParserASTNode *node_block = _RULE_NAME(if_block)(_RULE_PARSER_CTX);
	if (node_block != NULL) {
		_RULE_NODE(PP_NODE_DIRECTIVE_ELSE, _RULE_TOKEN)
		_RULE_ADD_CHILD(node_block)
	} else {
		_RULE_NOT_MATCHED
	}
_RULE_END

#define	_RULE_IFXXX_PART	\
	ParserASTNode *node_block = _RULE_NAME(if_block)(_RULE_PARSER_CTX);	\
	if (node_block != NULL) {	\
		_RULE_ADD_CHILD(node_block)	\
	} else {	\
		_RULE_NOT_MATCHED	\
	}	\
	for (;;) {	\
		ParserASTNode *node_elifxxx = _RULE_NAME(elifxxx)(_RULE_PARSER_CTX);	\
		if (node_elifxxx != NULL) {	\
			_RULE_ADD_CHILD(node_elifxxx)	\
		} else {	\
			break;	\
		}	\
	}	\
	ParserASTNode *node_else = _RULE_NAME(else)(_RULE_PARSER_CTX);	\
	if (node_else != NULL) {	\
		_RULE_ADD_CHILD(node_else)	\
	}	\
	_RULE_NEXT_TOKEN	\
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ENDIF) {	\
		_RULE_NOT_MATCHED	\
	}	\
	PP_IFXXX_AST_NODE_SET_TOKEN_ENDIF(_RULE_CURRENT_NODE, _RULE_TOKEN);

_RULE(if)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_IF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_IF, _RULE_TOKEN)

	_RULE_IFXXX_PART
_RULE_END

_RULE(ifdef)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_IFDEF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_IFDEF, _RULE_TOKEN)

	_RULE_IFXXX_PART
_RULE_END

_RULE(ifndef)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_IFNDEF) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_IFNDEF, _RULE_TOKEN)

	_RULE_IFXXX_PART
_RULE_END

_RULE(info)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_INFO) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_INFO, _RULE_TOKEN)
_RULE_END

_RULE(warn)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_WARN) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_WARN, _RULE_TOKEN)
_RULE_END

_RULE(error)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_KEYWORD_ERROR) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_DIRECTIVE_ERROR, _RULE_TOKEN)
_RULE_END

_RULE(cmd_line)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_CMD_LINE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_CMD_LINE, _RULE_TOKEN)
_RULE_END

_RULE(line)
	_RULE_NEXT_TOKEN
	if (_RULE_TOKEN_TYPE != PP_TOKEN_LINE) {
		_RULE_NOT_MATCHED
	}

	_RULE_NODE(PP_NODE_LINE, _RULE_TOKEN)
_RULE_END

_RULE(block_item)
	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(space)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(include)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(define)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(undef)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(if)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(ifdef)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(ifndef)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(info)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(warn)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(error)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(cmd_line)(_RULE_PARSER_CTX))
	}

	if (_RULE_CURRENT_NODE == NULL) {
		_RULE_RETURNED_NODE(_RULE_NAME(line)(_RULE_PARSER_CTX))
	}
_RULE_END

_RULE(block)
	_RULE_NODE(PP_NODE_BLOCK, NULL)

	for (;;) {
		ParserASTNode *node_block_item = _RULE_NAME(block_item)(_RULE_PARSER_CTX);
		if (node_block_item != NULL) {
			_RULE_ADD_CHILD(node_block_item)
		} else {
			if (_RULE_LEXER_CTX->current != NULL) {
				_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
			}
			break;
		}
	}
_RULE_END

_RULE(if_block)
	_RULE_NODE(PP_NODE_BLOCK, NULL)

	for (;;) {
		ParserASTNode *node_block_item = _RULE_NAME(block_item)(_RULE_PARSER_CTX);
		if (node_block_item != NULL) {
			_RULE_ADD_CHILD(node_block_item)
		} else {
			if (_RULE_LEXER_CTX->current != NULL) {
				int type = _RULE_LEXER_CTX->current->type;
				if (type == PP_TOKEN_KEYWORD_ELIF
						|| type == PP_TOKEN_KEYWORD_ELIFDEF
						|| type == PP_TOKEN_KEYWORD_ELIFNDEF
						|| type == PP_TOKEN_KEYWORD_ELSE
						|| type == PP_TOKEN_KEYWORD_ENDIF) {
					break;
				}
			}
			_RULE_PARSER_CTX->syntax_error(_RULE_PARSER_CTX);
		}
	}
_RULE_END

_RULE(module)
	_RULE_NODE(PP_NODE_MODULE, NULL)

	ParserASTNode *node_block = _RULE_NAME(block)(_RULE_PARSER_CTX);
	_RULE_ADD_CHILD(node_block);
_RULE_END

ParserContext * pp_parser_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	LexerContext *lexctx = pp_lexer_new_context(file, source, len);
	lexer_parse(lexctx);

	ParserContext *ctx = parser_new_context(lexctx);
	ctx->arch = 0;

	return ctx;
}

void pp_parser_free_context(ParserContext * ctx) {
	assert(ctx);

	pp_lexer_free_context(ctx->lexctx);
	parser_free_context(ctx);
}

void pp_parser_parse(ParserContext * ctx) {
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
		"@%s#%x#%p#%p#%d",
		node->type_name != NULL ? node->type_name : "N/A",
		node->type,
		node->parent,
		node,
		node->nchilds
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

void pp_parser_print_ast(
	ParserContext * ctx,
	FILE *file
) {
	assert(ctx);
	assert(ctx->ast);
	assert(file);

	_print_ast(ctx->ast, file, 0, '\t');
}
