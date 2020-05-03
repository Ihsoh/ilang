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
);

static void _expect_integer(
	ParserContext *ctx,
	ParserASTNode *node,
	AsmExprEvalResult *result
) {
	assert(ctx);
	assert(node);
	assert(result);

	_eval_expr(ctx, node, result);
	if (result->type != ASM_EXPR_EVAL_RESULT_TYPE_UINT64) {
		_SYNERR_NODE(ctx, node, "expect integer!");
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
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 || oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_AND: {
			assert(node->nchilds == 2);
			
			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 && oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_BOR: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 | oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_BXOR: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 ^ oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_BAND: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 & oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_EQ: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 == oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_NEQ: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 != oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_LT: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 < oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_LE: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 <= oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_GT: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 > oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_GE: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 >= oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_SHIFT_LEFT: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 << oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_SHIFT_RIGHT: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 >> oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_ADD: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 + oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_SUB: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 - oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_MUL: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 * oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_DIV: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 / oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_MOD: {
			assert(node->nchilds == 2);

			AsmExprEvalResult oprd1;
			_expect_integer(ctx, node->childs[0], &oprd1);

			AsmExprEvalResult oprd2;
			_expect_integer(ctx, node->childs[1], &oprd2);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd1.value.u64 % oprd2.value.u64;
			break;
		}

		case ASM_NODE_EXPR_NOT: {
			assert(node->nchilds == 1);

			AsmExprEvalResult oprd;
			_expect_integer(ctx, node->childs[0], &oprd);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = !oprd.value.u64;
			break;
		}

		case ASM_NODE_EXPR_BNOT: {
			assert(node->nchilds == 1);

			AsmExprEvalResult oprd;
			_expect_integer(ctx, node->childs[0], &oprd);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = ~oprd.value.u64;
			break;
		}

		case ASM_NODE_EXPR_POSITIVE: {
			assert(node->nchilds == 1);

			AsmExprEvalResult oprd;
			_expect_integer(ctx, node->childs[0], &oprd);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = oprd.value.u64;
			break;
		}

		case ASM_NODE_EXPR_NEGATIVE: {
			assert(node->nchilds == 1);

			AsmExprEvalResult oprd;
			_expect_integer(ctx, node->childs[0], &oprd);

			result->type = ASM_EXPR_EVAL_RESULT_TYPE_UINT64;
			result->value.u64 = -oprd.value.u64;
			break;
		}

		case ASM_NODE_EXPR_ATOM: {
			_eval_expr_atom(ctx, node, result);
			break;
		}
		default: {
			printf("ERROR: NODE-TYPE: %x, %s\n", node->type, node->type_name);
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

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(ctx);

	ASM_PARSER_CONTEXT_DATA_SET_STEP(ctx, step);
	for (int i = 0; i < ctx->ast->nchilds; i++) {
		ParserASTNode *ins_node = ctx->ast->childs[i];

		if (ins_node->type == ASM_NODE_INSTRUCTION) {
			bool processed = false;

			assert(ASM_INS_AST_NODE_GET_COUNT(ins_node) > 0);

			for (int j = 0; j < ASM_INS_AST_NODE_GET_COUNT(ins_node); j++) {
				Instruction *ins = ASM_INS_AST_NODE_GET_INS(ins_node, j);
				assert(ins);
				
				if (arch == ASM_ARCH_BIT64) {
					if (ins->superscript & INS_SS_i64) {
						continue;
					}
				}

				for (int j = 0; j < ins_node->nchilds; j++) {
					ParserASTNode *node_child = ins_node->childs[j];

					if (node_child->type == ASM_NODE_EXPR) {
						_eval_expr_wrapper(ctx, node_child);
					} else if (node_child->type == ASM_NODE_REG) {

					} else if (node_child->type == ASM_NODE_MEM
									&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node_child) == ASM_MEM_ADDR_SIZE_16) {
						uint32_t disp = 0;
						ParserASTNode *node_disp = ASM_MEM_AST_NODE_GET_NODE_DISP(node_child);
						if (node_disp != NULL) {
							_eval_expr_wrapper(ctx, node_disp);

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_disp);
							assert(result);
							assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

							disp = (uint32_t) (result->value.u64 & 0xffff);
						}

						int reg1 = ASM_MEM_AST_NODE_GET_REG1(node_child);
						int reg2 = ASM_MEM_AST_NODE_GET_REG2(node_child);

						int mod = 0;
						int rm = 0;

						if (disp == 0) {
							mod = 0;
						} else if (disp <= 0xff) {
							mod = 1;
						} else if (disp <= 0xffff) {
							mod = 2;
						}

						if (reg1 == INS_AM_BX && reg2 == INS_AM_SI) {
							rm = 0;
						} else if (reg1 == INS_AM_BX && reg2 == INS_AM_DI) {
							rm = 1;
						} else if (reg1 == INS_AM_BP && reg2 == INS_AM_SI) {
							rm = 2;
						} else if (reg1 == INS_AM_BP && reg2 == INS_AM_DI) {
							rm = 3;
						} else if (reg1 == INS_AM_SI && reg2 == INS_OPRD_NONE) {
							rm = 4;
						} else if (reg1 == INS_AM_DI && reg2 == INS_OPRD_NONE) {
							rm = 5;
						} else if (reg1 == INS_AM_BP && reg2 == INS_OPRD_NONE) {
							rm = 6;
						} else if (reg1 == INS_AM_BX && reg2 == INS_OPRD_NONE) {
							rm = 7;
						}

						// disp16
						if (reg1 == INS_OPRD_NONE
								&& reg2 == INS_OPRD_NONE
								&& node_disp != NULL) {
							mod = 0;
							rm = 6;
						}

						// [BP] -> [BP + 0]
						if (reg1 == INS_AM_BP
								&& reg2 == INS_OPRD_NONE
								&& node_disp == NULL) {
							mod = -1;
							rm = -1;
						}

						ASM_MEM_AST_NODE_SET_MOD(node_child, mod);
						ASM_MEM_AST_NODE_SET_RM(node_child, rm);
						ASM_MEM_AST_NODE_SET_DISP(node_child, disp);
					} else if (node_child->type == ASM_NODE_MEM
									&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node_child) == ASM_MEM_ADDR_SIZE_32) {
						uint32_t scale = 0;
						ParserASTNode *node_scale = ASM_MEM_AST_NODE_GET_NODE_SCALE(node_child);
						if (node_scale != NULL) {
							_eval_expr_wrapper(ctx, node_scale);

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_scale);
							assert(result);
							assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

							scale = result->value.u64;
							if (scale != 2 && scale != 4 && scale != 8) {
								assert(0);
							}
						}
						
						uint32_t disp = 0;
						ParserASTNode *node_disp = ASM_MEM_AST_NODE_GET_NODE_DISP(node_child);
						if (node_disp != NULL) {
							_eval_expr_wrapper(ctx, node_disp);

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_disp);
							assert(result);
							assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

							disp = (uint32_t) (result->value.u64 & 0xffffffff);
						}

						int reg1 = ASM_MEM_AST_NODE_GET_REG1(node_child);
						int reg2 = ASM_MEM_AST_NODE_GET_REG2(node_child);

						int mod = 0;
						int rm = 0;
						int sib_base = -1;
						int sib_index = -1;
						int sib_ss = -1;

						if (reg1 == INS_OPRD_NONE
								&& reg2 == INS_OPRD_NONE
								&& node_scale == NULL
								&& node_disp != NULL) {
							// [disp32]
							mod = 0;
							rm = 5;
						} else if (reg1 != INS_OPRD_NONE
										&& reg2 == INS_OPRD_NONE
										&& node_scale == NULL) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							switch (reg1) {
								case INS_AM_EAX: {
									rm = 0;
									break;
								}
								case INS_AM_ECX: {
									rm = 1;
									break;
								}
								case INS_AM_EDX: {
									rm = 2;
									break;
								}
								case INS_AM_EBX: {
									rm = 3;
									break;
								}
								case INS_AM_ESP: {
									rm = 4;
									break;
								}
								case INS_AM_EBP: {
									rm = 5;
									break;
								}
								case INS_AM_ESI: {
									rm = 6;
									break;
								}
								case INS_AM_EDI: {
									rm = 7;
									break;
								}
							}
						} else if (reg1 == INS_OPRD_NONE
										&& reg2 != INS_OPRD_NONE
										&& node_scale != NULL) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							rm = 4;
							sib_base = 5;

							switch (scale)
							{
								case 2: {
									sib_ss = 1;
									break;
								}
								case 4: {
									sib_ss = 2;
									break;
								}
								case 8: {
									sib_ss = 3;
									break;
								}
								default: {
									assert(0);
									break;
								}
							}

							switch (reg2) {
								case INS_AM_EAX: {
									sib_index = 0;
									break;
								}
								case INS_AM_ECX: {
									sib_index = 1;
									break;
								}
								case INS_AM_EDX: {
									sib_index = 2;
									break;
								}
								case INS_AM_EBX: {
									sib_index = 3;
									break;
								}
								case INS_AM_ESP: {
									sib_index = 4;
									break;
								}
								case INS_AM_EBP: {
									sib_index = 5;
									break;
								}
								case INS_AM_ESI: {
									sib_index = 6;
									break;
								}
								case INS_AM_EDI: {
									sib_index = 7;
									break;
								}
							}
						} else if (reg1 != INS_OPRD_NONE
										&& reg2 != INS_OPRD_NONE) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							rm = 4;

							if (node_scale != NULL) {
								switch (scale)
								{
									case 2: {
										sib_ss = 1;
										break;
									}
									case 4: {
										sib_ss = 2;
										break;
									}
									case 8: {
										sib_ss = 3;
										break;
									}
									default: {
										assert(0);
										break;
									}
								}
							}

							switch (reg1) {
								case INS_AM_EAX: {
									sib_base = 0;
									break;
								}
								case INS_AM_ECX: {
									sib_base = 1;
									break;
								}
								case INS_AM_EDX: {
									sib_base = 2;
									break;
								}
								case INS_AM_EBX: {
									sib_base = 3;
									break;
								}
								case INS_AM_ESP: {
									sib_base = 4;
									break;
								}
								case INS_AM_EBP: {
									sib_base = 5;
									break;
								}
								case INS_AM_ESI: {
									sib_base = 6;
									break;
								}
								case INS_AM_EDI: {
									sib_base = 7;
									break;
								}
							}

							switch (reg2) {
								case INS_AM_EAX: {
									sib_index = 0;
									break;
								}
								case INS_AM_ECX: {
									sib_index = 1;
									break;
								}
								case INS_AM_EDX: {
									sib_index = 2;
									break;
								}
								case INS_AM_EBX: {
									sib_index = 3;
									break;
								}
								case INS_AM_ESP: {
									sib_index = 4;
									break;
								}
								case INS_AM_EBP: {
									sib_index = 5;
									break;
								}
								case INS_AM_ESI: {
									sib_index = 6;
									break;
								}
								case INS_AM_EDI: {
									sib_index = 7;
									break;
								}
							}
						}

						ASM_MEM_AST_NODE_SET_MOD(node_child, mod);
						ASM_MEM_AST_NODE_SET_RM(node_child, rm);
						ASM_MEM_AST_NODE_SET_DISP(node_child, disp);
						ASM_MEM_AST_NODE_SET_SCALE(node_child, scale);
						ASM_MEM_AST_NODE_SET_SIB_BASE(node_child, sib_base);
						ASM_MEM_AST_NODE_SET_SIB_INDEX(node_child, sib_index);
						ASM_MEM_AST_NODE_SET_SIB_SS(node_child, sib_ss);
					} else if (node_child->type == ASM_NODE_MEM
									&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node_child) == ASM_MEM_ADDR_SIZE_64) {
						uint32_t scale = 0;
						ParserASTNode *node_scale = ASM_MEM_AST_NODE_GET_NODE_SCALE(node_child);
						if (node_scale != NULL) {
							_eval_expr_wrapper(ctx, node_scale);

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_scale);
							assert(result);
							assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

							scale = result->value.u64;
							if (scale != 2 && scale != 4 && scale != 8) {
								assert(0);
							}
						}
						
						uint32_t disp = 0;
						ParserASTNode *node_disp = ASM_MEM_AST_NODE_GET_NODE_DISP(node_child);
						if (node_disp != NULL) {
							_eval_expr_wrapper(ctx, node_disp);

							AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node_disp);
							assert(result);
							assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

							disp = (uint32_t) (result->value.u64 & 0xffffffff);
						}

						int reg1 = ASM_MEM_AST_NODE_GET_REG1(node_child);
						int reg2 = ASM_MEM_AST_NODE_GET_REG2(node_child);

						int mod = 0;
						int rm = 0;
						int sib_base = -1;
						int sib_index = -1;
						int sib_ss = -1;

						if (reg1 == INS_OPRD_NONE
								&& reg2 == INS_OPRD_NONE
								&& node_scale == NULL
								&& node_disp != NULL) {
							// [disp32]
							mod = 0;
							rm = 5;
						} else if (reg1 != INS_OPRD_NONE
										&& reg2 == INS_OPRD_NONE
										&& node_scale == NULL) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							switch (reg1) {
								case INS_AM_RAX: {
									rm = 0;
									break;
								}
								case INS_AM_RCX: {
									rm = 1;
									break;
								}
								case INS_AM_RDX: {
									rm = 2;
									break;
								}
								case INS_AM_RBX: {
									rm = 3;
									break;
								}
								case INS_AM_RSP: {
									rm = 4;
									break;
								}
								case INS_AM_RBP: {
									rm = 5;
									break;
								}
								case INS_AM_RSI: {
									rm = 6;
									break;
								}
								case INS_AM_RDI: {
									rm = 7;
									break;
								}
								case INS_AM_r8: {
									rm = 8;
									break;
								}
								case INS_AM_r9: {
									rm = 9;
									break;
								}
								case INS_AM_r10: {
									rm = 10;
									break;
								}
								case INS_AM_r11: {
									rm = 11;
									break;
								}
								case INS_AM_r12: {
									rm = 12;
									break;
								}
								case INS_AM_r13: {
									rm = 13;
									break;
								}
								case INS_AM_r14: {
									rm = 14;
									break;
								}
								case INS_AM_r15: {
									rm = 15;
									break;
								}
							}
						} else if (reg1 == INS_OPRD_NONE
										&& reg2 != INS_OPRD_NONE
										&& node_scale != NULL) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							rm = 4;
							sib_base = 5;

							switch (scale)
							{
								case 2: {
									sib_ss = 1;
									break;
								}
								case 4: {
									sib_ss = 2;
									break;
								}
								case 8: {
									sib_ss = 3;
									break;
								}
								default: {
									assert(0);
									break;
								}
							}

							switch (reg2) {
								case INS_AM_RAX: {
									sib_index = 0;
									break;
								}
								case INS_AM_RCX: {
									sib_index = 1;
									break;
								}
								case INS_AM_RDX: {
									sib_index = 2;
									break;
								}
								case INS_AM_RBX: {
									sib_index = 3;
									break;
								}
								case INS_AM_RSP: {
									sib_index = 4;
									break;
								}
								case INS_AM_RBP: {
									sib_index = 5;
									break;
								}
								case INS_AM_RSI: {
									sib_index = 6;
									break;
								}
								case INS_AM_RDI: {
									sib_index = 7;
									break;
								}
								case INS_AM_r8: {
									sib_index = 8;
									break;
								}
								case INS_AM_r9: {
									sib_index = 9;
									break;
								}
								case INS_AM_r10: {
									sib_index = 10;
									break;
								}
								case INS_AM_r11: {
									sib_index = 11;
									break;
								}
								case INS_AM_r12: {
									sib_index = 12;
									break;
								}
								case INS_AM_r13: {
									sib_index = 13;
									break;
								}
								case INS_AM_r14: {
									sib_index = 14;
									break;
								}
								case INS_AM_r15: {
									sib_index = 15;
									break;
								}
							}
						} else if (reg1 != INS_OPRD_NONE
										&& reg2 != INS_OPRD_NONE) {
							if (node_disp == NULL
									|| disp == 0) {
								mod = 0;
							} else {
								if (disp <= 0xff) {
									mod = 1;
								} else {
									mod = 2;
								}
							}

							rm = 4;

							if (node_scale != NULL) {
								switch (scale)
								{
									case 2: {
										sib_ss = 1;
										break;
									}
									case 4: {
										sib_ss = 2;
										break;
									}
									case 8: {
										sib_ss = 3;
										break;
									}
									default: {
										assert(0);
										break;
									}
								}
							}

							switch (reg1) {
								case INS_AM_RAX: {
									sib_base = 0;
									break;
								}
								case INS_AM_RCX: {
									sib_base = 1;
									break;
								}
								case INS_AM_RDX: {
									sib_base = 2;
									break;
								}
								case INS_AM_RBX: {
									sib_base = 3;
									break;
								}
								case INS_AM_RSP: {
									sib_base = 4;
									break;
								}
								case INS_AM_RBP: {
									sib_base = 5;
									break;
								}
								case INS_AM_RSI: {
									sib_base = 6;
									break;
								}
								case INS_AM_RDI: {
									sib_base = 7;
									break;
								}
								case INS_AM_r8: {
									sib_base = 8;
									break;
								}
								case INS_AM_r9: {
									sib_base = 9;
									break;
								}
								case INS_AM_r10: {
									sib_base = 10;
									break;
								}
								case INS_AM_r11: {
									sib_base = 11;
									break;
								}
								case INS_AM_r12: {
									sib_base = 12;
									break;
								}
								case INS_AM_r13: {
									sib_base = 13;
									break;
								}
								case INS_AM_r14: {
									sib_base = 14;
									break;
								}
								case INS_AM_r15: {
									sib_base = 15;
									break;
								}
							}

							switch (reg2) {
								case INS_AM_RAX: {
									sib_index = 0;
									break;
								}
								case INS_AM_RCX: {
									sib_index = 1;
									break;
								}
								case INS_AM_RDX: {
									sib_index = 2;
									break;
								}
								case INS_AM_RBX: {
									sib_index = 3;
									break;
								}
								case INS_AM_RSP: {
									sib_index = 4;
									break;
								}
								case INS_AM_RBP: {
									sib_index = 5;
									break;
								}
								case INS_AM_RSI: {
									sib_index = 6;
									break;
								}
								case INS_AM_RDI: {
									sib_index = 7;
									break;
								}
								case INS_AM_r8: {
									sib_index = 8;
									break;
								}
								case INS_AM_r9: {
									sib_index = 9;
									break;
								}
								case INS_AM_r10: {
									sib_index = 10;
									break;
								}
								case INS_AM_r11: {
									sib_index = 11;
									break;
								}
								case INS_AM_r12: {
									sib_index = 12;
									break;
								}
								case INS_AM_r13: {
									sib_index = 13;
									break;
								}
								case INS_AM_r14: {
									sib_index = 14;
									break;
								}
								case INS_AM_r15: {
									sib_index = 15;
									break;
								}
							}
						}

						ASM_MEM_AST_NODE_SET_MOD(node_child, mod);
						ASM_MEM_AST_NODE_SET_RM(node_child, rm);
						ASM_MEM_AST_NODE_SET_DISP(node_child, disp);
						ASM_MEM_AST_NODE_SET_SCALE(node_child, scale);
						ASM_MEM_AST_NODE_SET_SIB_BASE(node_child, sib_base);
						ASM_MEM_AST_NODE_SET_SIB_INDEX(node_child, sib_index);
						ASM_MEM_AST_NODE_SET_SIB_SS(node_child, sib_ss);
					}
				}
				

				InstructionEncoderData data;
				data.ctx = ctx;
				data.ins_node = ins_node;

				ins->encoder(ins, &data);

				processed = true;
				break;
			}

			if (!processed) {
				ctx->syntax_error_node_msg(
					ctx,
					ins_node,
					"instruction cannot be processed."
				);
			}
		} else {
			assert(0);
		}
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
