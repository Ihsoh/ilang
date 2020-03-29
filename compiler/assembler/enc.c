#include "enc.h"

#include <assert.h>

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"

#include "parser.h"
#include "lexer.h"
#include "ins.h"

#define	_SYNERR_TOKEN(ctx, token, msg)	{ (ctx)->syntax_error_token_msg((ctx), (token), (msg)); }
#define	_SYNERR_NODE(ctx, node, msg, args...)	{ char __buffer__[1024]; sprintf(__buffer__, msg, ##args); (ctx)->syntax_error_node_msg((ctx), (node), __buffer__); }

static void _eval_expr_atom(
	ParserContext *ctx,
	ParserASTNode *node,
	AsmExprEvalResult *result
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_EXPR_ATOM);
	assert(node->nchilds == 1);
	assert(result);

	ParserASTNode *child = node->childs[0];

	LexerToken *token = child->token;

	switch (token->type) {
		case ASM_TOKEN_IDENTIFIER: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = asm_parser_get_symbol_by_token_key(ctx, token);
			break;
		}
		case ASM_TOKEN_LITERAL_UINT_BIN: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = util_get_uint64_bin_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_UINT_OCT: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = util_get_uint64_oct_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_UINT_DEC: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = util_get_uint64_dec_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_UINT_HEX: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = util_get_uint64_hex_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_CHAR: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = util_get_uint64_char_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_REAL: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_DOUBLE;
			result->value.d = util_get_double_val(token->content, token->len);
			break;
		}
		case ASM_TOKEN_LITERAL_STRING: {
			result->type = ASM_EXPR_EVAL_RESULT_TYPE_STRING;
			result->value.str.ptr = token->content + 1;
			result->value.str.len = token->len - 2;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _eval_expr(
	ParserContext *ctx,
	ParserASTNode *node,
	AsmExprEvalResult *result
) {
	assert(ctx);
	assert(node);
	assert(result);

	switch (node->type) {
		case ASM_NODE_EXPR_OR: {
			assert(node->nchilds == 2);
			
			AsmExprEvalResult oprd1;
			_eval_expr(ctx, node->childs[0], &oprd1);
			if (oprd1.type != ASM_EXPR_EVAL_RESULT_TYPE_UINT64) {
				_SYNERR_NODE(ctx, node->childs[0], "expect integer!");
			}

			AsmExprEvalResult oprd2;
			_eval_expr(ctx, node->childs[1], &oprd1);
			if (oprd2.type != ASM_EXPR_EVAL_RESULT_TYPE_UINT64) {
				_SYNERR_NODE(ctx, node->childs[1], "expect integer!");
			}

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 || oprd2.value.u64;

			break;
		}

		case ASM_NODE_EXPR_ATOM: {
			_eval_expr_atom(ctx, node, result);
			break;
		}
		default: {
			assert(0);
			return;
		}
	}
}

static void _eval_expr_wrapper(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == ASM_NODE_EXPR);
	assert(node->nchilds == 1);

	ParserASTNode *child = node->childs[0];

	_eval_expr(ctx, child, &ASM_EXPR_AST_NODE_GET_RESULT(node));
}






static void _encode(
	ParserContext *ctx,
	int step
) {
	assert(ctx);
	assert(ctx->ast);

	ASM_PARSER_CONTEXT_DATA_SET_STEP(ctx, step);
	for (int i = 0; i < ctx->ast->nchilds; i++) {
		ParserASTNode *child = ctx->ast->childs[i];
		if (child->type == ASM_NODE_INSTRUCTION) {
			for (int j = 0; j < child->nchilds; j++) {
				if (child->childs[j]->type == ASM_NODE_EXPR) {
					_eval_expr_wrapper(ctx, child->childs[j]);
				}
			}
		}

		Instruction *ins = ASM_INS_AST_NODE_GET_INS(child);

		InstructionEncoderData data;
		data.ctx = ctx;
		data.ins_node = child;

		ins->encoder(ins, &data);
	}
}

void enc_encode(
	ParserContext *ctx
) {
	assert(ctx);
	assert(ctx->ast);

	_encode(ctx, ASM_STEP_SCAN);
	_encode(ctx, ASM_STEP_ENCODE);
}
