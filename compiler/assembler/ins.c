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
static Instruction *ins_table[] = {
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

	if (iter->next == NULL) {
		return NULL;
	}

	Instruction * current = iter->next;

	iter->next += 1;
	if (iter->next->mnemonic == NULL) {
		iter->ins_table_index += 1;
		iter->next = ins_table[iter->ins_table_index];
	}

	return current;
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
