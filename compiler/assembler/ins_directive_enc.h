#ifndef __COMPILER_ASSEMBLER_LEXER_H__
#define __COMPILER_ASSEMBLER_LEXER_H__

#include <inttypes.h>
#include <stddef.h>

#include "ins.h"

extern bool ins_enc_bit16(
	Instruction *ins,
	InstructionEncoderData *data
);
extern bool ins_enc_bit32(
	Instruction *ins,
	InstructionEncoderData *data
);
extern bool ins_enc_bit64(
	Instruction *ins,
	InstructionEncoderData *data
);




extern bool ins_enc_print(
	Instruction *ins,
	InstructionEncoderData *data
);

#endif
