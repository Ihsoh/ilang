#include "ins.h"

#include "ins_enc.h"

Instruction ins_2byte_1[] = {
	// 0x0f | 0x00, 0x00~0x07
	// Grp 6
	{
		"SLDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_R | INS_OT_v
		},
		ins_enc_opcode_ext_XXX_Rv,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"SLDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_M | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Mw,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"STR", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_R | INS_OT_v
		},
		ins_enc_opcode_ext_XXX_Rv,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"STR", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_M | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Mw,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"LLDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_E | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Ew,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"LTR", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_E | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Ew,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"VERR", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_E | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Ew,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"VERW", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			INS_AM_E | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Ew,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// Grp 7
	// reg=0x0
	{
		"SGDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_s
		},
		ins_enc_opcode_ext_XXX_Ms,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x0
		}
	},
	{
		"VMCALL", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x1
		}
	},
	{
		"VMLAUNCH", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x2
		}
	},
	{
		"VMRESUME", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x3
		}
	},
	{
		"VMXOFF", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x4
		}
	},
	
	// reg=0x1
	{
		"SIDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_s
		},
		ins_enc_opcode_ext_XXX_Ms,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x1
		}
	},
	{
		"MONITOR", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x0
		}
	},
	{
		"MWAIT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x1
		}
	},
	{
		"CLAC", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x2
		}
	},
	{
		"STAC", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x3
		}
	},
	{
		"ENCLS", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x7
		}
	},

	// reg=0x2
	{
		"LGDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_s
		},
		ins_enc_opcode_ext_XXX_Ms,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x2
		}
	},
	{
		"XGETBV", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x0
		}
	},
	{
		"XSETBV", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x1
		}
	},
	{
		"VMFUNC", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x4
		}
	},
	{
		"XEND", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x5
		}
	},
	{
		"XTEST", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x6
		}
	},
	{
		"ENCLU", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x7
		}
	},

	// reg=0x3
	{
		"LIDT", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_s
		},
		ins_enc_opcode_ext_XXX_Ms,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x3
		}
	},

	// reg=0x4
	{
		"SMSW", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Mw,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SMSW", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_R | INS_OT_v
		},
		ins_enc_opcode_ext_XXX_Rv,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// reg=0x6
	{
		"LMSW", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_E | INS_OT_w
		},
		ins_enc_opcode_ext_XXX_Ew,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM | INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// reg=0x7
	{
		"INVLPG", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			INS_AM_M | INS_OT_b
		},
		ins_enc_opcode_ext_XXX_Mb,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG,
			.mod = INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x7
		}
	},
	{
		"SWAPGS", INS_SS_1A | INS_SS_o64,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7,
			.rm = 0x0
		}
	},
	{
		"RDTSCP", INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		ins_enc_opcode_ext_rm,
		.opcode_ext = {
			.mask = INS_OPCODE_EXT_MASK_MOD | INS_OPCODE_EXT_MASK_REG | INS_OPCODE_EXT_MASK_RM,
			.mod = INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7,
			.rm = 0x1
		}
	},

	{
		"LAR", INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x02},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_w
		},
		ins_enc_XXX_Gv_Ew
	},
	{
		"LSL", INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x03},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_w
		},
		ins_enc_XXX_Gv_Ew
	},
	{
		"SYSCALL", INS_SS_o64,
		{2, 0x0f, 0x00 | 0x05},
		{
		},
		ins_enc_opcode
	},
	{
		"CLTS", INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x06},
		{
		},
		ins_enc_opcode
	},
	{
		"SYSRET", INS_SS_o64,
		{2, 0x0f, 0x00 | 0x07},
		{
		},
		ins_enc_opcode
	},






	




	// 0x0f | 0x01, 0x00~0x07




	{
		NULL
	}
};
