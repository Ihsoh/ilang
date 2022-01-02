#include "ins_1byte_enc.h"

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

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
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

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
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

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
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

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_AL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *al_node = data->ins_node->childs[0];

	ParserASTNode *ib_node = data->ins_node->childs[1];

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm8(data->ctx, ins, ib_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_rAX_Iz(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	InsRegister *target_reg = ASM_REG_AST_NODE_GET_REG(target);

	switch (arch) {
		case ASM_ARCH_BIT16: {
			switch (target_reg->id) {
				case INS_AM_AX: {
					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_EAX: {
					enc_ins.legacy_prefix.operand_size_override = true;
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_RAX: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 16-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			switch (target_reg->id) {
				case INS_AM_AX: {
					enc_ins.legacy_prefix.operand_size_override = true;
					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_EAX: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_RAX: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 32-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			switch (target_reg->id) {
				case INS_AM_AX: {
					enc_ins.legacy_prefix.operand_size_override = true;
					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_EAX: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_AM_RAX: {
					enc_ins.rex_prefix_used = true;
					enc_ins.rex_prefix.w = 1;
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_eXX(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	InsRegister *target_reg = ASM_REG_AST_NODE_GET_REG(target);

	switch (arch) {
		case ASM_ARCH_BIT16: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_rXX(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	InsRegister *target_reg = ASM_REG_AST_NODE_GET_REG(target);

	switch (arch) {
		case ASM_ARCH_BIT16: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 16-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 32-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 64-bit mode."
					);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_bound_Gv_Ma(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	size_t target_size = ins_get_oprd_size(data->ctx, target);
	if (target_size != 2 && target_size != 4) {
		data->ctx->syntax_error_node_msg(data->ctx, target, "operand type not matched.");
	}

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_arpl_Ew_Gw(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	ins_fill_GX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_pushad_popad(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	switch (arch) {
		case ASM_ARCH_BIT16: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT32: {
			break;
		}
		case ASM_ARCH_BIT64: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 64-bit mode."
			);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_movsxd(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	size_t target_size = ins_get_oprd_size(data->ctx, target);
	if (target_size != 8) {
		data->ctx->syntax_error_node_msg(
			data->ctx,
			target,
			"operand type not matched."
		);
	}

	size_t source_size = ins_get_oprd_size(data->ctx, source);
	if (source_size != 4) {
		data->ctx->syntax_error_node_msg(
			data->ctx,
			target,
			"operand type not matched."
		);
	}

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_Jb(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *node_label = data->ins_node->childs[0];

	uint64_t addr = asm_parser_get_symbol_by_token_key(data->ctx, node_label->token);

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.disp_len = 1;
	enc_ins.disp = 0;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	uint64_t ip = ASM_PARSER_CONTEXT_DATA_GET_ADDRESS_COUNTER(data->ctx) + len;
	uint64_t offset = addr - ip;
	enc_ins.disp = offset;

	len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_ext_Eb_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_ext_Ev_Iz(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	size_t target_size = ins_get_oprd_size(data->ctx, target);
	switch (target_size) {
		case 2: {
			ins_fill_imm16(data->ctx, ins, source, &enc_ins);
			break;
		}
		case 4:
		case 8: {
			ins_fill_imm32(data->ctx, ins, source, &enc_ins);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_ext_Ev_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_pause(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.legacy_prefix.rep = true;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_xchg_rAX_XXX(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	ins_check_operand_type(data->ctx, ins, target, source, NULL);

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	InsRegister *ins_reg_target = ASM_REG_AST_NODE_GET_REG(target);
	InsRegister *ins_reg_source = ASM_REG_AST_NODE_GET_REG(source);
	InsRegister *ins_reg_another = NULL;
	if (ins_reg_target->index == INS_REGISTER_INDEX_AX) {
		ins_reg_another = ins_reg_source;
	} else {
		ins_reg_another = ins_reg_target;
	}

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);

	switch (arch) {
		case ASM_ARCH_BIT16: {
			if (!(ins_reg_another->arch & ASM_ARCH_BIT16)) {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 16-bit mode."
				);
			}

			if (ins_reg_another->type == INS_REGISTER_GENERAL_2BYTE) {

			} else if (ins_reg_another->type == INS_REGISTER_GENERAL_4BYTE) {
				enc_ins.legacy_prefix.operand_size_override = true;
			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 16-bit mode."
				);
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			if (!(ins_reg_another->arch & ASM_ARCH_BIT32)) {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 32-bit mode."
				);
			}

			if (ins_reg_another->type == INS_REGISTER_GENERAL_2BYTE) {
				enc_ins.legacy_prefix.operand_size_override = true;
			} else if (ins_reg_another->type == INS_REGISTER_GENERAL_4BYTE) {

			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 32-bit mode."
				);
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			if (!(ins_reg_another->arch & ASM_ARCH_BIT64)) {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 64-bit mode."
				);
			}

			if (ins_reg_another->index & 0x08) {
				enc_ins.rex_prefix_used = true;
				enc_ins.rex_prefix.r = true;
			}

			if (ins_reg_another->type == INS_REGISTER_GENERAL_2BYTE) {
				enc_ins.legacy_prefix.operand_size_override = true;
			} else if (ins_reg_another->type == INS_REGISTER_GENERAL_4BYTE) {

			} else if (ins_reg_another->type == INS_REGISTER_GENERAL_8BYTE) {
				
			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 64-bit mode."
				);
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_rAX_Ob(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);
	if (arch == ASM_ARCH_BIT64) {
		enc_ins.disp_len = 8;
	}

	enc_ins.mod_rm_used = false;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_Ob_rAX(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);
	if (arch == ASM_ARCH_BIT64) {
		enc_ins.disp_len = 8;
	}

	enc_ins.mod_rm_used = false;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXXsw(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);
	switch (arch) {
		case ASM_ARCH_BIT16: {
			break;
		}
		case ASM_ARCH_BIT32: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT64: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXXsd(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);
	switch (arch) {
		case ASM_ARCH_BIT16: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT32: {
			break;
		}
		case ASM_ARCH_BIT64: {
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXXsq(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx);
	switch (arch) {
		case ASM_ARCH_BIT16: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 16-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT32: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 32-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT64: {
			enc_ins.rex_prefix_used = true;
			enc_ins.rex_prefix.w = true;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_xL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);
	
	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_RxL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);
	
	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	enc_ins.rex_prefix_used = true;
	enc_ins.rex_prefix.b = true;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_rxX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	InsRegister *ins_target_reg = ASM_REG_AST_NODE_GET_REG(target);
	assert(ins_target_reg);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 64-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 64-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_mov_Rx_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);
	
	InsRegister *ins_target_reg = ASM_REG_AST_NODE_GET_REG(target);
	assert(ins_target_reg);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 64-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					data->ctx->syntax_error_node_msg(
						data->ctx,
						ins_node,
						"instruction not supported in 64-bit mode."
					);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			switch (ins_target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;

					ins_fill_imm16(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				case INS_REGISTER_GENERAL_8BYTE: {
					ins_fill_imm32(data->ctx, ins, source, &enc_ins);
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	enc_ins.rex_prefix_used = true;
	enc_ins.rex_prefix.b = true;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}









void ins_enc_ret_Iw(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *source = data->ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm16(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_les_lds(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	ParserASTNode *source = data->ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_xabort(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *source = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.mod = 0x3;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_xbegin(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *node_label = ins_node->childs[0];

	uint64_t addr = asm_parser_get_symbol_by_token_key(data->ctx, node_label->token);

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.mod = 0x3;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			enc_ins.disp_len = 2;
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			enc_ins.disp_len = 4;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
	enc_ins.disp = 0;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	uint64_t ip = ASM_PARSER_CONTEXT_DATA_GET_ADDRESS_COUNTER(data->ctx) + len;
	uint64_t offset = addr - ip;
	enc_ins.disp = offset;

	len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void int_enc_shift(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = data->ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *source = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_IN_AL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_IN_eAX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	InsRegister *target_reg = ASM_REG_AST_NODE_GET_REG(target);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			switch (target_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	ins_fill_imm8(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_OUT_Ib_AL(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm8(data->ctx, ins, target, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_OUT_Ib_eAX(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	InsRegister *source_reg = ASM_REG_AST_NODE_GET_REG(source);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			switch (source_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			switch (source_reg->type) {
				case INS_REGISTER_GENERAL_2BYTE: {
					enc_ins.legacy_prefix.operand_size_override = true;
					break;
				}
				case INS_REGISTER_GENERAL_4BYTE: {
					// 无需操作数大小前缀。
					break;
				}
				default: {
					assert(0);
					break;
				}
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	ins_fill_imm8(data->ctx, ins, target, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_ext_Eb(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, oprd, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_ext_Ev(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	enc_ins.mod_rm_used = true;
	enc_ins.mod_rm.reg = ins->opcode_ext.reg;

	ins_fill_EX(data->ctx, ins, oprd, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_Iz(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			ins_fill_imm16(data->ctx, ins, oprd, &enc_ins);
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			ins_fill_imm32(data->ctx, ins, oprd, &enc_ins);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_IMUL_Gv_Ev_Iz(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 3);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source1 = ins_node->childs[1];

	ParserASTNode *source2 = ins_node->childs[2];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source1, &enc_ins);

	ins_check_operand_type(data->ctx, ins, target, source1, NULL);

	switch (ins_get_oprd_size(data->ctx, target)) {
		case 2: {
			ins_fill_imm16(data->ctx, ins, source2, &enc_ins);
			break;
		}
		case 4:
		case 8: {
			ins_fill_imm32(data->ctx, ins, source2, &enc_ins);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_XXX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_imm8(data->ctx, ins, oprd, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_IMUL_Gv_Ev_Ib(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 3);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source1 = ins_node->childs[1];

	ParserASTNode *source2 = ins_node->childs[2];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source1, &enc_ins);

	ins_check_operand_type(data->ctx, ins, target, source1, NULL);

	ins_fill_imm8(data->ctx, ins, source2, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_INS_OUTS_W(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_INS_OUTS_D(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_MOV_Ev_Sw(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_EX(data->ctx, ins, target, &enc_ins);

	ins_fill_GX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_LEA_Gv_M(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_MOV_Sw_Ew(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 2);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *target = ins_node->childs[0];

	ParserASTNode *source = ins_node->childs[1];

	EncoderInstruction enc_ins;

	ins_init(data->ctx, ins, ins_node, &enc_ins);

	ins_fill_GX(data->ctx, ins, target, &enc_ins);

	ins_fill_EX(data->ctx, ins, source, &enc_ins);

	enc_ins.legacy_prefix.operand_size_override = false;

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_POP_Ev(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	size_t oprd_size = ins_get_oprd_size(data->ctx, oprd);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			if (oprd_size == 2
					|| oprd_size == 4) {
				ins_fill_EX(data->ctx, ins, oprd, &enc_ins);
			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 16-bit mode."
				);
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			if (oprd_size == 2
					|| oprd_size == 4) {
				ins_fill_EX(data->ctx, ins, oprd, &enc_ins);
			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 32-bit mode."
				);
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			if (oprd_size == 2
					|| oprd_size == 8) {
				ins_fill_EX(data->ctx, ins, oprd, &enc_ins);
			} else {
				data->ctx->syntax_error_node_msg(
					data->ctx,
					ins_node,
					"instruction not supported in 64-bit mode."
				);
			}
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_implicit_oprd16(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {

			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_implicit_oprd32(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT32:
		case ASM_ARCH_BIT64: {

			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_opcode_implicit_oprd64(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 16-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT32: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 32-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT64: {
			enc_ins.rex_prefix_used = true;
			enc_ins.rex_prefix.w = true;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_CALLF_Ap(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 1);

	ParserASTNode *ins_node = data->ins_node;

	ParserASTNode *oprd = ins_node->childs[0];

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	int addr_size = ASM_DIRECT_ADDRESS_AST_NODE_GET_ADDR_SIZE(oprd);

	uint64_t base = ASM_DIRECT_ADDRESS_AST_NODE_GET_BASE(oprd);
	uint64_t offset = ASM_DIRECT_ADDRESS_AST_NODE_GET_OFFSET(oprd);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			if (addr_size == ASM_MEM_ADDR_SIZE_UNKNOWN
					|| addr_size == ASM_MEM_ADDR_SIZE_16) {
				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) offset
				);

				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) base
				);
			} else if (addr_size == ASM_MEM_ADDR_SIZE_32) {
				enc_ins.legacy_prefix.operand_size_override = true;

				ins_add_uint32_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint32_t) offset
				);

				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) base
				);
			} else {
				assert(0);
			}
			break;
		}
		case ASM_ARCH_BIT32: {
			if (addr_size == ASM_MEM_ADDR_SIZE_16) {
				enc_ins.legacy_prefix.operand_size_override = true;

				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) offset
				);

				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) base
				);
			} else if (addr_size == ASM_MEM_ADDR_SIZE_32
					|| addr_size == ASM_MEM_ADDR_SIZE_UNKNOWN) {
				ins_add_uint32_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint32_t) offset
				);

				ins_add_uint16_to_ext(
					data->ctx,
					ins,
					&enc_ins,
					(uint16_t) base
				);
			} else {
				assert(0);
			}
			break;
		}
		case ASM_ARCH_BIT64: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 64-bit mode."
			);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_pushfd_popfd(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			enc_ins.legacy_prefix.operand_size_override = true;
			break;
		}
		case ASM_ARCH_BIT32: {

			break;
		}
		case ASM_ARCH_BIT64: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 64-bit mode."
			);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}

void ins_enc_pushfq_popfq(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(ins);
	assert(data);
	assert(data->ins_node);
	assert(data->ins_node->nchilds == 0);

	ParserASTNode *ins_node = data->ins_node;

	EncoderInstruction enc_ins;
	
	ins_init(data->ctx, ins, ins_node, &enc_ins);

	switch (ASM_PARSER_CONTEXT_DATA_GET_ARCH(data->ctx)) {
		case ASM_ARCH_BIT16: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 16-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT32: {
			data->ctx->syntax_error_node_msg(
				data->ctx,
				ins_node,
				"instruction not supported in 32-bit mode."
			);
			break;
		}
		case ASM_ARCH_BIT64: {
			
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	uint8_t buffer[32];
	size_t len = enc_ins_encode(&enc_ins, buffer, sizeof(buffer));

	ASM_PARSER_CONTEXT_DATA_INC_ADDRESS_COUNTER(data->ctx, len);
	if (ASM_PARSER_CONTEXT_DATA_GET_STEP(data->ctx) == ASM_STEP_SCAN) {
		return;
	}

	fwrite(buffer, len, 1, ASM_PARSER_CONTEXT_DATA_GET_OUT(data->ctx));
}
