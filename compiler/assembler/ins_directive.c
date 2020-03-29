#include "ins.h"
#include "ins_directive_enc.h"

Instruction ins_directive[] = {
	{
		"BIT16", INS_SS_DIRECTIVE,
		{0},
		{
		},
		ins_enc_bit16
	},
	{
		"BIT32", INS_SS_DIRECTIVE,
		{0},
		{
		},
		ins_enc_bit32
	},
	{
		"BIT64", INS_SS_DIRECTIVE,
		{0},
		{
		},
		ins_enc_bit64
	},

	{
		"INT8", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int8
	},
	{
		"INT16", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int16
	},
	{
		"INT32", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int32
	},
	{
		"INT64", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int64
	},
	{
		"FLOAT", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_REAL
		},
		ins_enc_float
	},
	{
		"DOUBLE", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_REAL
		},
		ins_enc_double
	},
	{
		"STRING", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_STRING
		},
		ins_enc_string
	},

	{
		"INT8S", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int8s
	},
	{
		"INT16S", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int16s
	},
	{
		"INT32S", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int32s
	},
	{
		"INT64S", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_int64s
	},
	{
		"FLOATS", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_REAL
		},
		ins_enc_floats
	},
	{
		"DOUBLES", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_REAL
		},
		ins_enc_doubles
	},
	{
		"STRINGS", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_INT,
			INS_DIRECTIVE_OPRD_STRING
		},
		ins_enc_strings
	},

	{
		"LABEL", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_ID
		},
		ins_enc_label
	},
	{
		"SET", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_ID,
			INS_DIRECTIVE_OPRD_INT
		},
		ins_enc_set
	},
	{
		"PRINT", INS_SS_DIRECTIVE,
		{0},
		{
			INS_DIRECTIVE_OPRD_EXPR
		},
		ins_enc_print
	},


	{
		NULL
	}
};
