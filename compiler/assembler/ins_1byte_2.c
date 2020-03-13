#include "ins.h"

Instruction ins_1byte_2[] = {
	// 0x00, 0x08~0x0f
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OR", _INS_SS_NONE,
		{1, 0x00 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_i64,
		{1, 0x00 | 0x0e},
		{
			_INS_AM_CS
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x01, 0x08~0x0f
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x10 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_i64,
		{1, 0x10 | 0x0e},
		{
			_INS_AM_DS
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_i64,
		{1, 0x10 | 0x0f},
		{
			_INS_AM_DS
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x02, 0x08~0x0f
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SUB", _INS_SS_NONE,
		{1, 0x20 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CS", _INS_SS_PREFIX,
		{1, 0x20 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DAS", _INS_SS_i64,
		{1, 0x20 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x03, 0x08~0x0f
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMP", _INS_SS_NONE,
		{1, 0x30 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DS", _INS_SS_PREFIX,
		{1, 0x30 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAS", _INS_SS_i64,
		{1, 0x30 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x04, 0x08~0x0f
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x08},
		{
			_INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x09},
		{
			_INS_AM_eCX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0a},
		{
			_INS_AM_eDX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0b},
		{
			_INS_AM_eBX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0c},
		{
			_INS_AM_eSP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0d},
		{
			_INS_AM_eBP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0e},
		{
			_INS_AM_eSI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DEC", _INS_SS_i64,
		{1, 0x40 | 0x0f},
		{
			_INS_AM_eDI
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	{
		"REX.W", _INS_SS_PREFIX,
		{1, 0x40 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WB", _INS_SS_PREFIX,
		{1, 0x40 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WX", _INS_SS_PREFIX,
		{1, 0x40 | 0x0a},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WXB", _INS_SS_PREFIX,
		{1, 0x40 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WR", _INS_SS_PREFIX,
		{1, 0x40 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRB", _INS_SS_PREFIX,
		{1, 0x40 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRX", _INS_SS_PREFIX,
		{1, 0x40 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.WRXB", _INS_SS_PREFIX,
		{1, 0x40 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x05, 0x08~0x0f
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x08},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x09},
		{
			_INS_AM_rCX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0a},
		{
			_INS_AM_rDX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0b},
		{
			_INS_AM_rBX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0c},
		{
			_INS_AM_rSP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0d},
		{
			_INS_AM_rBP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0e},
		{
			_INS_AM_rSI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0f},
		{
			_INS_AM_rDI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x08},
		{
			_INS_AM_r8
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x09},
		{
			_INS_AM_r9
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0a},
		{
			_INS_AM_r10
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0b},
		{
			_INS_AM_r11
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0c},
		{
			_INS_AM_r12
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0d},
		{
			_INS_AM_r13
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0e},
		{
			_INS_AM_r14
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_d64,
		{1, 0x50 | 0x0f},
		{
			_INS_AM_r15
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x06, 0x08~0x0f
	{
		"PUSH", _INS_SS_d64,
		{1, 0x60 | 0x08},
		{
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IMUL", _INS_SS_NONE,
		{1, 0x60 | 0x09},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x60 | 0x0a},
		{
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IMUL", _INS_SS_NONE,
		{1, 0x60 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INS", _INS_SS_NONE,
		{1, 0x60 | 0x0c},
		{
			_INS_AM_Y | _INS_OT_b,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INSB", _INS_SS_NONE,
		{1, 0x60 | 0x0c},
		{
			_INS_AM_Y | _INS_OT_b,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INS", _INS_SS_NONE,
		{1, 0x60 | 0x0d},
		{
			_INS_AM_Y | _INS_OT_z,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INSW", _INS_SS_NONE,
		{1, 0x60 | 0x0d},
		{
			_INS_AM_Y | _INS_OT_z,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INSD", _INS_SS_NONE,
		{1, 0x60 | 0x0d},
		{
			_INS_AM_Y | _INS_OT_z,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUTS", _INS_SS_NONE,
		{1, 0x60 | 0x0e},
		{
			_INS_AM_DX,
			_INS_AM_X | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUTSB", _INS_SS_NONE,
		{1, 0x60 | 0x0e},
		{
			_INS_AM_DX,
			_INS_AM_X | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUTS", _INS_SS_NONE,
		{1, 0x60 | 0x0f},
		{
			_INS_AM_DX,
			_INS_AM_X | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUTSW", _INS_SS_NONE,
		{1, 0x60 | 0x0f},
		{
			_INS_AM_DX,
			_INS_AM_X | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUTSD", _INS_SS_NONE,
		{1, 0x60 | 0x0f},
		{
			_INS_AM_DX,
			_INS_AM_X | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x07, 0x08~0x0f
	{
		"JS", _INS_SS_f64,
		{1, 0x70 | 0x08},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNS", _INS_SS_f64,
		{1, 0x70 | 0x09},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JP", _INS_SS_f64,	// P/PE
		{1, 0x70 | 0x0a},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNP", _INS_SS_f64,	// NP/PO
		{1, 0x70 | 0x0b},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JL", _INS_SS_f64,	// L/NGE
		{1, 0x70 | 0x0c},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNL", _INS_SS_f64,	// NL/GE
		{1, 0x70 | 0x0d},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JLE", _INS_SS_f64,	// LE/NG
		{1, 0x70 | 0x0e},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNLE", _INS_SS_f64,	// NLE/G
		{1, 0x70 | 0x0f},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x08, 0x08~0x0f
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x0c},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_S | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LEA", _INS_SS_NONE,
		{1, 0x80 | 0x0d},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_M
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0x80 | 0x0e},
		{
			_INS_AM_S | _INS_OT_w,
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Grp 1A
	// POP
	{
		"POP", _INS_SS_1A | _INS_SS_d64,
		{1, 0x80 | 0x0f},
		{
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// 0x09, 0x08~0x0f
	{
		"CBW", _INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CWDE", _INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CDQE", _INS_SS_NONE,
		{1, 0x90 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CWD", _INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CDQ", _INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CQO", _INS_SS_NONE,
		{1, 0x90 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CALL", _INS_SS_i64,
		{1, 0x90 | 0x0a},
		{
			_INS_AM_FAR,
			_INS_AM_A | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"FWAIT", _INS_SS_NONE,
		{1, 0x90 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"WAIT", _INS_SS_NONE,
		{1, 0x90 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHF", _INS_SS_NONE,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHFD", _INS_SS_NONE,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHFQ", _INS_SS_d64,
		{1, 0x90 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPF", _INS_SS_NONE,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPFD", _INS_SS_NONE,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPFQ", _INS_SS_d64,
		{1, 0x90 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SAHF", _INS_SS_NONE,
		{1, 0x90 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LAHF", _INS_SS_NONE,
		{1, 0x90 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0a, 0x08~0x0f
	{
		"TEST", _INS_SS_NONE,
		{1, 0xa0 | 0x08},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"TEST", _INS_SS_NONE,
		{1, 0xa0 | 0x09},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOS", _INS_SS_NONE,
		{1, 0xa0 | 0x0a},
		{
			_INS_AM_Y | _INS_OT_b,
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSB", _INS_SS_NONE,
		{1, 0xa0 | 0x0a},
		{
			_INS_AM_Y | _INS_OT_b,
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOS", _INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			_INS_AM_Y | _INS_OT_v,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSW", _INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			_INS_AM_Y | _INS_OT_v,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSD", _INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			_INS_AM_Y | _INS_OT_v,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STOSQ", _INS_SS_NONE,
		{1, 0xa0 | 0x0b},
		{
			_INS_AM_Y | _INS_OT_v,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODS", _INS_SS_NONE,
		{1, 0xa0 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_X | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSB", _INS_SS_NONE,
		{1, 0xa0 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_X | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODS", _INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_X | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSW", _INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_X | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSD", _INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_X | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LODSQ", _INS_SS_NONE,
		{1, 0xa0 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_X | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCAS", _INS_SS_NONE,
		{1, 0xa0 | 0x0e},
		{
			_INS_AM_AL,
			_INS_AM_Y | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASB", _INS_SS_NONE,
		{1, 0xa0 | 0x0e},
		{
			_INS_AM_AL,
			_INS_AM_Y | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCAS", _INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			_INS_AM_rAX,
			_INS_AM_Y | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASW", _INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			_INS_AM_rAX,
			_INS_AM_Y | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASD", _INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			_INS_AM_rAX,
			_INS_AM_Y | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SCASQ", _INS_SS_NONE,
		{1, 0xa0 | 0x0f},
		{
			_INS_AM_rAX,
			_INS_AM_Y | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0b, 0x08~0x0f
	// MOV immediate word or double into word, double, or quad register
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x09},
		{
			_INS_AM_rCX,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			_INS_AM_rDX,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			_INS_AM_rBX,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
			_INS_AM_rSP,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			_INS_AM_rBP,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0e},
		{
			_INS_AM_rSI,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
			_INS_AM_rDI,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			_INS_AM_r8,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x09},
		{
			_INS_AM_r9,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			_INS_AM_r10,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			_INS_AM_r11,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
			_INS_AM_r12,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			_INS_AM_r13,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0e},
		{
			_INS_AM_r14,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
			_INS_AM_r15,
			_INS_AM_I | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0c, 0x08~0x0f
	{
		"ENTER", _INS_SS_NONE,
		{1, 0xb0 | 0x08},
		{
			_INS_AM_I | _INS_OT_w,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LEAVE", _INS_SS_d64,
		{1, 0xb0 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", _INS_SS_NONE,
		{1, 0xb0 | 0x0a},
		{
			_INS_AM_FAR,
			_INS_AM_I | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", _INS_SS_NONE,
		{1, 0xb0 | 0x0b},
		{
			_INS_AM_FAR
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT3", _INS_SS_NONE,
		{1, 0xb0 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT", _INS_SS_NONE,
		{1, 0xb0 | 0x0d},
		{
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INTO", _INS_SS_i64,
		{1, 0xb0 | 0x0e},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRET", _INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRETD", _INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IRETQ", _INS_SS_NONE,
		{1, 0xb0 | 0x0f},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0d, 0x08~0x0f
	// ESC (Escape to coprocessor instruction set)

	// 0x0e, 0x08~0x0f
	{
		"CALL", _INS_SS_f64,
		{1, 0xe0 | 0x08},
		{
			_INS_AM_NEAR,
			_INS_AM_J | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", _INS_SS_f64,
		{1, 0xe0 | 0x09},
		{
			_INS_AM_NEAR,
			_INS_AM_J | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", _INS_SS_i64,
		{1, 0xe0 | 0x0a},
		{
			_INS_AM_FAR,
			_INS_AM_A | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JMP", _INS_SS_f64,
		{1, 0xe0 | 0x0b},
		{
			_INS_AM_SHORT,
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", _INS_SS_NONE,
		{1, 0xe0 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", _INS_SS_NONE,
		{1, 0xe0 | 0x0d},
		{
			_INS_AM_eAX,
			_INS_AM_DX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", _INS_SS_NONE,
		{1, 0xe0 | 0x0e},
		{
			_INS_AM_DX,
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", _INS_SS_NONE,
		{1, 0xe0 | 0x0f},
		{
			_INS_AM_DX,
			_INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0f, 0x08~0x0f
	{
		"CLC", _INS_SS_NONE,
		{1, 0xf0 | 0x08},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STC", _INS_SS_NONE,
		{1, 0xf0 | 0x09},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CLI", _INS_SS_NONE,
		{1, 0xf0 | 0x0a},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STI", _INS_SS_NONE,
		{1, 0xf0 | 0x0b},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CLD", _INS_SS_NONE,
		{1, 0xf0 | 0x0c},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"STD", _INS_SS_NONE,
		{1, 0xf0 | 0x0d},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	
	// INC/DEC Grp 4
	{
		"INC", _INS_SS_1A,
		{1, 0xf0 | 0x0e},
		{
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"DEC", _INS_SS_1A,
		{1, 0xf0 | 0x0e},
		{
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// INC/DEC Grp 5
	{
		"INC", _INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"DEC", _INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"CALL", _INS_SS_1A | _INS_SS_f64,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_NEAR,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"CALL", _INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_FAR,
			_INS_AM_E | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"JMP", _INS_SS_1A | _INS_SS_f64,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_NEAR,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"JMP", _INS_SS_1A,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_FAR,
			_INS_AM_M | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"PUSH", _INS_SS_1A | _INS_SS_d64,
		{1, 0xf0 | 0x0f},
		{
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},




	{
		NULL
	}
};
