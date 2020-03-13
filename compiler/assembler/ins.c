#include "ins.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

extern Instruction ins_1byte_1[];
extern Instruction ins_1byte_2[];
extern Instruction ins_2byte_1[];

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

static void _print_ins_list(
	Instruction (*ins_list)[],
	FILE *file
) {
	assert(ins_list);
	assert(file);

	for (size_t i = 0; (*ins_list)[i].mnemonic != NULL; i++) {
		_print_ins(&((*ins_list)[i]), file);
	}
}

void ins_print_all_ins_list(
	FILE *file
) {
	_print_ins_list(&ins_1byte_1, file);
	_print_ins_list(&ins_1byte_2, file);
	_print_ins_list(&ins_2byte_1, file);
}
