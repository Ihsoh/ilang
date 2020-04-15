#include "ins_1byte_1_enc.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <inttypes.h>

#include "../../lexer.h"
#include "../../parser.h"

#include "parser.h"

#include "ins.h"
#include "enc_ins.h"

void ins_enc_XXX_Eb_Gb(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *eb_node = data->ins_node->childs[0];

	ParserASTNode *gb_node = data->ins_node->childs[1];

	ins_check_operand_type(data->ctx, ins, eb_node, gb_node, NULL);

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, eb_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, gb_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_Ev_Gv(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *ev_node = data->ins_node->childs[0];

	ParserASTNode *gv_node = data->ins_node->childs[1];

	ins_check_operand_type(data->ctx, ins, ev_node, gv_node, NULL);

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, ev_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, gv_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_Gb_Eb(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *gb_node = data->ins_node->childs[0];

	ParserASTNode *eb_node = data->ins_node->childs[1];

	ins_check_operand_type(data->ctx, ins, gb_node, eb_node, NULL);

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, gb_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, eb_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_Gv_Ev(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *gv_node = data->ins_node->childs[0];

	ParserASTNode *ev_node = data->ins_node->childs[1];

	ins_check_operand_type(data->ctx, ins, gv_node, ev_node, NULL);

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, gv_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, ev_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}
