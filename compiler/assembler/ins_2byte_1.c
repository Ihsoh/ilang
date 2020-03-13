#include "ins.h"

Instruction ins_2byte_1[] = {
	// 0x0f | 0x00, 0x00~0x07
	// Grp 6
	{
		"SLDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_R | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"SLDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_M | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"STR", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_R | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"STR", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_M | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"LLDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"LTR", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"VERR", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"VERW", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x00},
		{
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// Grp 7
	// reg=0x0
	{
		"SGDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_s
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x0
		}
	},
	{
		"VMCALL", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x1
		}
	},
	{
		"VMLAUNCH", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x2
		}
	},
	{
		"VMRESUME", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x3
		}
	},
	{
		"VMXOFF", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0,
			.rm = 0x4
		}
	},
	
	// reg=0x1
	{
		"SIDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_s
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x1
		}
	},
	{
		"MONITOR", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x0
		}
	},
	{
		"MWAIT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x1
		}
	},
	{
		"CLAC", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x2
		}
	},
	{
		"STAC", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x3
		}
	},
	{
		"ENCLS", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1,
			.rm = 0x7
		}
	},

	// reg=0x2
	{
		"LGDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_s
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x2
		}
	},
	{
		"XGETBV", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x0
		}
	},
	{
		"XSETBV", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x1
		}
	},
	{
		"VMFUNC", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x4
		}
	},
	{
		"XEND", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x5
		}
	},
	{
		"XTEST", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x6
		}
	},
	{
		"ENCLU", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2,
			.rm = 0x7
		}
	},

	// reg=0x3
	{
		"LIDT", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_s
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x3
		}
	},

	// reg=0x4
	{
		"SMSW", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SMSW", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_R | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// reg=0x6
	{
		"LMSW", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// reg=0x7
	{
		"INVLPG", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
			_INS_AM_M | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM,
			.reg = 0x7
		}
	},
	{
		"SWAPGS", _INS_SS_1A | _INS_SS_o64,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7,
			.rm = 0x0
		}
	},
	{
		"RDTSCP", _INS_SS_1A,
		{2, 0x0f, 0x00 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_MOD | _INS_OPCODE_EXT_MASK_REG | _INS_OPCODE_EXT_MASK_RM,
			.mod = _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7,
			.rm = 0x1
		}
	},

	{
		"LAR", _INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x02},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LSL", _INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SYSCALL", _INS_SS_o64,
		{2, 0x0f, 0x00 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CLTS", _INS_SS_NONE,
		{2, 0x0f, 0x00 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SYSRET", _INS_SS_o64,
		{2, 0x0f, 0x00 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},






	




	// 0x0f | 0x01, 0x00~0x07




	{
		NULL
	}
};
