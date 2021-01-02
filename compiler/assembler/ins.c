#include "ins.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../../parser.h"
#include "../../lexer.h"

#include "parser.h"
#include "lexer.h"
#include "enc_ins.h"

static InsRegister _reg[] = {
	{
		"CS",
		INS_AM_CS,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"SS",
		INS_AM_SS,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"DS",
		INS_AM_DS,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"ES",
		INS_AM_ES,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"FS",
		INS_AM_FS,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"GS",
		INS_AM_GS,
		INS_REGISTER_SEG,
		0,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},

	{
		"AL",
		INS_AM_AL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_AX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"AH",
		INS_AM_AH,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_AX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"AX",
		INS_AM_AX,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_AX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"EAX",
		INS_AM_EAX,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_AX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RAX",
		INS_AM_RAX,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_AX,
		ASM_ARCH_BIT64
	},

	{
		"CL",
		INS_AM_CL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_CX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"CH",
		INS_AM_CH,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_CX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"CX",
		INS_AM_CX,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_CX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"ECX",
		INS_AM_ECX,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_CX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RCX",
		INS_AM_RCX,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_CX,
		ASM_ARCH_BIT64
	},

	{
		"DL",
		INS_AM_DL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_DX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"DH",
		INS_AM_DH,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_DX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"DX",
		INS_AM_DX,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_DX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"EDX",
		INS_AM_EDX,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_DX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RDX",
		INS_AM_RDX,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_DX,
		ASM_ARCH_BIT64
	},

	{
		"BL",
		INS_AM_BL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_BX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"BH",
		INS_AM_BH,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_BX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"BX",
		INS_AM_BX,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_BX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"EBX",
		INS_AM_EBX,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_BX,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RBX",
		INS_AM_RBX,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_BX,
		ASM_ARCH_BIT64
	},

	{
		"SPL",
		INS_AM_SPL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_SP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"SP",
		INS_AM_SP,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_SP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"ESP",
		INS_AM_ESP,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_SP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RSP",
		INS_AM_RSP,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_SP,
		ASM_ARCH_BIT64
	},

	{
		"BPL",
		INS_AM_BPL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_BP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"BP",
		INS_AM_BP,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_BP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"EBP",
		INS_AM_EBP,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_BP,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RBP",
		INS_AM_RBP,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_BP,
		ASM_ARCH_BIT64
	},

	{
		"SIL",
		INS_AM_SIL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_SI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"SI",
		INS_AM_SI,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_SI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"ESI",
		INS_AM_ESI,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_SI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RSI",
		INS_AM_RSI,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_SI, ASM_ARCH_BIT64
	},

	{
		"DIL",
		INS_AM_DIL,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_DI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"DI",
		INS_AM_DI,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_DI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"EDI",
		INS_AM_EDI,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_DI,
		ASM_ARCH_BIT16 | ASM_ARCH_BIT32 | ASM_ARCH_BIT64
	},
	{
		"RDI",
		INS_AM_RDI,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_DI,
		ASM_ARCH_BIT64
	},

	{
		"R8B",
		INS_AM_r8l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R8,
		ASM_ARCH_BIT64
	},
	{
		"R8W",
		INS_AM_r8w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R8,
		ASM_ARCH_BIT64
	},
	{
		"R8D",
		INS_AM_r8d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R8,
		ASM_ARCH_BIT64
	},
	{
		"R8",
		INS_AM_r8,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R8,
		ASM_ARCH_BIT64
	},

	{
		"R9B",
		INS_AM_r9l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R9,
		ASM_ARCH_BIT64
	},
	{
		"R9W",
		INS_AM_r9w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R9,
		ASM_ARCH_BIT64
	},
	{
		"R9D",
		INS_AM_r9d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R9,
		ASM_ARCH_BIT64
	},
	{
		"R9",
		INS_AM_r9,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R9,
		ASM_ARCH_BIT64
	},

	{
		"R10B",
		INS_AM_r10l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R10,
		ASM_ARCH_BIT64
	},
	{
		"R10W",
		INS_AM_r10w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R10,
		ASM_ARCH_BIT64
	},
	{
		"R10D",
		INS_AM_r10d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R10,
		ASM_ARCH_BIT64
	},
	{
		"R10",
		INS_AM_r10,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R10,
		ASM_ARCH_BIT64
	},

	{
		"R11B",
		INS_AM_r11l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R11,
		ASM_ARCH_BIT64
	},
	{
		"R11W",
		INS_AM_r11w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R11,
		ASM_ARCH_BIT64
	},
	{
		"R11D",
		INS_AM_r11d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R11,
		ASM_ARCH_BIT64
	},
	{
		"R11",
		INS_AM_r11,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R11,
		ASM_ARCH_BIT64
	},

	{
		"R12B",
		INS_AM_r12l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R12,
		ASM_ARCH_BIT64
	},
	{
		"R12W",
		INS_AM_r12w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R12,
		ASM_ARCH_BIT64
	},
	{
		"R12D",
		INS_AM_r12d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R12,
		ASM_ARCH_BIT64
	},
	{
		"R12",
		INS_AM_r12,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R12,
		ASM_ARCH_BIT64
	},

	{
		"R13B",
		INS_AM_r13l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R13,
		ASM_ARCH_BIT64
	},
	{
		"R13W",
		INS_AM_r13w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R13,
		ASM_ARCH_BIT64
	},
	{
		"R13D",
		INS_AM_r13d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R13,
		ASM_ARCH_BIT64
	},
	{
		"R13",
		INS_AM_r13,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R13,
		ASM_ARCH_BIT64
	},

	{
		"R14B",
		INS_AM_r14l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R14,
		ASM_ARCH_BIT64
	},
	{
		"R14W",
		INS_AM_r14w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R14,
		ASM_ARCH_BIT64
	},
	{
		"R14D",
		INS_AM_r14d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R14,
		ASM_ARCH_BIT64
	},
	{
		"R14",
		INS_AM_r14,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R14,
		ASM_ARCH_BIT64
	},

	{
		"R15B",
		INS_AM_r15l,
		INS_REGISTER_GENERAL_1BYTE,
		INS_REGISTER_INDEX_R15,
		ASM_ARCH_BIT64
	},
	{
		"R15W",
		INS_AM_r15w,
		INS_REGISTER_GENERAL_2BYTE,
		INS_REGISTER_INDEX_R15,
		ASM_ARCH_BIT64
	},
	{
		"R15D",
		INS_AM_r15d,
		INS_REGISTER_GENERAL_4BYTE,
		INS_REGISTER_INDEX_R15,
		ASM_ARCH_BIT64
	},
	{
		"R15",
		INS_AM_r15,
		INS_REGISTER_GENERAL_8BYTE,
		INS_REGISTER_INDEX_R15,
		ASM_ARCH_BIT64
	},

	{
		NULL
	}
};

void ins_reg_iter_init(
	InsRegisterIterator *iter
) {
	assert(iter);

	iter->next = _reg;
}

InsRegister * ins_reg_iter_next(
	InsRegisterIterator *iter
) {
	assert(iter);

	if (iter->next == NULL
			|| iter->next->name == NULL) {
		return NULL;
	}

	return iter->next++;
}

void ins_reg_iter_free(
	InsRegisterIterator *iter
) {
	assert(iter);

	iter->next = NULL;
}

InsRegister * ins_reg_get_by_name(
	const char *name
) {
	assert(name);

	InsRegister *ins_reg = NULL;

	InsRegisterIterator iter;
	ins_reg_iter_init(&iter);
	for (InsRegister *reg = ins_reg_iter_next(&iter); reg != NULL; reg = ins_reg_iter_next(&iter)) {
		assert(reg->name);

		if (strcmp(reg->name, name) == 0) {
			ins_reg = reg;
		}
	}
	ins_reg_iter_free(&iter);

	return ins_reg;
}

InsRegister * ins_reg_get_by_id(
	int id
) {
	InsRegister *ins_reg = NULL;

	InsRegisterIterator iter;
	ins_reg_iter_init(&iter);
	for (InsRegister *reg = ins_reg_iter_next(&iter); reg != NULL; reg = ins_reg_iter_next(&iter)) {
		assert(reg->name);

		if (reg->id == id) {
			ins_reg = reg;
		}
	}
	ins_reg_iter_free(&iter);

	return ins_reg;
}




extern Instruction ins_directive[];
extern Instruction ins_1byte_1[];
extern Instruction ins_1byte_2[];
extern Instruction ins_2byte_1[];
static Instruction *ins_table[] = {
	ins_directive,
	ins_1byte_1,
	ins_1byte_2,
	ins_2byte_1,
	NULL
};

void ins_iter_init(
	InstructionIterator *iter
) {
	assert(iter);

	iter->ins_table_index = 0;
	iter->next = ins_table[0];
}

Instruction * ins_iter_next(
	InstructionIterator *iter
) {
	assert(iter);

	// 检测是否到达ins_table的结尾。
	if (iter->next == NULL) {
		return NULL;
	}

	// 一直更换ins_table里的ins_*，直到取出一个mnemonic不为NULL的项。
	// ins_*列表的最后一项的mnemonic为NULL。
	// 有的ins_*可能只有一个mnemonic为NULL的项，所以要用循环跳过去。
	while (iter->next->mnemonic == NULL) {
		iter->ins_table_index += 1;
		iter->next = ins_table[iter->ins_table_index];
		if (iter->next == NULL) {
			return NULL;
		}
	}

	return iter->next++;
}

void ins_iter_free(
	InstructionIterator *iter
) {
	assert(iter);

	iter->ins_table_index = 0;
	iter->next = NULL;
}

void ins_init(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_INSTRUCTION);
	assert(enc_ins);

	memset(enc_ins, 0, sizeof(EncoderInstruction));

	enc_ins->legacy_prefix.lock = false;
	enc_ins->legacy_prefix.repne = false;
	enc_ins->legacy_prefix.rep = false;

	enc_ins->legacy_prefix.branch_not_taken = false;
	enc_ins->legacy_prefix.branch_taken = false;

	enc_ins->opcode_len = ins->opcode.len;
	enc_ins->opcode[0] = ins->opcode.o1;
	enc_ins->opcode[1] = ins->opcode.o2;
	enc_ins->opcode[2] = ins->opcode.o3;
}

void ins_fill_GX(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_REG);
	assert(enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(ctx);

	InsRegister *ins_reg = ASM_REG_AST_NODE_GET_REG(node);
	assert(ins_reg);

	enc_ins->mod_rm.reg = ins_reg->index & 0x7;	// 0x7 == 0b0111

	if (ins_reg->index & 0x08) {	// 0x8 == 0b1000
		enc_ins->rex_prefix_used = true;
		enc_ins->rex_prefix.r = true;
	}

	if (arch == ASM_ARCH_BIT16) {
		// Operand size
		if (ins_reg->type == INS_REGISTER_GENERAL_4BYTE) {
			enc_ins->legacy_prefix.operand_size_override = true;
		}
	} else if (arch == ASM_ARCH_BIT32) {
		// Operand size
		if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
			enc_ins->legacy_prefix.operand_size_override = true;
		}
	} else if (arch == ASM_ARCH_BIT64) {
		// Operand size
		if (ins->defult_oprd_sz_64bits) {
			if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}
		} else {
			if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
				enc_ins->legacy_prefix.operand_size_override = true;
			} else if (ins_reg->type == INS_REGISTER_GENERAL_8BYTE) {
				enc_ins->rex_prefix_used = true;
				enc_ins->rex_prefix.w = true;
			}
		}
	}
}

void ins_fill_EX(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(enc_ins);

	int arch = ASM_PARSER_CONTEXT_DATA_GET_ARCH(ctx);

	enc_ins->mod_rm_used = true;

	if (node->type == ASM_NODE_MEM) {
		int seg = ASM_MEM_AST_NODE_GET_SEG(node);
		if (seg == INS_AM_CS) {
			enc_ins->legacy_prefix.cs = true;
		}
		if (seg == INS_AM_SS) {
			enc_ins->legacy_prefix.ss = true;
		}
		if (seg == INS_AM_DS) {
			enc_ins->legacy_prefix.ds = true;
		}
		if (seg == INS_AM_ES) {
			enc_ins->legacy_prefix.es = true;
		}
		if (seg == INS_AM_FS) {
			enc_ins->legacy_prefix.fs = true;
		}
		if (seg == INS_AM_GS) {
			enc_ins->legacy_prefix.gs = true;
		}
	} else {
		enc_ins->legacy_prefix.cs = false;
		enc_ins->legacy_prefix.ss = false;
		enc_ins->legacy_prefix.ds = false;
		enc_ins->legacy_prefix.es = false;
		enc_ins->legacy_prefix.fs = false;
		enc_ins->legacy_prefix.gs = false;
	}

	if (node->type == ASM_NODE_REG) {
		InsRegister *ins_reg = ASM_REG_AST_NODE_GET_REG(node);
		assert(ins_reg);

		enc_ins->mod_rm.mod = 3;
		enc_ins->mod_rm.rm = ins_reg->index & 0x7;	// 0x7 == 0b0111

		if (ins_reg->index & 0x08) {	// 0x8 == 0b1000
			enc_ins->rex_prefix_used = true;
			enc_ins->rex_prefix.b = true;
		}

		if (arch == ASM_ARCH_BIT16) {
			// Operand size
			if (ins_reg->type == INS_REGISTER_GENERAL_4BYTE) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}
		} else if (arch == ASM_ARCH_BIT32) {
			// Operand size
			if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}
		} else if (arch == ASM_ARCH_BIT64) {
			// Operand size
			if (ins->defult_oprd_sz_64bits) {
				if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
					enc_ins->legacy_prefix.operand_size_override = true;
				}
			} else {
				if (ins_reg->type == INS_REGISTER_GENERAL_2BYTE) {
					enc_ins->legacy_prefix.operand_size_override = true;
				} else if (ins_reg->type == INS_REGISTER_GENERAL_8BYTE) {
					enc_ins->rex_prefix_used = true;
					enc_ins->rex_prefix.w = true;
				}
			}
		}
	} else if (node->type == ASM_NODE_MEM
					&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node) == ASM_MEM_ADDR_SIZE_16) {
		int type = ASM_MEM_AST_NODE_GET_TYPE(node);
		int mod = ASM_MEM_AST_NODE_GET_MOD(node);
		int rm = ASM_MEM_AST_NODE_GET_RM(node);
		uint32_t disp = ASM_MEM_AST_NODE_GET_DISP(node);

		if (mod == -1 && rm == -1) {
			mod = 1;
			rm = 6;
			enc_ins->disp_len = 1;
			enc_ins->disp = 0;
		} else if (mod == 0 && rm == 0x6) {
			enc_ins->disp_len = 2;
			enc_ins->disp = disp;
		} else {
			if (mod == 0) {
				enc_ins->disp_len = 0;
			} else if (mod == 1) {
				enc_ins->disp_len = 1;
			} else if (mod == 2) {
				enc_ins->disp_len = 2;
			} else {
				assert(0);
			}
			enc_ins->disp = disp;
		}

		enc_ins->mod_rm.mod = mod;
		enc_ins->mod_rm.rm = rm;

		if (arch == ASM_ARCH_BIT16) {
			// Operand size
			if (type == ASM_MEM_TYPE_DWORD) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}

			// Address size
			// None
		} else if (arch == ASM_ARCH_BIT32) {
			// Operand size
			if (type == ASM_MEM_TYPE_WORD) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}

			// Address size
			enc_ins->legacy_prefix.address_size_override = true;
		} else if (arch == ASM_ARCH_BIT64) {
			assert(0);
		}
	} else if (node->type == ASM_NODE_MEM
					&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node) == ASM_MEM_ADDR_SIZE_32) {
		int type = ASM_MEM_AST_NODE_GET_TYPE(node);
		int mod = ASM_MEM_AST_NODE_GET_MOD(node);
		int rm = ASM_MEM_AST_NODE_GET_RM(node);
		int sib_base = ASM_MEM_AST_NODE_GET_SIB_BASE(node);
		int sib_index = ASM_MEM_AST_NODE_GET_SIB_INDEX(node);
		int sib_ss = ASM_MEM_AST_NODE_GET_SIB_SS(node);
		uint32_t disp = ASM_MEM_AST_NODE_GET_DISP(node);

		if (mod == 0 && rm == 0x5) {
			enc_ins->disp_len = 4;
			enc_ins->disp = disp;
		} else {
			if (mod == 0) {
				enc_ins->disp_len = 0;
			} else if (mod == 1) {
				enc_ins->disp_len = 1;
			} else if (mod == 2) {
				enc_ins->disp_len = 4;
			} else {
				assert(0);
			}
			enc_ins->disp = disp;
		}

		enc_ins->mod_rm.mod = mod;
		enc_ins->mod_rm.rm = rm;

		if (sib_base != -1 && sib_index != -1 && sib_ss != -1) {
			enc_ins->sib_used = true;
			enc_ins->sib.base = sib_base;
			enc_ins->sib.index = sib_index;
			enc_ins->sib.scale = sib_ss;
		}

		if (arch == ASM_ARCH_BIT16) {
			// Operand size
			if (type == ASM_MEM_TYPE_DWORD) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}

			// Address size
			enc_ins->legacy_prefix.address_size_override = true;
		} else if (arch == ASM_ARCH_BIT32) {
			// Operand size
			if (type == ASM_MEM_TYPE_WORD) {
				enc_ins->legacy_prefix.operand_size_override = true;
			}

			// Address size
			// None
		} else if (arch == ASM_ARCH_BIT64) {
			// Operand size
			if (ins->defult_oprd_sz_64bits) {
				if (type == ASM_MEM_TYPE_WORD) {
					enc_ins->legacy_prefix.operand_size_override = true;
				}
			} else {
				if (type == ASM_MEM_TYPE_WORD) {
					enc_ins->legacy_prefix.operand_size_override = true;
				} else if (type == ASM_MEM_TYPE_QWORD) {
					enc_ins->rex_prefix_used = true;
					enc_ins->rex_prefix.w = true;
				}
			}

			// Address size
			enc_ins->legacy_prefix.address_size_override = true;
		}
	} else if (node->type == ASM_NODE_MEM
					&& ASM_MEM_AST_NODE_GET_ADDR_SIZE(node) == ASM_MEM_ADDR_SIZE_64) {
		int type = ASM_MEM_AST_NODE_GET_TYPE(node);
		int mod = ASM_MEM_AST_NODE_GET_MOD(node);
		int rm = ASM_MEM_AST_NODE_GET_RM(node);
		int sib_base = ASM_MEM_AST_NODE_GET_SIB_BASE(node);
		int sib_index = ASM_MEM_AST_NODE_GET_SIB_INDEX(node);
		int sib_ss = ASM_MEM_AST_NODE_GET_SIB_SS(node);
		uint64_t disp = ASM_MEM_AST_NODE_GET_DISP(node);

		if (mod == 0 && rm == 0x5) {
			enc_ins->disp_len = 4;
			enc_ins->disp = disp;
		} else {
			if (mod == 0) {
				enc_ins->disp_len = 0;
			} else if (mod == 1) {
				enc_ins->disp_len = 1;
			} else if (mod == 2) {
				enc_ins->disp_len = 4;
			} else {
				assert(0);
			}
			enc_ins->disp = disp;
		}

		enc_ins->mod_rm.mod = mod;
		enc_ins->mod_rm.rm = rm & 0x7;	// 0x7 == 0b0111
		if (rm & 0x8) {	// 0x8 == 0b1000
			enc_ins->rex_prefix_used = true;
			enc_ins->rex_prefix.b = true;
		}

		if (sib_base != -1 && sib_index != -1 && sib_ss != -1) {
			enc_ins->sib_used = true;
			
			enc_ins->sib.base = sib_base & 0x7;	// 0x7 == 0b0111
			if (sib_base & 0x8) {	// 0x8 == 0b1000
				enc_ins->rex_prefix_used = true;
				enc_ins->rex_prefix.b = true;
			}

			enc_ins->sib.index = sib_index & 0x7;	// 0x7 == 0b0111
			if (sib_index & 0x8) {	// 0x8 == 0b1000
				enc_ins->rex_prefix_used = true;
				enc_ins->rex_prefix.x = true;
			}

			enc_ins->sib.scale = sib_ss;
		}

		if (arch == ASM_ARCH_BIT16) {
			assert(0);
		} else if (arch == ASM_ARCH_BIT32) {
			assert(0);
		} else if (arch == ASM_ARCH_BIT64) {
			// Operand size
			if (ins->defult_oprd_sz_64bits) {
				if (type == ASM_MEM_TYPE_WORD) {
					enc_ins->legacy_prefix.operand_size_override = true;
				}
			} else {
				if (type == ASM_MEM_TYPE_WORD) {
					enc_ins->legacy_prefix.operand_size_override = true;
				} else if (type == ASM_MEM_TYPE_QWORD) {
					enc_ins->rex_prefix_used = true;
					enc_ins->rex_prefix.w = true;
				}
			}

			// Address size
			// None
		}
	} else {
		assert(0);
	}
}

void ins_fill_imm8(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_EXPR);
	assert(enc_ins);

	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	enc_ins->imm_len = 1;
	enc_ins->imm = result->value.u64 & 0xff;
}

void ins_fill_imm16(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_EXPR);
	assert(enc_ins);

	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	enc_ins->imm_len = 2;
	enc_ins->imm = result->value.u64 & 0xffff;
}

void ins_fill_imm32(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_EXPR);
	assert(enc_ins);

	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	enc_ins->imm_len = 4;
	enc_ins->imm = result->value.u64 & 0xffffffff;
}

void ins_fill_imm64(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *node,
	EncoderInstruction *enc_ins
) {
	assert(ctx);
	assert(ins);
	assert(node);
	assert(node->type == ASM_NODE_EXPR);
	assert(enc_ins);

	AsmExprEvalResult *result = &ASM_EXPR_AST_NODE_GET_RESULT(node);
	assert(result->type == ASM_EXPR_EVAL_RESULT_TYPE_UINT64);

	enc_ins->imm_len = 4;
	enc_ins->imm = result->value.u64;
}








size_t _get_size(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	if (node->type == ASM_NODE_REG) {
		InsRegister *ins_reg = ASM_REG_AST_NODE_GET_REG(node);
		assert(ins_reg);

		switch (ins_reg->type) {
			case INS_REGISTER_GENERAL_1BYTE: {
				return 1;
			}
			case INS_REGISTER_GENERAL_2BYTE: {
				return 2;
			}
			case INS_REGISTER_GENERAL_4BYTE: {
				return 4;
			}
			case INS_REGISTER_GENERAL_8BYTE: {
				return 8;
			}
			default: {
				assert(0);
				return 0;
			}
		}
	} else if (node->type == ASM_NODE_MEM) {
		int type = ASM_MEM_AST_NODE_GET_TYPE(node);

		switch (type) {
			case ASM_MEM_TYPE_BYTE: {
				return 1;
			}
			case ASM_MEM_TYPE_WORD: {
				return 2;
			}
			case ASM_MEM_TYPE_DWORD: {
				return 4;
			}
			case ASM_MEM_TYPE_QWORD: {
				return 8;
			}
			default: {
				assert(0);
				return 0;
			}
		}
	} else {
		assert(0);
		return 0;
	}
}

size_t ins_get_oprd_size(
	ParserContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	return _get_size(ctx, node);
}

void ins_check_operand_type(
	ParserContext *ctx,
	Instruction *ins,
	ParserASTNode *a,
	ParserASTNode *b,
	ParserASTNode *c
) {
	assert(ctx);
	assert(ins);
	assert(a);
	assert(b);

	size_t sz_a = _get_size(ctx, a);
	size_t sz_b = _get_size(ctx, b);
	if (sz_a != sz_b) {
		ctx->syntax_error_node_msg(ctx, a, "operand type not matched.");
	}

	if (c != NULL) {
		size_t sz_c = _get_size(ctx, c);
		if (sz_b != sz_c) {
			ctx->syntax_error_node_msg(ctx, c, "operand type not matched.");
		}
	}
}

void ins_enc_not_implemented(
	Instruction *ins,
	InstructionEncoderData *data
) {
	printf("ERROR, NOT IMPLEMENTED: %s\n", ins->mnemonic);
	assert(0);
	exit(-1);
}

static void _print_ins(
	Instruction *ins,
	FILE *file
) {
	assert(ins);
	assert(file);

	fprintf(file, "%s\n", ins->mnemonic);
}

void ins_print_all_ins_list(
	FILE *file
) {
	assert(file);

	InstructionIterator iter;
	ins_iter_init(&iter);
	for (Instruction *ins = ins_iter_next(&iter); ins != NULL; ins = ins_iter_next(&iter)) {
		_print_ins(ins, file);
	}
	ins_iter_free(&iter);
}
