#ifndef __COMPILER_ASSEMBLER_LEXER_H__
#define __COMPILER_ASSEMBLER_LEXER_H__

#include <inttypes.h>
#include <stddef.h>

#include "ins.h"

extern void ins_enc_bit16(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_bit32(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_bit64(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_int8(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_int16(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_int32(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_int64(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_float(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_double(
	Instruction *ins,
	InstructionEncoderData *data
);
extern void ins_enc_string(
	Instruction *ins,
	InstructionEncoderData *data
);






extern void ins_enc_label(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_set(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_print(
	Instruction *ins,
	InstructionEncoderData *data
);

#endif
