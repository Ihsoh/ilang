#include "ins.h"

#include "ins_enc.h"

Instruction ins_2byte_3[] = {
	// 0x0f | 0x08, 0x00~0x07
	{
		"JO", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x00},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNO", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x01},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JB", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x02},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JC", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x02},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNAE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x02},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JAE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x03},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNB", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x03},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNC", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x03},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x04},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JZ", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x04},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JNE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x05},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNZ", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x05},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JBE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x06},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNA", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x06},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	{
		"JA", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x07},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},
	{
		"JNBE", INS_SS_NONE,
		{2, 0x0f, 0x80 | 0x07},
		{
			INS_AM_J | INS_OT_z
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x09, 0x00~0x07
	{
		"SETO", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x00},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNO", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x01},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},


	{
		"SETB", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x02},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETC", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x02},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNAE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x02},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETAE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x03},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNB", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x03},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNC", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x03},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x04},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETZ", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x04},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETNE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x05},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNZ", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x05},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETBE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x06},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNA", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x06},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SETA", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x07},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},
	{
		"SETNBE", INS_SS_NONE,
		{2, 0x0f, 0x90 | 0x07},
		{
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x0a, 0x00~0x07
	{
		"PUSH", INS_SS_d64,
		{2, 0x0f, 0xa0 | 0x00},
		{
			INS_AM_FS
		},
		ins_enc_not_implemented
	},

	{
		"POP", INS_SS_d64,
		{2, 0x0f, 0xa0 | 0x01},
		{
			INS_AM_FS
		},
		ins_enc_not_implemented
	},

	{
		"CPUID", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x02},
		{
		},
		ins_enc_not_implemented
	},

	{
		"BT", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"SHLD", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x04},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v,
			INS_AM_I | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"SHLD", INS_SS_NONE,
		{2, 0x0f, 0xa0 | 0x05},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v,
			INS_AM_CL
		},
		ins_enc_not_implemented
	},

	// 0x0f | 0x0b, 0x00~0x07
	{
		"CMPXCHG", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x00},
		{
			INS_AM_E | INS_OT_b,
			INS_AM_G | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"CMPXCHG", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x01},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"LSS", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x02},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_M | INS_OT_p
		},
		ins_enc_not_implemented
	},

	{
		"BTR", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x03},
		{
			INS_AM_E | INS_OT_v,
			INS_AM_G | INS_OT_v
		},
		ins_enc_not_implemented
	},

	{
		"LFS", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x04},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_M | INS_OT_p
		},
		ins_enc_not_implemented
	},

	{
		"LGS", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x05},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_M | INS_OT_p
		},
		ins_enc_not_implemented
	},

	{
		"MOVZX", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x06},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_b
		},
		ins_enc_not_implemented
	},

	{
		"MOVZX", INS_SS_NONE,
		{2, 0x0f, 0xb0 | 0x07},
		{
			INS_AM_G | INS_OT_v,
			INS_AM_E | INS_OT_w
		},
		ins_enc_not_implemented
	},




	// Table A-3. Two-byte Opcode Map: 80H — F7H (First Byte is 0FH) *
	// 0x0f | 0x0c, 0x00~0x07
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 80H — F7H (First Byte is 0FH) *
	// 0x0f | 0x0d, 0x00~0x07
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 80H — F7H (First Byte is 0FH) *
	// 0x0f | 0x0e, 0x00~0x07
	// TODO: 暂不实现。




	// Table A-3. Two-byte Opcode Map: 80H — F7H (First Byte is 0FH) *
	// 0x0f | 0x0f, 0x00~0x07
	// TODO: 暂不实现。




	{
		NULL
	}
};
