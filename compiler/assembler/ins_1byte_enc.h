#ifndef __COMPILER_ASSEMBLER_INS_1BYTE_ENC_H__
#define __COMPILER_ASSEMBLER_INS_1BYTE_ENC_H__

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

extern void ins_enc_Jb(
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

extern void ins_enc_xchg_rAX_XXX(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_rAX_Ob(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_Ob_rAX(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXXsw(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXXsd(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXXsq(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_xL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_RxL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_rxX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_mov_Rx_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_ret_Iw(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_les_lds(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_xabort(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_xbegin(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void int_enc_shift(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_Iw(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_IN_AL_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_IN_eAX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_OUT_Ib_AL(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_OUT_Ib_eAX(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Eb(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Ev(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_ext_Ev(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_Iz(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_IMUL_Gv_Ev_Iz(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_XXX_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_IMUL_Gv_Ev_Ib(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_INS_OUTS_W(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_INS_OUTS_D(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_MOV_Ev_Sw(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_LEA_Gv_M(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_MOV_Sw_Ew(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_POP_Ev(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_implicit_oprd16(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_implicit_oprd32(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_opcode_implicit_oprd64(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_CALLF_Ap(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_pushfd_popfd(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_pushfq_popfq(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_enter(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_iretd(
	Instruction *ins,
	InstructionEncoderData *data
);

extern void ins_enc_iretq(
	Instruction *ins,
	InstructionEncoderData *data
);

#endif
