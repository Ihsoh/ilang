#include "ins.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

static InsRegister _reg[] = {
	{"CS", INS_AM_CS, INS_REGISTER_SEG},
	{"SS", INS_AM_SS, INS_REGISTER_SEG},
	{"DS", INS_AM_DS, INS_REGISTER_SEG},
	{"ES", INS_AM_ES, INS_REGISTER_SEG},
	{"FS", INS_AM_FS, INS_REGISTER_SEG},
	{"GS", INS_AM_GS, INS_REGISTER_SEG},

	{"AL", INS_AM_AL, INS_REGISTER_GENERAL_1BYTE, 0},
	{"AH", INS_AM_AH, INS_REGISTER_GENERAL_1BYTE, 0},
	{"AX", INS_AM_AX, INS_REGISTER_GENERAL_2BYTE, 0},
	{"EAX", INS_AM_EAX, INS_REGISTER_GENERAL_4BYTE, 0},
	{"RAX", INS_AM_RAX, INS_REGISTER_GENERAL_8BYTE, 0},

	{"CL", INS_AM_CL, INS_REGISTER_GENERAL_1BYTE, 1},
	{"CH", INS_AM_CH, INS_REGISTER_GENERAL_1BYTE, 1},
	{"CX", INS_AM_CX, INS_REGISTER_GENERAL_2BYTE, 1},
	{"ECX", INS_AM_ECX, INS_REGISTER_GENERAL_4BYTE, 1},
	{"RCX", INS_AM_RCX, INS_REGISTER_GENERAL_8BYTE, 1},

	{"DL", INS_AM_DL, INS_REGISTER_GENERAL_1BYTE, 2},
	{"DH", INS_AM_DH, INS_REGISTER_GENERAL_1BYTE, 2},
	{"DX", INS_AM_DX, INS_REGISTER_GENERAL_2BYTE, 2},
	{"EDX", INS_AM_EDX, INS_REGISTER_GENERAL_4BYTE, 2},
	{"RDX", INS_AM_RDX, INS_REGISTER_GENERAL_8BYTE, 2},

	{"BL", INS_AM_BL, INS_REGISTER_GENERAL_1BYTE, 3},
	{"BH", INS_AM_BH, INS_REGISTER_GENERAL_1BYTE, 3},
	{"BX", INS_AM_BX, INS_REGISTER_GENERAL_2BYTE, 3},
	{"EBX", INS_AM_EBX, INS_REGISTER_GENERAL_4BYTE, 3},
	{"RBX", INS_AM_RBX, INS_REGISTER_GENERAL_8BYTE, 3},

	{"SPL", INS_AM_SPL, INS_REGISTER_GENERAL_1BYTE, 4},
	{"SP", INS_AM_SP, INS_REGISTER_GENERAL_2BYTE, 4},
	{"ESP", INS_AM_ESP, INS_REGISTER_GENERAL_4BYTE, 4},
	{"RSP", INS_AM_RSP, INS_REGISTER_GENERAL_8BYTE, 4},

	{"BPL", INS_AM_BPL, INS_REGISTER_GENERAL_1BYTE, 5},
	{"BP", INS_AM_BP, INS_REGISTER_GENERAL_2BYTE, 5},
	{"EBP", INS_AM_EBP, INS_REGISTER_GENERAL_4BYTE, 5},
	{"RBP", INS_AM_RBP, INS_REGISTER_GENERAL_8BYTE, 5},

	{"SIL", INS_AM_SIL, INS_REGISTER_GENERAL_1BYTE, 6},
	{"SI", INS_AM_SI, INS_REGISTER_GENERAL_2BYTE, 6},
	{"ESI", INS_AM_ESI, INS_REGISTER_GENERAL_4BYTE, 6},
	{"RSI", INS_AM_RSI, INS_REGISTER_GENERAL_8BYTE, 6},

	{"DIL", INS_AM_DIL, INS_REGISTER_GENERAL_1BYTE, 7},
	{"DI", INS_AM_DI, INS_REGISTER_GENERAL_2BYTE, 7},
	{"EDI", INS_AM_EDI, INS_REGISTER_GENERAL_4BYTE, 7},
	{"RDI", INS_AM_RDI, INS_REGISTER_GENERAL_8BYTE, 7},

	{"R8B", INS_AM_r8l, INS_REGISTER_GENERAL_1BYTE},
	{"R8W", INS_AM_r8w, INS_REGISTER_GENERAL_2BYTE},
	{"R8D", INS_AM_r8d, INS_REGISTER_GENERAL_4BYTE},
	{"R8", INS_AM_r8, INS_REGISTER_GENERAL_8BYTE},

	{"R9B", INS_AM_r9l, INS_REGISTER_GENERAL_1BYTE},
	{"R9W", INS_AM_r9w, INS_REGISTER_GENERAL_2BYTE},
	{"R9D", INS_AM_r9d, INS_REGISTER_GENERAL_4BYTE},
	{"R9", INS_AM_r9, INS_REGISTER_GENERAL_8BYTE},

	{"R10B", INS_AM_r10l, INS_REGISTER_GENERAL_1BYTE},
	{"R10W", INS_AM_r10w, INS_REGISTER_GENERAL_2BYTE},
	{"R10D", INS_AM_r10d, INS_REGISTER_GENERAL_4BYTE},
	{"R10", INS_AM_r10, INS_REGISTER_GENERAL_8BYTE},

	{"R11B", INS_AM_r11l, INS_REGISTER_GENERAL_1BYTE},
	{"R11W", INS_AM_r11w, INS_REGISTER_GENERAL_2BYTE},
	{"R11D", INS_AM_r11d, INS_REGISTER_GENERAL_4BYTE},
	{"R11", INS_AM_r11, INS_REGISTER_GENERAL_8BYTE},

	{"R12B", INS_AM_r12l, INS_REGISTER_GENERAL_1BYTE},
	{"R12W", INS_AM_r12w, INS_REGISTER_GENERAL_2BYTE},
	{"R12D", INS_AM_r12d, INS_REGISTER_GENERAL_4BYTE},
	{"R12", INS_AM_r12, INS_REGISTER_GENERAL_8BYTE},

	{"R13B", INS_AM_r13l, INS_REGISTER_GENERAL_1BYTE},
	{"R13W", INS_AM_r13w, INS_REGISTER_GENERAL_2BYTE},
	{"R13D", INS_AM_r13d, INS_REGISTER_GENERAL_4BYTE},
	{"R13", INS_AM_r13, INS_REGISTER_GENERAL_8BYTE},

	{"R14B", INS_AM_r14l, INS_REGISTER_GENERAL_1BYTE},
	{"R14W", INS_AM_r14w, INS_REGISTER_GENERAL_2BYTE},
	{"R14D", INS_AM_r14d, INS_REGISTER_GENERAL_4BYTE},
	{"R14", INS_AM_r14, INS_REGISTER_GENERAL_8BYTE},

	{"R15B", INS_AM_r15l, INS_REGISTER_GENERAL_1BYTE},
	{"R15W", INS_AM_r15w, INS_REGISTER_GENERAL_2BYTE},
	{"R15D", INS_AM_r15d, INS_REGISTER_GENERAL_4BYTE},
	{"R15", INS_AM_r15, INS_REGISTER_GENERAL_8BYTE},

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
