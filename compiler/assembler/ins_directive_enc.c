#include "ins_directive_enc.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <inttypes.h>

#include "../../lexer.h"
#include "../../parser.h"

#include "parser.h"

#include "ins.h"

bool ins_enc_bit16(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT16);

	return true;
}

bool ins_enc_bit32(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT32);

	return true;
}

bool ins_enc_bit64(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);

	ASM_PARSER_CONTEXT_DATA_SET_ARCH(data->ctx, ASM_ARCH_BIT64);

	return true;
}










bool ins_enc_print(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node->nchilds == 1);

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

	return true;
}
