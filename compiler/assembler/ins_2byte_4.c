#include "ins.h"

#include "ins_enc.h"

Instruction ins_2byte_4[] = {
	// 0x0f | 0x08, 0x08~0x0f
	{
		"JS", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x08},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNS", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x09},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JP", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0a},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JPE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0a},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNP", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0b},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JPO", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0b},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JL", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0c},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNGE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0c},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNL", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0d},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JGE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0d},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JLE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0e},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNG", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0e},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNLE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0f},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JG", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x0f},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x09, 0x08~0x0f
	{
		"SETS", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x08},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNS", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x09},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETP", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0a},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETPE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0a},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNP", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0b},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETPO", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0b},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETL", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0c},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNGE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0c},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNL", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0d},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETGE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0d},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETLE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0e},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNG", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0e},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNLE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0f},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETG", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x0f},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x0a, 0x08~0x0f
	{
		"PUSH", INS_SS_d64,
		{2, 0x0f, 0xa0 | 0x08},
		{
			INS_AM_GS
		},
		ins_enc_not_implemented
	},

	{
		"POP", INS_SS_d64,
		{2, 0x0f, 0xa0 | 0x09},
		{
			INS_AM_GS
		},
		ins_enc_not_implemented
	},

	{
		"RSM", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x0a},
		{
		},
		ins_enc_not_implemented
	},

	{
		"BTS", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x0b},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"SHRD", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x0c},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SHRD", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x0d},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v,
			INS_AM_CL
		},
		ins_enc_not_implemented
	},

	{
		"FXSAVE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x0
		}
	},
	{
		"FXRSTOR", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x1
		}
	},
	{
		"LDMXCSR", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x2
		}
	},
	{
		"STMXCSR", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x3
		}
	},
	{
		"XSAVE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x4
		}
	},
	{
		"XRSTOR", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x5
		}
	},
	{
		"XSAVEOPT", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x6
		}
	},
	{
		"CLFLUSH", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x7
		}
	},

	{
		"LFENCE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"MFENCE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"SFENCE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"RDFSBASE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
			INS_AM_R | INS_OT_y
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_PFX,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.pfx = 0xf3
		}
	},
	{
		"RDGSBASE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
			INS_AM_R | INS_OT_y
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_PFX,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.pfx = 0xf3
		}
	},
	{
		"WRFSBASE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
			INS_AM_R | INS_OT_y
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_PFX,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.pfx = 0xf3
		}
	},
	{
		"WRGSBASE", INS_SS_1A | INS_SS_1C,
		{2, 0x0f, 0xa0 | 0x0e},
		{
			INS_AM_R | INS_OT_y
		},
		ins_enc_not_implemented,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_PFX,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3,
			.pfx = 0xf3
		}
	},

	{
		"IMUL", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x0f},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x0b, 0x08~0x0f
























};
