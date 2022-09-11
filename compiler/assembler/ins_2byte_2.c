#include "ins.h"

#include "ins_enc.h"

Instruction ins_2byte_2[] = {
	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x00, 0x08~0x0f
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x01, 0x08~0x0f
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x02, 0x08~0x0f
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x03, 0x08~0x0f
	// TODO: 暂不实现。




	// 0x0f | 0x04, 0x08~0x0f
	{
		"CMOVS", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x08},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVNS", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x09},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVP", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0a},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVPE", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0a},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVNP", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVPO", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0b},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVL", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0c},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVNGE", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0c},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVNL", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0d},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVGE", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0d},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVLE", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0e},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVNG", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0e},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"CMOVNLE", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0f},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},
	{
		"CMOVG", INS_SS_NONE,
		{2, 0x0f, 0x40 | 0x0f},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_v
		},
		ins_enc_not_implemented
	},




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x05, 0x08~0x0f
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x06, 0x08~0x0f
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 08H — 7FH (First Byte is 0FH) *
	// 0x0f | 0x07, 0x08~0x0f
	// TODO: 暂不实现。




	{
		NULL
	}
};