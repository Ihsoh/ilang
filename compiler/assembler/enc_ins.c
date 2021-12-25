#include "enc_ins.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

size_t enc_ins_encode(
	EncoderInstruction *ins,
	uint8_t *buffer,
	size_t buffer_size
) {
	assert(ins);
	assert(buffer);
	assert(buffer_size > 0);

	size_t buffer_index = 0;

	#define	_BYTE(v, i)	((uint8_t)((v) >> (i * 8)))
	#define	_OUT_BYTE(v) {	\
		assert(buffer_index + 1 <= buffer_size);	\
		buffer[buffer_index++] = v;	\
	}
	#define	_OUT_2BYTE(v) {	\
		_OUT_BYTE(_BYTE(v, 0))	\
		_OUT_BYTE(_BYTE(v, 1))	\
	}
	#define	_OUT_4BYTE(v) {	\
		_OUT_BYTE(_BYTE(v, 0))	\
		_OUT_BYTE(_BYTE(v, 1))	\
		_OUT_BYTE(_BYTE(v, 2))	\
		_OUT_BYTE(_BYTE(v, 3))	\
	}
	#define	_OUT_8BYTE(v) {	\
		_OUT_BYTE(_BYTE(v, 0))	\
		_OUT_BYTE(_BYTE(v, 1))	\
		_OUT_BYTE(_BYTE(v, 2))	\
		_OUT_BYTE(_BYTE(v, 3))	\
		_OUT_BYTE(_BYTE(v, 4))	\
		_OUT_BYTE(_BYTE(v, 5))	\
		_OUT_BYTE(_BYTE(v, 6))	\
		_OUT_BYTE(_BYTE(v, 7))	\
	}
	
	// Legacy Prefix
	// ==================================================
	// Group 1
	// ==================================================
	// LOCK, 0xF0
	if (ins->legacy_prefix.lock) {
		_OUT_BYTE(0xF0)
	}

	// REPNE/REPNZ, 0xF2
	if (ins->legacy_prefix.repne) {
		_OUT_BYTE(0xF2)
	}

	// REP/REPE/REPZ, 0xF3
	if (ins->legacy_prefix.rep) {
		_OUT_BYTE(0xF3)
	}

	// ==================================================
	// Group 2
	// ==================================================
	// CS, 0x2E
	if (ins->legacy_prefix.cs) {
		_OUT_BYTE(0x2E)
	}

	// SS, 0x36
	if (ins->legacy_prefix.ss) {
		_OUT_BYTE(0x36)
	}

	// DS, 0x3E
	if (ins->legacy_prefix.ds) {
		_OUT_BYTE(0x3E)
	}

	// ES, 0x26
	if (ins->legacy_prefix.es) {
		_OUT_BYTE(0x26)
	}

	// FS, 0x64
	if (ins->legacy_prefix.fs) {
		_OUT_BYTE(0x64)
	}

	// GS, 0x65
	if (ins->legacy_prefix.gs) {
		_OUT_BYTE(0x65)
	}

	// Branch not taken, 0x2E
	if (ins->legacy_prefix.branch_not_taken) {
		_OUT_BYTE(0x2E)
	}

	// Branch taken, 0x3E
	if (ins->legacy_prefix.branch_taken) {
		_OUT_BYTE(0x3E)
	}

	// ==================================================
	// Group 3
	// ==================================================
	// Operand-size override, 0x66
	if (ins->legacy_prefix.operand_size_override) {
		_OUT_BYTE(0x66)
	}

	// ==================================================
	// Group 4
	// ==================================================
	// Address-size override, 0x67
	if (ins->legacy_prefix.address_size_override) {
		_OUT_BYTE(0x67)
	}

	// REX Prefix
	if (ins->rex_prefix_used) {
		uint8_t rex_prefix = 0x40;

		if (ins->rex_prefix.b) {
			rex_prefix |= 0x01;
		}

		if (ins->rex_prefix.x) {
			rex_prefix |= 0x02;
		}

		if (ins->rex_prefix.r) {
			rex_prefix |= 0x04;
		}

		if (ins->rex_prefix.w) {
			rex_prefix |= 0x08;
		}

		_OUT_BYTE(rex_prefix)
	}

	// Opcode
	switch (ins->opcode_len) {
		case 1: {
			_OUT_BYTE(ins->opcode[0])
			break;
		}
		case 2: {
			_OUT_BYTE(ins->opcode[0])
			_OUT_BYTE(ins->opcode[1])
			break;
		}
		case 3: {
			_OUT_BYTE(ins->opcode[0])
			_OUT_BYTE(ins->opcode[1])
			_OUT_BYTE(ins->opcode[2])
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	// ModR/M
	if (ins->mod_rm_used) {
		uint8_t mod_rm = 0x00;

		// Mod
		mod_rm |= ((ins->mod_rm.mod & 0x03) << 6);

		// Reg
		mod_rm |= ((ins->mod_rm.reg & 0x07) << 3);

		// R/M
		mod_rm |= ins->mod_rm.rm & 0x07;

		_OUT_BYTE(mod_rm)
	}

	// SIB
	if (ins->sib_used) {
		uint8_t sib = 0x00;

		// Scale
		sib |= ((ins->sib.scale & 0x03) << 6);

		// Index
		sib |= ((ins->sib.index & 0x07) << 3);
		
		// Base
		sib |= ins->sib.base & 0x07;

		_OUT_BYTE(sib)
	}

	// Displacement
	switch (ins->disp_len) {
		case 0: {
			break;
		}
		case 1: {
			_OUT_BYTE(ins->disp);
			break;
		}
		case 2: {
			_OUT_2BYTE(ins->disp);
			break;
		}
		case 4: {
			_OUT_4BYTE(ins->disp);
			break;
		}
		case 8: {
			_OUT_8BYTE(ins->disp);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	// Immediate
	switch (ins->imm_len) {
		case 0: {
			break;
		}
		case 1: {
			_OUT_BYTE(ins->imm);
			break;
		}
		case 2: {
			_OUT_2BYTE(ins->imm);
			break;
		}
		case 4: {
			_OUT_4BYTE(ins->imm);
			break;
		}
		case 8: {
			_OUT_8BYTE(ins->imm);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	// 扩展字段
	if (ins->ext_len > ENCODER_INSTRUCTION_EXT_MAX_LEN) {
		assert(0);
	}
	for (int i = 0; i < ins->ext_len; i++) {
		_OUT_BYTE(ins->ext[i]);
	}

	#undef	_BYTE
	#undef	_OUT_BYTE
	#undef	_OUT_2BYTE
	#undef	_OUT_4BYTE
	#undef	_OUT_8BYTE

	return buffer_index;
}
