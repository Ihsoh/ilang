#include "ins.h"

Instruction ins_1byte_1[] = {
	// 0x00, 0x00~0x07
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x02},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x04},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADD", _INS_SS_NONE,
		{1, 0x00 | 0x05},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_i64,
		{1, 0x00 | 0x06},
		{
			_INS_AM_ES
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_i64,
		{1, 0x00 | 0x07},
		{
			_INS_AM_ES
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x01, 0x00~0x07
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x02},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x04},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADC", _INS_SS_NONE,
		{1, 0x10 | 0x05},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_i64,
		{1, 0x10 | 0x06},
		{
			_INS_AM_SS
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POP", _INS_SS_i64,
		{1, 0x10 | 0x07},
		{
			_INS_AM_SS
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x02, 0x00~0x07
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x02},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x04},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AND", _INS_SS_NONE,
		{1, 0x20 | 0x05},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ES", _INS_SS_PREFIX,
		{1, 0x20 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"DAA", _INS_SS_i64,
		{1, 0x20 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x03, 0x00~0x07
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x02},
		{
			_INS_AM_G | _INS_OT_b,
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x04},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XOR", _INS_SS_NONE,
		{1, 0x30 | 0x05},
		{
			_INS_AM_rAX,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"SS", _INS_SS_PREFIX,
		{1, 0x30 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAA", _INS_SS_i64,
		{1, 0x30 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x04, 0x00~0x07
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x00},
		{
			_INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x01},
		{
			_INS_AM_eCX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x02},
		{
			_INS_AM_eDX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x03},
		{
			_INS_AM_eBX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x04},
		{
			_INS_AM_eSP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x05},
		{
			_INS_AM_eBP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x06},
		{
			_INS_AM_eSI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INC", _INS_SS_i64,
		{1, 0x40 | 0x07},
		{
			_INS_AM_eDI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX", _INS_SS_PREFIX,
		{1, 0x40 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.B", _INS_SS_PREFIX,
		{1, 0x40 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.X", _INS_SS_PREFIX,
		{1, 0x40 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.XB", _INS_SS_PREFIX,
		{1, 0x40 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.R", _INS_SS_PREFIX,
		{1, 0x40 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RB", _INS_SS_PREFIX,
		{1, 0x40 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RX", _INS_SS_PREFIX,
		{1, 0x40 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REX.RXB", _INS_SS_PREFIX,
		{1, 0x40 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x05, 0x00~0x07
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x00},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x01},
		{
			_INS_AM_rCX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x02},
		{
			_INS_AM_rDX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x03},
		{
			_INS_AM_rBX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x04},
		{
			_INS_AM_rSP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x05},
		{
			_INS_AM_rBP
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x06},
		{
			_INS_AM_rSI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x07},
		{
			_INS_AM_rDI
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x00},
		{
			_INS_AM_r8
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x01},
		{
			_INS_AM_r9
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x02},
		{
			_INS_AM_r10
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x03},
		{
			_INS_AM_r11
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x04},
		{
			_INS_AM_r12
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x05},
		{
			_INS_AM_r13
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x06},
		{
			_INS_AM_r14
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSH", _INS_SS_d64,
		{1, 0x50 | 0x07},
		{
			_INS_AM_r15
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x06, 0x00~0x07
	{
		"PUSHA", _INS_SS_i64,
		{1, 0x60 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPA", _INS_SS_i64,
		{1, 0x60 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"BOUND", _INS_SS_i64,
		{1, 0x60 | 0x02},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_M | _INS_OT_a
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ARPL", _INS_SS_i64,
		{1, 0x60 | 0x03},
		{
			_INS_AM_E | _INS_OT_w,
			_INS_AM_G | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"FS", _INS_SS_PREFIX,
		{1, 0x60 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"GS", _INS_SS_PREFIX,
		{1, 0x60 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OPRD_SIZE", _INS_SS_PREFIX,
		{1, 0x60 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"ADDR_SIZE", _INS_SS_PREFIX,
		{1, 0x60 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PUSHAD", _INS_SS_i64,
		{1, 0x60 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"POPAD", _INS_SS_i64,
		{1, 0x60 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOVSXD", _INS_SS_o64,
		{1, 0x60 | 0x03},
		{
			_INS_AM_G | _INS_OT_v,
			_INS_AM_E | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x07, 0x00~0x07
	{
		"JO", _INS_SS_f64,
		{1, 0x70 | 0x00},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNO", _INS_SS_f64,
		{1, 0x70 | 0x01},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JB", _INS_SS_f64,	// B/NAE/C
		{1, 0x70 | 0x02},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNB", _INS_SS_f64,	// NB/AE/NC
		{1, 0x70 | 0x03},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JZ", _INS_SS_f64,	// Z/E
		{1, 0x70 | 0x04},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNZ", _INS_SS_f64,	// NZ/NE
		{1, 0x70 | 0x05},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JBE", _INS_SS_f64,	// BE/NA
		{1, 0x70 | 0x06},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JNBE", _INS_SS_f64,	// NBE/A
		{1, 0x70 | 0x07},
		{
			_INS_AM_LABEL
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x08, 0x00~0x07
	// Group1-ADD
	{
		"ADD", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ADD", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// Group1-OR
	{
		"OR", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"OR", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// Group1-ADC
	{
		"ADC", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"ADC", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// Group1-SBB
	{
		"SBB", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"SBB", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// Group1-AND
	{
		"AND", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"AND", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// Group1-SUB
	{
		"SUB", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SUB", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// Group1-XOR
	{
		"XOR", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"XOR", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// Group1-CMP
	{
		"CMP", _INS_SS_1A,
		{1, 0x80 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", _INS_SS_1A,
		{1, 0x80 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", _INS_SS_1A | _INS_SS_i64,
		{1, 0x80 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"CMP", _INS_SS_1A,
		{1, 0x80 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"TEST", _INS_SS_NONE,
		{1, 0x80 | 0x04},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"TEST", _INS_SS_NONE,
		{1, 0x80 | 0x05},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x80 | 0x06},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_G | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x80 | 0x07},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_G | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},

	// 0x09, 0x00~0x07
	{
		"NOP", _INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"PAUSE", _INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x00},
		{
			_INS_AM_r8,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			_INS_AM_rCX,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x01},
		{
			_INS_AM_r9,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			_INS_AM_rDX,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x02},
		{
			_INS_AM_r10,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			_INS_AM_rBX,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x03},
		{
			_INS_AM_r11,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			_INS_AM_rSP,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x04},
		{
			_INS_AM_r12,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			_INS_AM_rBP,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x05},
		{
			_INS_AM_r13,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			_INS_AM_rSI,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x06},
		{
			_INS_AM_r14,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			_INS_AM_rDI,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},
	{
		"XCHG", _INS_SS_NONE,
		{1, 0x90 | 0x07},
		{
			_INS_AM_r15,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.oprd_reversible = true
	},

	// 0x0a, 0x00~0x07
	{
		"MOV", _INS_SS_NONE,
		{1, 0xa0 | 0x00},
		{
			_INS_AM_AL,
			_INS_AM_O | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xa0 | 0x01},
		{
			_INS_AM_rAX,
			_INS_AM_O | _INS_OT_v
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xa0 | 0x02},
		{
			_INS_AM_O | _INS_OT_b,
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xa0 | 0x03},
		{
			_INS_AM_O | _INS_OT_v,
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOVSB", _INS_SS_NONE,
		{1, 0xa0 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOVSW", _INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOVSD", _INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOVSQ", _INS_SS_NONE,
		{1, 0xa0 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMPSB", _INS_SS_NONE,
		{1, 0xa0 | 0x06},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMPSW", _INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMPSD", _INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMPSQ", _INS_SS_NONE,
		{1, 0xa0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0b, 0x00~0x07
	// MOV immediate byte into byte register
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x00},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x00},
		{
			_INS_AM_r8l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x01},
		{
			_INS_AM_CL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x01},
		{
			_INS_AM_r9l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x02},
		{
			_INS_AM_DL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x02},
		{
			_INS_AM_r10l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x03},
		{
			_INS_AM_BL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x03},
		{
			_INS_AM_r11l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x04},
		{
			_INS_AM_AH,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x04},
		{
			_INS_AM_r12l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x05},
		{
			_INS_AM_CH,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x05},
		{
			_INS_AM_r13l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x06},
		{
			_INS_AM_DH,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x06},
		{
			_INS_AM_r14l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x07},
		{
			_INS_AM_BH,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"MOV", _INS_SS_NONE,
		{1, 0xb0 | 0x07},
		{
			_INS_AM_r15l,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0c, 0x00~0x07
	// Shift Grp 2

	// ROL
	{
		"ROL", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// ROR
	{
		"ROR", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// RCL
	{
		"RCL", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// RCR
	{
		"RCR", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// SHL/SAL
	// SHL和SAL是同一条指令，指令码完全一样。
	{
		"SHL", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// SHR
	{
		"SHR", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// SAR
	{
		"SAR", _INS_SS_1A,
		{1, 0xc0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", _INS_SS_1A,
		{1, 0xc0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"RET", _INS_SS_f64,
		{1, 0xc0 | 0x02},
		{
			_INS_AM_NEAR,
			_INS_AM_I | _INS_OT_w
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"RET", _INS_SS_f64,
		{1, 0xc0 | 0x03},
		{
			_INS_AM_NEAR
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LES", _INS_SS_i64,
		{1, 0xc0 | 0x04},
		{
			_INS_AM_G | _INS_OT_z,
			_INS_AM_M | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LDS", _INS_SS_i64,
		{1, 0xc0 | 0x05},
		{
			_INS_AM_G | _INS_OT_z,
			_INS_AM_M | _INS_OT_p
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Grp 11 - MOV
	// MOV
	{
		"MOV", _INS_SS_1A,
		{1, 0xc0 | 0x06},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"MOV", _INS_SS_1A,
		{1, 0xc0 | 0x07},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// 0x0d, 0x00~0x07
	// Shift Grp 2
	// ROL
	{
		"ROL", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"ROL", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// ROR
	{
		"ROR", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},
	{
		"ROR", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x1
		}
	},

	// RCL
	{
		"RCL", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"RCL", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// RCR
	{
		"RCR", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"RCR", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// SHL/SAL
	{
		"SHL", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SHL", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"SAL", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// SHR
	{
		"SHR", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"SHR", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// SAR
	{
		"SAR", _INS_SS_1A,
		{1, 0xd0 | 0x00},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", _INS_SS_1A,
		{1, 0xd0 | 0x01},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CONST_1
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", _INS_SS_1A,
		{1, 0xd0 | 0x02},
		{
			_INS_AM_E | _INS_OT_b,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"SAR", _INS_SS_1A,
		{1, 0xd0 | 0x03},
		{
			_INS_AM_E | _INS_OT_v,
			_INS_AM_CL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		"AAM", _INS_SS_i64,
		{1, 0xd0 | 0x04},
		{
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"AAD", _INS_SS_i64,
		{1, 0xd0 | 0x05},
		{
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XLAT", _INS_SS_NONE,
		{1, 0xd0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XLATB", _INS_SS_NONE,
		{1, 0xd0 | 0x07},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0e, 0x00~0x07
	{
		"LOOPNE", _INS_SS_f64,
		{1, 0xe0 | 0x00},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPNZ", _INS_SS_f64,
		{1, 0xe0 | 0x00},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPE", _INS_SS_f64,
		{1, 0xe0 | 0x01},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOPZ", _INS_SS_f64,
		{1, 0xe0 | 0x01},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"LOOP", _INS_SS_f64,
		{1, 0xe0 | 0x02},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JCXZ", _INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JECXZ", _INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"JRCXZ", _INS_SS_NONE,
		{1, 0xe0 | 0x03},
		{
			_INS_AM_J | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", _INS_SS_NONE,
		{1, 0xe0 | 0x04},
		{
			_INS_AM_AL,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"IN", _INS_SS_NONE,
		{1, 0xe0 | 0x05},
		{
			_INS_AM_eAX,
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", _INS_SS_NONE,
		{1, 0xe0 | 0x06},
		{
			_INS_AM_I | _INS_OT_b,
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"OUT", _INS_SS_NONE,
		{1, 0xe0 | 0x07},
		{
			_INS_AM_I | _INS_OT_b,
			_INS_AM_eAX
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// 0x0f, 0x00~0x07
	{
		"LOCK", _INS_SS_PREFIX,
		{1, 0xf0 | 0x00},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"INT1", _INS_SS_NONE,
		{1, 0xf0 | 0x01},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REPNE", _INS_SS_PREFIX,
		{1, 0xf0 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XACQUIRE", _INS_SS_PREFIX,
		{1, 0xf0 | 0x02},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REP", _INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"REPE", _INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"XRELEASE", _INS_SS_PREFIX,
		{1, 0xf0 | 0x03},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"HLT", _INS_SS_NONE,
		{1, 0xf0 | 0x04},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},
	{
		"CMC", _INS_SS_NONE,
		{1, 0xf0 | 0x05},
		{
		},
		INS_ENC_NOT_IMPLEMENTED
	},

	// Unary Grp 3
	// TEST
	{
		"TEST", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_I | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},
	{
		"TEST", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_I | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x0
		}
	},

	// NOT
	{
		"NOT", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},
	{
		"NOT", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_E | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x2
		}
	},

	// NEG
	{
		"NEG", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_E | _INS_OT_b
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},
	{
		"NEG", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_E | _INS_OT_z
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x3
		}
	},

	// MUL
	{
		"MUL", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},
	{
		"MUL", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x4
		}
	},

	// IMUL
	{
		"IMUL", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},
	{
		"IMUL", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x5
		}
	},

	// DIV
	{
		"DIV", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},
	{
		"DIV", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x6
		}
	},

	// IDIV
	{
		"IDIV", _INS_SS_NONE,
		{1, 0xf0 | 0x06},
		{
			_INS_AM_AL
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},
	{
		"IDIV", _INS_SS_NONE,
		{1, 0xf0 | 0x07},
		{
			_INS_AM_rAX
		},
		INS_ENC_NOT_IMPLEMENTED,
		.opcode_ext = {
			.mask = _INS_OPCODE_EXT_MASK_REG,
			.mod = _INS_OPCODE_EXT_MOD_MEM | _INS_OPCODE_EXT_MOD_11B,
			.reg = 0x7
		}
	},

	{
		NULL
	}
};
