#ifndef __COMPILER_ASSEMBLER_ENC_INS_H__
#define __COMPILER_ASSEMBLER_ENC_INS_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	struct {
		// Legacy Prefix总共分为4组，每组只能使用一项。
		// 也就是说一条指令最多可以有4个Legacy Prefix，最大4个字节。

		// ==================================================
		// Group 1
		// ==================================================
		// LOCK, 0xF0
		bool lock;
		
		// REPNE/REPNZ, 0xF2
		bool repne;

		// REP/REPE/REPZ, 0xF3
		bool rep;

		// ==================================================
		// Group 2
		// ==================================================
		// CS, 0x2E
		bool cs;

		// SS, 0x36
		bool ss;

		// DS, 0x3E
		bool ds;

		// ES, 0x26
		bool es;

		// FS, 0x64
		bool fs;

		// GS, 0x65
		bool gs;

		// Branch not taken, 0x2E
		bool branch_not_taken;

		// Branch taken, 0x3E
		bool branch_taken;

		// ==================================================
		// Group 3
		// ==================================================
		// Operand-size override, 0x66
		bool operand_size_override;

		// ==================================================
		// Group 4
		// ==================================================
		// Address-size override, 0x67
		bool address_size_override;
	} legacy_prefix;

	// rex_prefix_used字段指定指令是否包含REX Prefix。
	// 当rex_prefix_used=true时，rex_prefix字段内各项有意义。
	bool rex_prefix_used;
	struct {
		// REX Prefix用于访问64位时使用。
		
		// 0 = Operand size根据CS.D确定。
		// 1 = 使用64位的Operand size
		bool w;

		// 扩展ModR/M的reg字段。
		bool r;

		// 扩展SIB的index字段。
		bool x;

		// 扩展ModR/M的r/m字段、SIB的base字段，Opcode的reg字段。
		bool b;
	} rex_prefix;

	// opcode字段储存Opcode，Opcode的长度由opcode_len字段指定。
	// Opcode的最小长度为1字节，最大长度为3字节。
	int8_t opcode_len;
	uint8_t opcode[3];

	// mod_rm_used字段指定指令是否包含ModR/M。
	// 当mod_rm_used=true时，mod_rm字段内各项有效。
	bool mod_rm_used;
	struct {
		uint8_t mod;

		uint8_t reg;

		uint8_t rm;
	} mod_rm;

	// sib_used字段指定指令是否包含SIB。
	// 当sib_used=true时，sib字段内各项有效。
	bool sib_used;
	struct {
		uint8_t scale;

		uint8_t index;

		uint8_t base;
	} sib;

	// disp_len字段指定Displacement的长度。当disp_len字段为0时，
	// 表示指令不包含Displacement，disp字段无效。
	// disp_len字段取值范围为：0（无Displacement）、1（8位）、2（16位）、4（32位）、8（64位）。
	// disp字段包含Displacement的值。
	uint8_t disp_len;
	uint64_t disp;

	// imm_len字段指定Immediate的长度。当imm_len字段为0时，
	// 表示指令不包含Immediate，imm字段无效。
	// imm_len字段取值范围为：0（无Immediate）、1（8位）、2（16位）、4（32位）、8（64位）。
	// imm字段包含Immediate的值。
	uint8_t imm_len;
	uint64_t imm;
} EncoderInstruction;

#endif
