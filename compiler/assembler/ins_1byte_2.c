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
		{1, 0x01 | 0x08},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x01 | 0x09},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x01 | 0x0a},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x01 | 0x0b},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x01 | 0x0c},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SBB", _INS_SS_NONE,
		{1, 0x01 | 0x0d},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_i64,
		{1, 0x01 | 0x0e},
		{
			_INS_AM_DS
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_i64,
		{1, 0x01 | 0x0f},
		{
			_INS_AM_DS
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x02, 0x08~0x0f
	







	{
		NULL
	}
};
