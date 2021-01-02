#include "ins.h"

#include "ins_1byte_1_enc.h"

Instruction ins_1byte_1[] = {
	// 0x00, 0x00~0x07
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x02},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x04},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"ADD", INS_SS_NONE,
		{1, 0x00 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"PUSH", INS_SS_i64,
		{1, 0x00 | 0x06},
		{
			INS_AM_ES
		},
		ins_enc_opcode
	},
	{
		"POP", INS_SS_i64,
		{1, 0x00 | 0x07},
		{
			INS_AM_ES
		},
		ins_enc_opcode
	},

	// 0x01, 0x00~0x07
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x02},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x04},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"ADC", INS_SS_NONE,
		{1, 0x10 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"PUSH", INS_SS_i64,
		{1, 0x10 | 0x06},
		{
			INS_AM_SS
		},
		ins_enc_opcode
	},
	{
		"POP", INS_SS_i64,
		{1, 0x10 | 0x07},
		{
			INS_AM_SS
		},
		ins_enc_opcode
	},

	// 0x02, 0x00~0x07
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x02},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x04},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"AND", INS_SS_NONE,
		{1, 0x20 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"ES", INS_SS_PREFIX,
		{1, 0x20 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DAA", INS_SS_i64,
		{1, 0x20 | 0x07},
		{
		},
		ins_enc_opcode
	},

	// 0x03, 0x00~0x07
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x02},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x04},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_XXX_AL_Ib
	},
	{
		"XOR", INS_SS_NONE,
		{1, 0x30 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_I | INS_OT_z
		},
		ins_enc_XXX_rAX_Iz
	},
	{
		"SS", INS_SS_PREFIX,
		{1, 0x30 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAA", INS_SS_i64,
		{1, 0x30 | 0x07},
		{
		},
		ins_enc_opcode
	},

	// 0x04, 0x00~0x07
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x00},
		{
			INS_AM_eAX
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x01},
		{
			INS_AM_eCX
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x02},
		{
			INS_AM_eDX
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x03},
		{
			INS_AM_eBX
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x04},
		{
			INS_AM_eSP
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x05},
		{
			INS_AM_eBP
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x06},
		{
			INS_AM_eSI
		},
		ins_enc_opcode_eXX
	},
	{
		"INC", INS_SS_i64,
		{1, 0x40 | 0x07},
		{
			INS_AM_eDI
		},
		ins_enc_opcode_eXX
	},
	{
		"REX", INS_SS_PREFIX,
		{1, 0x40 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.B", INS_SS_PREFIX,
		{1, 0x40 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.X", INS_SS_PREFIX,
		{1, 0x40 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.XB", INS_SS_PREFIX,
		{1, 0x40 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.R", INS_SS_PREFIX,
		{1, 0x40 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RB", INS_SS_PREFIX,
		{1, 0x40 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RX", INS_SS_PREFIX,
		{1, 0x40 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RXB", INS_SS_PREFIX,
		{1, 0x40 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x05, 0x00~0x07
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x00},
		{
			INS_AM_rAX
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x01},
		{
			INS_AM_rCX
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x02},
		{
			INS_AM_rDX
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x03},
		{
			INS_AM_rBX
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x04},
		{
			INS_AM_rSP
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x05},
		{
			INS_AM_rBP
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x06},
		{
			INS_AM_rSI
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x07},
		{
			INS_AM_rDI
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x00},
		{
			INS_AM_r8
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x01},
		{
			INS_AM_r9
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x02},
		{
			INS_AM_r10
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x03},
		{
			INS_AM_r11
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x04},
		{
			INS_AM_r12
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x05},
		{
			INS_AM_r13
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x06},
		{
			INS_AM_r14
		},
		ins_enc_opcode_rXX
	},
	{
		"PUSH", INS_SS_d64,
		{1, 0x50 | 0x07},
		{
			INS_AM_r15
		},
		ins_enc_opcode_rXX
	},

	// 0x06, 0x00~0x07
	{
		"PUSHA", INS_SS_i64,
		{1, 0x60 | 0x00},
		{
		},
		ins_enc_opcode
	},
	{
		"POPA", INS_SS_i64,
		{1, 0x60 | 0x01},
		{
		},
		ins_enc_opcode
	},
	{
		"BOUND", INS_SS_i64,
		{1, 0x60 | 0x02},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_M | INS_OT_a
		},
		ins_enc_bound_Gv_Ma
	},
	{
		"ARPL", INS_SS_i64,
		{1, 0x60 | 0x03},
		{
			INS_AM_E | INS_OT_w,
			INS_AM_G | INS_OT_w
		},
		ins_enc_arpl_Ew_Gw
	},
	{
		"FS", INS_SS_PREFIX,
		{1, 0x60 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"GS", INS_SS_PREFIX,
		{1, 0x60 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OPRD_SIZE", INS_SS_PREFIX,
		{1, 0x60 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADDR_SIZE", INS_SS_PREFIX,
		{1, 0x60 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHAD", INS_SS_i64,
		{1, 0x60 | 0x00},
		{
		},
		ins_enc_pushad_popad
	},
	{
		"POPAD", INS_SS_i64,
		{1, 0x60 | 0x01},
		{
		},
		ins_enc_pushad_popad
	},
	{
		"MOVSXD", INS_SS_o64,
		{1, 0x60 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_movsxd
	},

	// 0x07, 0x00~0x07
	{
		"JO", INS_SS_f64,
		{1, 0x70 | 0x00},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JNO", INS_SS_f64,
		{1, 0x70 | 0x01},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JB", INS_SS_f64,	// B/NAE/C
		{1, 0x70 | 0x02},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JNB", INS_SS_f64,	// NB/AE/NC
		{1, 0x70 | 0x03},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JZ", INS_SS_f64,	// Z/E
		{1, 0x70 | 0x04},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JNZ", INS_SS_f64,	// NZ/NE
		{1, 0x70 | 0x05},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JBE", INS_SS_f64,	// BE/NA
		{1, 0x70 | 0x06},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},
	{
		"JNBE", INS_SS_f64,	// NBE/A
		{1, 0x70 | 0x07},
		{
			INS_AM_LABEL
		},
		ins_enc_jcc
	},

	// 0x08, 0x00~0x07
	// Group1-ADD
	{
		"ADD", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// Group1-OR
	{
		"OR", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// Group1-ADC
	{
		"ADC", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// Group1-SBB
	{
		"SBB", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// Group1-AND
	{
		"AND", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// Group1-SUB
	{
		"SUB", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// Group1-XOR
	{
		"XOR", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// Group1-CMP
	{
		"CMP", INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		ins_enc_opcode_ext_Ev_Iz,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", INS_SS_1A | INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Eb_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_opcode_ext_Ev_Ib,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"TEST", INS_SS_NONE,
		{1, 0x80 | 0x04},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"TEST", INS_SS_NONE,
		{1, 0x80 | 0x05},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x80 | 0x06},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_XXX_Eb_Gb
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x80 | 0x06},
		{
			INS_AM_G | INS_OT_b,
			INS_AM_E | INS_OT_b
		},
		ins_enc_XXX_Gb_Eb
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x80 | 0x07},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_XXX_Ev_Gv
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x80 | 0x07},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_XXX_Gv_Ev
	},

	// 0x09, 0x00~0x07
	{
		"NOP", INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
		},
		ins_enc_opcode
	},
	{
		"PAUSE", INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
		},
		ins_enc_pause
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
			INS_AM_r8,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
			INS_AM_rAX,
			INS_AM_r8
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			INS_AM_rCX,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			INS_AM_rAX,
			INS_AM_rCX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			INS_AM_r9,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			INS_AM_rAX,
			INS_AM_r9
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			INS_AM_rDX,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			INS_AM_rAX,
			INS_AM_rDX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			INS_AM_r10,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			INS_AM_rAX,
			INS_AM_r10
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			INS_AM_rBX,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			INS_AM_rAX,
			INS_AM_rBX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			INS_AM_r11,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			INS_AM_rAX,
			INS_AM_r11
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			INS_AM_rSP,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			INS_AM_rAX,
			INS_AM_rSP
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			INS_AM_r12,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			INS_AM_rAX,
			INS_AM_r12
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			INS_AM_rBP,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_rBP
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			INS_AM_r13,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			INS_AM_rAX,
			INS_AM_r13
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			INS_AM_rSI,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			INS_AM_rAX,
			INS_AM_rSI
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			INS_AM_r14,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			INS_AM_rAX,
			INS_AM_r14
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			INS_AM_rDI,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			INS_AM_rAX,
			INS_AM_rDI
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			INS_AM_r15,
			INS_AM_rAX
		},
		ins_enc_xchg_rAX_XXX
	},
	{
		"XCHG", INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			INS_AM_rAX,
			INS_AM_r15
		},
		ins_enc_xchg_rAX_XXX
	},

	// 0x0a, 0x00~0x07
	{
		"MOV", INS_SS_NONE,
		{1, 0xa0 | 0x00},
		{
			INS_AM_AL,
			INS_AM_O | INS_OT_b
		},
		ins_enc_mov_rAX_Ob
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xa0 | 0x01},
		{
			INS_AM_rAX,
			INS_AM_O | INS_OT_v
		},
		ins_enc_mov_rAX_Ob
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xa0 | 0x02},
		{
			INS_AM_O | INS_OT_b,
			INS_AM_AL
		},
		ins_enc_mov_Ob_rAX
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xa0 | 0x03},
		{
			INS_AM_O | INS_OT_v,
			INS_AM_rAX
		},
		ins_enc_mov_Ob_rAX
	},
	{
		"MOVSB", INS_SS_NONE,
		{1, 0xa0 | 0x04},
		{
		},
		ins_enc_opcode
	},
	{
		"MOVSW", INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		ins_enc_XXXsw
	},
	{
		"MOVSD", INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		ins_enc_XXXsd
	},
	{
		"MOVSQ", INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		ins_enc_XXXsq
	},
	{
		"CMPSB", INS_SS_NONE,
		{1, 0xa0 | 0x06},
		{
		},
		ins_enc_opcode
	},
	{
		"CMPSW", INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		ins_enc_XXXsw
	},
	{
		"CMPSD", INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		ins_enc_XXXsd
	},
	{
		"CMPSQ", INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		ins_enc_XXXsq
	},

	// 0x0b, 0x00~0x07
	// MOV immediate byte into byte register
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x00},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x00},
		{
			INS_AM_r8l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x01},
		{
			INS_AM_CL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x01},
		{
			INS_AM_r9l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x02},
		{
			INS_AM_DL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x02},
		{
			INS_AM_r10l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x03},
		{
			INS_AM_BL,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x03},
		{
			INS_AM_r11l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x04},
		{
			INS_AM_AH,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x04},
		{
			INS_AM_r12l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x05},
		{
			INS_AM_CH,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x05},
		{
			INS_AM_r13l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x06},
		{
			INS_AM_DH,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x06},
		{
			INS_AM_r14l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x07},
		{
			INS_AM_BH,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_xL_Ib
	},
	{
		"MOV", INS_SS_NONE,
		{1, 0xb0 | 0x07},
		{
			INS_AM_r15l,
			INS_AM_I | INS_OT_b
		},
		ins_enc_mov_RxL_Ib
	},

	// 0x0c, 0x00~0x07
	// Shift Grp 2

	// ROL
	{
		"ROL", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// ROR
	{
		"ROR", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// RCL
	{
		"RCL", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// RCR
	{
		"RCR", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// SHL/SAL
	// SHL和SAL是同一条指令，指令码完全一样。
	{
		"SHL", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// SHR
	{
		"SHR", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// SAR
	{
		"SAR", INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"RET", INS_SS_f64,
		{1, 0xc0 | 0x02},
		{
			INS_AM_NEAR,
			INS_AM_I | INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", INS_SS_f64,
		{1, 0xc0 | 0x03},
		{
			INS_AM_NEAR
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LES", INS_SS_i64,
		{1, 0xc0 | 0x04},
		{
			INS_AM_G | INS_OT_z,
			INS_AM_M | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LDS", INS_SS_i64,
		{1, 0xc0 | 0x05},
		{
			INS_AM_G | INS_OT_z,
			INS_AM_M | INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Grp 11 - MOV
	// MOV
	{
		"MOV", INS_SS_1A,
		{1, 0xc0 | 0x06},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"MOV", INS_SS_1A,
		{1, 0xc0 | 0x07},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_I | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// 0x0d, 0x00~0x07
	// Shift Grp 2
	// ROL
	{
		"ROL", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// ROR
	{
		"ROR", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// RCL
	{
		"RCL", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// RCR
	{
		"RCR", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// SHL/SAL
	{
		"SHL", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// SHR
	{
		"SHR", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// SAR
	{
		"SAR", INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"AAM", INS_SS_i64,
		{1, 0xd0 | 0x04},
		{
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAD", INS_SS_i64,
		{1, 0xd0 | 0x05},
		{
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XLAT", INS_SS_NONE,
		{1, 0xd0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XLATB", INS_SS_NONE,
		{1, 0xd0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0e, 0x00~0x07
	{
		"LOOPNE", INS_SS_f64,
		{1, 0xe0 | 0x00},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPNZ", INS_SS_f64,
		{1, 0xe0 | 0x00},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPE", INS_SS_f64,
		{1, 0xe0 | 0x01},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPZ", INS_SS_f64,
		{1, 0xe0 | 0x01},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOP", INS_SS_f64,
		{1, 0xe0 | 0x02},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JCXZ", INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JECXZ", INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JRCXZ", INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			INS_AM_J | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", INS_SS_NONE,
		{1, 0xe0 | 0x04},
		{
			INS_AM_AL,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", INS_SS_NONE,
		{1, 0xe0 | 0x05},
		{
			INS_AM_eAX,
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", INS_SS_NONE,
		{1, 0xe0 | 0x06},
		{
			INS_AM_I | INS_OT_b,
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", INS_SS_NONE,
		{1, 0xe0 | 0x07},
		{
			INS_AM_I | INS_OT_b,
			INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0f, 0x00~0x07
	{
		"LOCK", INS_SS_PREFIX,
		{1, 0xf0 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT1", INS_SS_NONE,
		{1, 0xf0 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REPNE", INS_SS_PREFIX,
		{1, 0xf0 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XACQUIRE", INS_SS_PREFIX,
		{1, 0xf0 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REP", INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REPE", INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XRELEASE", INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"HLT", INS_SS_NONE,
		{1, 0xf0 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMC", INS_SS_NONE,
		{1, 0xf0 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Unary Grp 3
	// TEST
	{
		"TEST", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_I | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"TEST", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_I | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// NOT
	{
		"NOT", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_E | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"NOT", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_E | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// NEG
	{
		"NEG", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_E | INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"NEG", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_E | INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// MUL
	{
		"MUL", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"MUL", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// IMUL
	{
		"IMUL", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"IMUL", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// DIV
	{
		"DIV", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"DIV", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// IDIV
	{
		"IDIV", INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"IDIV", INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		NULL
	}
};
