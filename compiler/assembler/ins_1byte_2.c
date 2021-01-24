#include "ins.h"

#include "ins_1byte_enc.h"

Instruction ins_1byte_2[] = {
	// 0x00, 0x08~0x0f
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x08},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x09},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x0a},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"OR", INS_SS_NONE,
		{1, 0x00 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"PUSH", INS_SS_i64,
		{1, 0x00 | 0x0e},
		{
			INS_AM_CS
		},
		ins_enc_opcode
	},

	// 0x01, 0x08~0x0f
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x08},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x09},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x0a},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"SBB", INS_SS_NONE,
		{1, 0x10 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"PUSH", INS_SS_i64,
		{1, 0x10 | 0x0e},
		{
			INS_AM_DS
		},
		ins_enc_opcode
	},
	{
		"POP", INS_SS_i64,
		{1, 0x10 | 0x0f},
		{
			INS_AM_DS
		},
		ins_enc_opcode
	},

	// 0x02, 0x08~0x0f
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x08},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x09},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x0a},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"SUB", INS_SS_NONE,
		{1, 0x20 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"CS", INS_SS_PREFIX,
		{1, 0x20 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DAS", INS_SS_i64,
		{1, 0x20 | 0x0f},
		{
		},
		ins_enc_opcode
	},

	// 0x03, 0x08~0x0f
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x08},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x09},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x0a},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"CMP", INS_SS_NONE,
		{1, 0x30 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"DS", INS_SS_PREFIX,
		{1, 0x30 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAS", INS_SS_i64,
		{1, 0x30 | 0x0f},
		{
		},
		ins_enc_opcode
	},

	// 0x04, 0x08~0x0f
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x08},
		{
			INS_AM_eAX
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x09},
		{
			INS_AM_eCX
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0a},
		{
			INS_AM_eDX
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0b},
		{
			INS_AM_eBX
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0c},
		{
			INS_AM_eSP
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0d},
		{
			INS_AM_eBP
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0e},
		{
			INS_AM_eSI
		},
		ins_enc_opcode_eXX
	},
	{
		"DEC", INS_SS_i64,
		{1, 0x40 | 0x0f},
		{
			INS_AM_eDI
		},
		ins_enc_opcode_eXX
	},

	{
		"REX.W", INS_SS_PREFIX,
		{1, 0x40 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WB", INS_SS_PREFIX,
		{1, 0x40 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WX", INS_SS_PREFIX,
		{1, 0x40 | 0x0a},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WXB", INS_SS_PREFIX,
		{1, 0x40 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WR", INS_SS_PREFIX,
		{1, 0x40 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRB", INS_SS_PREFIX,
		{1, 0x40 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRX", INS_SS_PREFIX,
		{1, 0x40 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRXB", INS_SS_PREFIX,
		{1, 0x40 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x05, 0x08~0x0f
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x08},
		{
			INS_AM_rAX
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x09},
		{
			INS_AM_rCX
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0a},
		{
			INS_AM_rDX
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0b},
		{
			INS_AM_rBX
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0c},
		{
			INS_AM_rSP
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0d},
		{
			INS_AM_rBP
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0e},
		{
			INS_AM_rSI
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0f},
		{
			INS_AM_rDI
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x08},
		{
			INS_AM_r8
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x09},
		{
			INS_AM_r9
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0a},
		{
			INS_AM_r10
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0b},
		{
			INS_AM_r11
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0c},
		{
			INS_AM_r12
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0d},
		{
			INS_AM_r13
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0e},
		{
			INS_AM_r14
		},
		ins_enc_opcode_rXX
	},
	{
		"POP", INS_SS_d64,
		{1, 0x50 | 0x0f},
		{
			INS_AM_r15
		},
		ins_enc_opcode_rXX
	},

	// 0x06, 0x08~0x0f
	{
		"PUSH", INS_SS_d64,
		{1, 0x60 | 0x08},
		{
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_Iz
	},
	{
		"IMUL", INS_SS_NONE,
		{1, 0x60 | 0x09},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_IMUL_Gv_Ev_Iz
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x60 | 0x0a},
		{
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_Ib
	},
	{
		"IMUL", INS_SS_NONE,
		{1, 0x60 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_IMUL_Gv_Ev_Ib
	},
	{
		"INSB", INS_SS_NONE,
		{1, 0x60 | 0x0c},
		{
		},
		ins_enc_opcode
	},
	{
		"INSW", INS_SS_NONE,
		{1, 0x60 | 0x0d},
		{
		},
		ins_enc_INS_OUTS_W
	},
	{
		"INSD", INS_SS_NONE,
		{1, 0x60 | 0x0d},
		{
		},
		ins_enc_INS_OUTS_D
	},
	{
		"OUTSB", INS_SS_NONE,
		{1, 0x60 | 0x0e},
		{
		},
		ins_enc_opcode
	},
	{
		"OUTSW", INS_SS_NONE,
		{1, 0x60 | 0x0f},
		{
		},
		ins_enc_INS_OUTS_W
	},
	{
		"OUTSD", INS_SS_NONE,
		{1, 0x60 | 0x0f},
		{
		},
		ins_enc_INS_OUTS_D
	},

	// 0x07, 0x08~0x0f
	{
		"JS", INS_SS_f64,
		{1, 0x70 | 0x08},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JNS", INS_SS_f64,
		{1, 0x70 | 0x09},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JP", INS_SS_f64,	// P/PE
		{1, 0x70 | 0x0a},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JNP", INS_SS_f64,	// NP/PO
		{1, 0x70 | 0x0b},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JL", INS_SS_f64,	// L/NGE
		{1, 0x70 | 0x0c},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JNL", INS_SS_f64,	// NL/GE
		{1, 0x70 | 0x0d},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JLE", INS_SS_f64,	// LE/NG
		{1, 0x70 | 0x0e},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},
	{
		"JNLE", INS_SS_f64,	// NLE/G
		{1, 0x70 | 0x0f},
		{
			INS_AM_J | INS_OT_b
		},
		ins_enc_Jb
	},

	// 0x08, 0x08~0x0f
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x08},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x09},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x0a},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x0c},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_S | INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LEA", INS_SS_NONE,
		{1, 0x80 | 0x0d},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_M
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0x80 | 0x0e},
		{
			INS_AM_S | INS_OT_w,
			INS_AM_E | INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Grp 1A
	// POP
	{
		"POP", INS_SS_1A | INS_SS_d64,
		{1, 0x80 | 0x0f},
		{
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// 0x09, 0x08~0x0f
	{
		"CBW", INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CWDE", INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CDQE", INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CWD", INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CDQ", INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CQO", INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CALL", INS_SS_i64,
		{1, 0x90 | 0x0a},
		{
			INS_AM_FAR,
			INS_AM_A | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"FWAIT", INS_SS_NONE,
		{1, 0x90 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"WAIT", INS_SS_NONE,
		{1, 0x90 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHF", INS_SS_NONE,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHFD", INS_SS_NONE,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHFQ", INS_SS_d64,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPF", INS_SS_NONE,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPFD", INS_SS_NONE,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPFQ", INS_SS_d64,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SAHF", INS_SS_NONE,
		{1, 0x90 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LAHF", INS_SS_NONE,
		{1, 0x90 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0a, 0x08~0x0f
	{
		"TEST", INS_SS_NONE,
		{1, 0xa0 | 0x08},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"TEST", INS_SS_NONE,
		{1, 0xa0 | 0x09},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOS", INS_SS_NONE,
		{1, 0xa0 | 0x0a},
		{
			INS_AM_Y | INS_OT_b,
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSB", INS_SS_NONE,
		{1, 0xa0 | 0x0a},
		{
			INS_AM_Y | INS_OT_b,
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOS", INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			INS_AM_Y | INS_OT_v,
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSW", INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			INS_AM_Y | INS_OT_v,
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSD", INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			INS_AM_Y | INS_OT_v,
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSQ", INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			INS_AM_Y | INS_OT_v,
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODS", INS_SS_NONE,
		{1, 0xa0 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_X | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSB", INS_SS_NONE,
		{1, 0xa0 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_X | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODS", INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_X | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSW", INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_X | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSD", INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_X | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSQ", INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			INS_AM_rAX,
			INS_AM_X | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCAS", INS_SS_NONE,
		{1, 0xa0 | 0x0e},
		{
			INS_AM_AL,
			INS_AM_Y | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASB", INS_SS_NONE,
		{1, 0xa0 | 0x0e},
		{
			INS_AM_AL,
			INS_AM_Y | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCAS", INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			INS_AM_rAX,
			INS_AM_Y | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASW", INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			INS_AM_rAX,
			INS_AM_Y | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASD", INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			INS_AM_rAX,
			INS_AM_Y | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASQ", INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			INS_AM_rAX,
			INS_AM_Y | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0b, 0x08~0x0f
	// MOV immediate word or double into word, double, or quad register
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x09},
		{
			INS_AM_rCX,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			INS_AM_rDX,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			INS_AM_rBX,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
			INS_AM_rSP,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			INS_AM_rBP,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0e},
		{
			INS_AM_rSI,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
			INS_AM_rDI,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			INS_AM_r8,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x09},
		{
			INS_AM_r9,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			INS_AM_r10,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			INS_AM_r11,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
			INS_AM_r12,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			INS_AM_r13,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0e},
		{
			INS_AM_r14,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
			INS_AM_r15,
			INS_AM_I | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0c, 0x08~0x0f
	{
		"ENTER", INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			INS_AM_I | INS_OT_w,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LEAVE", INS_SS_d64,
		{1, 0xb0 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			INS_AM_FAR,
			INS_AM_I | INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			INS_AM_FAR
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT3", INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT", INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INTO", INS_SS_i64,
		{1, 0xb0 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRET", INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRETD", INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRETQ", INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0d, 0x08~0x0f
	// ESC (Escape to coprocessor instruction set)

	// 0x0e, 0x08~0x0f
	{
		"CALL", INS_SS_f64,
		{1, 0xe0 | 0x08},
		{
			INS_AM_NEAR,
			INS_AM_J | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", INS_SS_f64,
		{1, 0xe0 | 0x09},
		{
			INS_AM_NEAR,
			INS_AM_J | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", INS_SS_i64,
		{1, 0xe0 | 0x0a},
		{
			INS_AM_FAR,
			INS_AM_A | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", INS_SS_f64,
		{1, 0xe0 | 0x0b},
		{
			INS_AM_SHORT,
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", INS_SS_NONE,
		{1, 0xe0 | 0x0c},
		{
			INS_AM_AL,
			INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", INS_SS_NONE,
		{1, 0xe0 | 0x0d},
		{
			INS_AM_eAX,
			INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", INS_SS_NONE,
		{1, 0xe0 | 0x0e},
		{
			INS_AM_DX,
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", INS_SS_NONE,
		{1, 0xe0 | 0x0f},
		{
			INS_AM_DX,
			INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0f, 0x08~0x0f
	{
		"CLC", INS_SS_NONE,
		{1, 0xf0 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STC", INS_SS_NONE,
		{1, 0xf0 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CLI", INS_SS_NONE,
		{1, 0xf0 | 0x0a},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STI", INS_SS_NONE,
		{1, 0xf0 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CLD", INS_SS_NONE,
		{1, 0xf0 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STD", INS_SS_NONE,
		{1, 0xf0 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	
	// INC/DEC Grp 4
	{
		"INC", INS_SS_1A,
		{1, 0xf0 | 0x0e},
		{
			INS_AM_E | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"DEC", INS_SS_1A,
		{1, 0xf0 | 0x0e},
		{
			INS_AM_E | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// INC/DEC Grp 5
	{
		"INC", INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"DEC", INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"CALL", INS_SS_1A | INS_SS_f64,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_NEAR,
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"CALL", INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_FAR,
			INS_AM_E | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"JMP", INS_SS_1A | INS_SS_f64,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_NEAR,
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"JMP", INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_FAR,
			INS_AM_M | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"PUSH", INS_SS_1A | INS_SS_d64,
		{1, 0xf0 | 0x0f},
		{
			INS_AM_E | INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},




	{
		NULL
	}
};
