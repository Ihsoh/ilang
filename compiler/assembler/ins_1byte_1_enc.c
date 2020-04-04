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

	

	EncoderInstruction enc_ins;
	memset(&enc_ins, 0, sizeof(EncoderInstruction));

	// Group 1
	enc_ins.legacy_prefix.lock = false;
	enc_ins.legacy_prefix.repne = false;
	enc_ins.legacy_prefix.rep = false;

	// Group 2
	int seg = ASM_MEM16_AST_NODE_GET_SEG(eb_node);
	if (seg == INS_AM_CS) {
		enc_ins.legacy_prefix.cs = true;
	}
	if (seg == INS_AM_SS) {
		enc_ins.legacy_prefix.ss = true;
	}
	if (seg == INS_AM_DS) {
		enc_ins.legacy_prefix.ds = true;
	}
	if (seg == INS_AM_ES) {
		enc_ins.legacy_prefix.es = true;
	}
	if (seg == INS_AM_FS) {
		enc_ins.legacy_prefix.fs = true;
	}
	if (seg == INS_AM_GS) {
		enc_ins.legacy_prefix.gs = true;
	}
	enc_ins.legacy_prefix.branch_not_taken = false;
	enc_ins.legacy_prefix.branch_taken = false;
	enc_ins.legacy_prefix.operand_size_override = false;
	enc_ins.legacy_prefix.address_size_override = false;

	enc_ins.rex_prefix_used = false;
	enc_ins.rex_prefix.w = false;
	enc_ins.rex_prefix.r = false;
	enc_ins.rex_prefix.x = false;
	enc_ins.rex_prefix.b = false;

	enc_ins.opcode_len = ins->opcode.len;
	enc_ins.opcode[0] = ins->opcode.o1;
	enc_ins.opcode[1] = ins->opcode.o2;
	enc_ins.opcode[2] = ins->opcode.o3;

	int mod = ASM_MEM16_AST_NODE_GET_MOD(eb_node);
	int rm = ASM_MEM16_AST_NODE_GET_RM(eb_node);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.mod = mod;
	enc_ins.mod_rm.rm = rm;

	InsRegister *ins_reg = ASM_REG_AST_NODE_GET_REG(gb_node);
	int reg = ins_reg->index;
	enc_ins.mod_rm.reg = reg;

	enc_ins.sib_used = false;
	enc_ins.sib.scale = 0;
	enc_ins.sib.index = 0;
	enc_ins.sib.base = 0;

	uint32_t disp = ASM_MEM16_AST_NODE_GET_DISP(eb_node);

	enc_ins.disp_len = 0;
	if (mod == 0) {
		enc_ins.disp_len = 0;
	} else if (mod == 1) {
		enc_ins.disp_len = 1;
	} else if (mod == 2) {
		enc_ins.disp_len = 2;
	} else {
		assert(0);
	}
	enc_ins.disp = disp;
	
	enc_ins.imm_len = 0;
	enc_ins.imm = 0;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}
