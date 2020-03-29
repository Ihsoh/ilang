#include "ins_directive_enc.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <inttypes.h>

#include "../../lexer.h"
#include "../../parser.h"

#include "parser.h"

#include "ins.h"

void ins_enc_bit16(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT16);
}

void ins_enc_bit32(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT32);
}

void ins_enc_bit64(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT64);
}

void ins_enc_int8(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	fwrite(&result->value.u64, 1, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_int16(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	fwrite(&result->value.u64, 2, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_int32(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	fwrite(&result->value.u64, 4, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_int64(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	fwrite(&result->value.u64, 8, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_float(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_DOUBLE);

	float f = (float) result->value.d;

	fwrite(&f, 4, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_double(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_DOUBLE);

	fwrite(&result->value.d, 8, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_string(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, 1);
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_STRING);

	fwrite(result->value.str.ptr, result->value.str.len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}











void ins_enc_print(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	ParserASTNode *child = data->ins_node->childs[0];
	
	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(child);

	switch (result->type) {
		case ASM_EXPR_EVAL_RESULT_TYPE_UINT64: {
			printf("%llu\n", result->value.u64);
			break;
		}
		case ASM_EXPR_EVAL_RESULT_TYPE_DOUBLE: {
			printf("%lf\n", result->value.d);
			break;
		}
		case ASM_EXPR_EVAL_RESULT_TYPE_STRING: {
			for (size_t i = 0; i < result->value.str.len; i++) {
				putchar(result->value.str.ptr[i]);
			}
			putchar('\n');
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}
