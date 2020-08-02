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

extern void ins_enc_opcode(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_eXX(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_rXX(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_bound_Gv_Ma(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_arpl_Ew_Gw(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_pushad_popad(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_movsxd(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_jcc(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Eb_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Ev_Iz(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Ev_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_pause(
	Instruction *ins,
	InstructionEncoderData *data
);

#endif
