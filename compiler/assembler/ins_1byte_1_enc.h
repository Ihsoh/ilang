#ifndef __COMPILER_ASSEMBLER_INS_1BYTE_1_ENC_H__
#define __COMPILER_ASSEMBLER_INS_1BYTE_1_ENC_H__

#include <inttypes.h>
#include <stddef.h>

#include "ins.h"

extern void ins_enc_XXX_Eb_Gb(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_Ev_Gv(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_Gb_Eb(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_Gv_Ev(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_AL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_XXX_AL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_XXX_rAX_Iz(
	Instruction *ins,
	InstructionEncoderData *data
);

void ins_enc_opcode(
	Instruction *ins,
	InstructionEncoderData *data
);

#endif
