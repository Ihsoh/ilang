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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
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
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// Grp 7
	




	// 0x0f | 0x01, 0x00~0x07




	{
		NULL
	}
};
