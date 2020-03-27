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
	{"CS", INS_AM_CS},
	{"SS", INS_AM_SS},
	{"DS", INS_AM_DS},
	{"ES", INS_AM_ES},
	{"FS", INS_AM_FS},
	{"GS", INS_AM_GS},

	{"AL", INS_AM_AL},
	{"AH", INS_AM_AH},
	{"AX", INS_AM_AX},
	{"EAX", INS_AM_EAX},
	{"RAX", INS_AM_RAX},

	{"CL", INS_AM_CL},
	{"CH", INS_AM_CH},
	{"CX", INS_AM_CX},
	{"ECX", INS_AM_ECX},
	{"RCX", INS_AM_RCX},

	{"DL", INS_AM_DL},
	{"DH", INS_AM_DH},
	{"DX", INS_AM_DX},
	{"EDX", INS_AM_EDX},
	{"RDX", INS_AM_RDX},

	{"BL", INS_AM_BL},
	{"BH", INS_AM_BH},
	{"BX", INS_AM_BX},
	{"EBX", INS_AM_EBX},
	{"RBX", INS_AM_RBX},

	{"SPL", INS_AM_SPL},
	{"SP", INS_AM_SP},
	{"ESP", INS_AM_ESP},
	{"RSP", INS_AM_RSP},

	{"BPL", INS_AM_BPL},
	{"BP", INS_AM_BP},
	{"EBP", INS_AM_EBP},
	{"RBP", INS_AM_RBP},

	{"SIL", INS_AM_SIL},
	{"SI", INS_AM_SI},
	{"ESI", INS_AM_ESI},
	{"RSI", INS_AM_RSI},

	{"DIL", INS_AM_DIL},
	{"DI", INS_AM_DI},
	{"EDI", INS_AM_EDI},
	{"RDI", INS_AM_RDI},

	{"R8B", INS_AM_r8l},
	{"R8W", INS_AM_r8w},
	{"R8D", INS_AM_r8d},
	{"R8", INS_AM_r8},

	{"R9B", INS_AM_r9l},
	{"R9W", INS_AM_r9w},
	{"R9D", INS_AM_r9d},
	{"R9", INS_AM_r9},

	{"R10B", INS_AM_r10l},
	{"R10W", INS_AM_r10w},
	{"R10D", INS_AM_r10d},
	{"R10", INS_AM_r10},

	{"R11B", INS_AM_r11l},
	{"R11W", INS_AM_r11w},
	{"R11D", INS_AM_r11d},
	{"R11", INS_AM_r11},

	{"R12B", INS_AM_r12l},
	{"R12W", INS_AM_r12w},
	{"R12D", INS_AM_r12d},
	{"R12", INS_AM_r12},

	{"R13B", INS_AM_r13l},
	{"R13W", INS_AM_r13w},
	{"R13D", INS_AM_r13d},
	{"R13", INS_AM_r13},

	{"R14B", INS_AM_r14l},
	{"R14W", INS_AM_r14w},
	{"R14D", INS_AM_r14d},
	{"R14", INS_AM_r14},

	{"R15B", INS_AM_r15l},
	{"R15W", INS_AM_r15w},
	{"R15D", INS_AM_r15d},
	{"R15", INS_AM_r15},

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









bool ins_enc_not_implemented(
	Instruction *ins,
	InstructionEncoderData *data
) {
	assert(0);
	return false;
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
