#ifndef __COMPILER_ASSEMBLER_INS_H__
#define __COMPILER_ASSEMBLER_INS_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#define	_INS_OPRD_NONE	0x0000

/*====================================================================================================
	Codes for Addressing Method
	
	0x00xx ~ 0x7fxx
====================================================================================================*/

/*
	Direct address: the instruction has no ModR/M byte; the address of the operand is encoded in the instruction.
	No base register, index register, or scaling factor can be applied (for example, far JMP (EA)).
*/
#define	_INS_AM_A	0x0100

/*
	The VEX.vvvv field of the VEX prefix selects a general purpose register.
*/
#define	_INS_AM_B	0x0200

/*
	The reg field of the ModR/M byte selects a control register (for example, MOV (0F20, 0F22)).
*/
#define	_INS_AM_C	0x0300

/*
	The reg field of the ModR/M byte selects a debug register (for example, MOV (0F21,0F23)).
*/
#define	_INS_AM_D	0x0400

/*
	A ModR/M byte follows the opcode and specifies the operand. The operand is either a general-purpose
	register or a memory address. If it is a memory address, the address is computed from a segment register
	and any of the following values: a base register, an index register, a scaling factor, a displacement.
*/
#define	_INS_AM_E	0x0500

/*
	EFLAGS/RFLAGS Register.
*/
#define	_INS_AM_F	0x0600

/*
	The reg field of the ModR/M byte selects a general register (for example, AX (000)).
*/
#define	_INS_AM_G	0x0700

/*
	The VEX.vvvv field of the VEX prefix selects a 128-bit XMM register or a 256-bit YMM register, determined
	by operand type. For legacy SSE encodings this operand does not exist, changing the instruction to
	destructive form.
*/
#define	_INS_AM_H	0x0800

/*
	Immediate data: the operand value is encoded in subsequent bytes of the instruction.
*/
#define	_INS_AM_I	0x0900

/*
	The instruction contains a relative offset to be added to the instruction pointer register (for example, JMP(0E9), LOOP).
*/
#define	_INS_AM_J	0x0a00

/*
	The upper 4 bits of the 8-bit immediate selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
	(the MSB is ignored in 32-bit mode)
*/
#define	_INS_AM_L	0x0b00

/*
	The ModR/M byte may refer only to memory (for example, BOUND, LES, LDS, LSS, LFS, LGS, CMPXCHG8B).
*/
#define	_INS_AM_M	0x0c00

/*
	The R/M field of the ModR/M byte selects a packed-quadword, MMX technology register.
*/
#define	_INS_AM_N	0x0d00

/*
	The instruction has no ModR/M byte. The offset of the operand is coded as a word or double word
	(depending on address size attribute) in the instruction. No base register, index register, or scaling factor
	can be applied (for example, MOV (A0–A3)).
*/
#define	_INS_AM_O	0x0e00

/*
	The reg field of the ModR/M byte selects a packed quadword MMX technology register.
*/
#define	_INS_AM_P	0x0f00

/*
	A ModR/M byte follows the opcode and specifies the operand. The operand is either an MMX technology
	register or a memory address. If it is a memory address, the address is computed from a segment register
	and any of the following values: a base register, an index register, a scaling factor, and a displacement.
*/
#define	_INS_AM_Q	0x1000

/*
	The R/M field of the ModR/M byte may refer only to a general register (for example, MOV (0F20-0F23)).
*/
#define	_INS_AM_R	0x1100

/*
	The reg field of the ModR/M byte selects a segment register (for example, MOV (8C,8E)).
*/
#define	_INS_AM_S	0x1200

/*
	The R/M field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
*/
#define	_INS_AM_U	0x1300

/*
	The reg field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
*/
#define	_INS_AM_V	0x1400

/*
	A ModR/M byte follows the opcode and specifies the operand. The operand is either a 128-bit XMM register,
	a 256-bit YMM register (determined by operand type), or a memory address. If it is a memory address, the
	address is computed from a segment register and any of the following values: a base register, an index
	register, a scaling factor, and a displacement.
*/
#define	_INS_AM_W	0x1500

/*
	Memory addressed by the DS:rSI register pair (for example, MOVS, CMPS, OUTS, or LODS).
*/
#define	_INS_AM_X	0x1600

/*
	Memory addressed by the ES:rDI register pair (for example, MOVS, CMPS, INS, STOS, or SCAS).
*/
#define	_INS_AM_Y	0x1700




/*====================================================================================================
	Codes for Addressing Method
	
	0x80xx ~ 0xffxx
====================================================================================================*/

#define	_INS_AM_CS		0x8000
#define	_INS_AM_SS		0x8100
#define	_INS_AM_DS		0x8200
#define	_INS_AM_ES		0x8300
#define	_INS_AM_FS		0x8400
#define	_INS_AM_GS		0x8500

#define	_INS_AM_eAX		0x8600
#define	_INS_AM_rAX		0x8700

#define	_INS_AM_eCX		0x8800
#define	_INS_AM_rCX		0x8900

#define	_INS_AM_eDX		0x8a00
#define	_INS_AM_rDX		0x8b00

#define	_INS_AM_eBX		0x8c00
#define	_INS_AM_rBX		0x8d00

#define	_INS_AM_eSP		0x8e00
#define	_INS_AM_rSP		0x8f00

#define	_INS_AM_eBP		0x9000
#define	_INS_AM_rBP		0x9100

#define	_INS_AM_eSI		0x9200
#define	_INS_AM_rSI		0x9300

#define	_INS_AM_eDI		0x9400
#define	_INS_AM_rDI		0x9500

#define	_INS_AM_r8		0xa600
#define	_INS_AM_r9		0xa700
#define	_INS_AM_r10		0xa800
#define	_INS_AM_r11		0xa900
#define	_INS_AM_r12		0xaa00
#define	_INS_AM_r13		0xab00
#define	_INS_AM_r14		0xac00
#define	_INS_AM_r15		0xad00


#define	_INS_AM_AL		0xb000


#define	_INS_AM_LABEL	0xc000









/*====================================================================================================
	Codes for Operand Type

	0x00 ~ 0x7f
====================================================================================================*/

/*
	Two one-word operands in memory or two double-word operands in memory, depending on operand-size
	attribute (used only by the BOUND instruction).
*/
#define	_INS_OT_a	0x0001

/*
	Byte, regardless of operand-size attribute.
*/
#define	_INS_OT_b	0x0002

/*
	Byte or word, depending on operand-size attribute.
*/
#define	_INS_OT_c	0x0003

/*
	Doubleword, regardless of operand-size attribute.
*/
#define	_INS_OT_d	0x0004

/*
	Double-quadword, regardless of operand-size attribute.
*/
#define	_INS_OT_dq	0x0005

/*
	32-bit, 48-bit, or 80-bit pointer, depending on operand-size attribute.
*/
#define	_INS_OT_p	0x0006

/*
	128-bit or 256-bit packed double-precision floating-point data.
*/
#define	_INS_OT_pd	0x0007

/*
	Quadword MMX technology register (for example: mm0).
*/
#define	_INS_OT_pi	0x0008

/*
	128-bit or 256-bit packed single-precision floating-point data.
*/
#define	_INS_OT_ps	0x0009

/*
	Quadword, regardless of operand-size attribute.
*/
#define	_INS_OT_q	0x000a

/*
	Quad-Quadword (256-bits), regardless of operand-size attribute.
*/
#define	_INS_OT_qq	0x000b

/*
	6-byte or 10-byte pseudo-descriptor.
*/
#define	_INS_OT_s	0x000c

/*
	Scalar element of a 128-bit double-precision floating data.
*/
#define	_INS_OT_sd	0x000d

/*
	Scalar element of a 128-bit single-precision floating data.
*/
#define	_INS_OT_ss	0x000e

/*
	Doubleword integer register (for example: eax).
*/
#define	_INS_OT_si	0x000f

/*
	Word, doubleword or quadword (in 64-bit mode), depending on operand-size attribute.
*/
#define	_INS_OT_v	0x0010

/*
	Word, regardless of operand-size attribute.
*/
#define	_INS_OT_w	0x0011

/*
	dq or qq based on the operand-size attribute.
*/
#define	_INS_OT_x	0x0012

/*
	Doubleword or quadword (in 64-bit mode), depending on operand-size attribute.
*/
#define	_INS_OT_y	0x0013

/*
	Word for 16-bit operand-size or doubleword for 32 or 64-bit operand-size.
*/
#define	_INS_OT_z	0x0014




/*====================================================================================================
	Superscripts Utilized in Opcode Tables

	0xx0 ~ 0xxf
====================================================================================================*/

#define	_INS_SS_NONE	0x00

/*
	Bits 5, 4, and 3 of ModR/M byte used as an opcode extension (refer to Section A.4, “Opcode Extensions For One-Byte
	And Two-byte Opcodes”).
*/
#define	_INS_SS_1A	0x01

/*
	Use the 0F0B opcode (UD2 instruction) or the 0FB9H opcode when deliberately trying to generate an invalid opcode
	exception (#UD).
*/
#define	_INS_SS_1B	0x02

/*
	Some instructions use the same two-byte opcode. If the instruction has variations, or the opcode represents
	different instructions, the ModR/M byte will be used to differentiate the instruction. For the value of the ModR/M
	byte needed to decode the instruction, see Table A-6.
*/
#define	_INS_SS_1C	0x03

/*
	The instruction is invalid or not encodable in 64-bit mode. 40 through 4F (single-byte INC and DEC) are REX prefix
	combinations when in 64-bit mode (use FE/FF Grp 4 and 5 for INC and DEC).
*/
#define	_INS_SS_i64	0x04

/*
	Instruction is only available when in 64-bit mode.
*/
#define	_INS_SS_o64	0x05

/*
	When in 64-bit mode, instruction defaults to 64-bit operand size and cannot encode 32-bit operand size.
*/
#define	_INS_SS_d64	0x06

/*
	The operand size is forced to a 64-bit operand size when in 64-bit mode (prefixes that change operand size are
	ignored for this instruction in 64-bit mode).
*/
#define	_INS_SS_f64	0x07

/*
	VEX form only exists. There is no legacy SSE form of the instruction. For Integer GPR instructions it means VEX
	prefix required.
*/
#define	_INS_SS_v	0x08

/*
	VEX128 & SSE forms only exist (no VEX256), when can’t be inferred from the data size.
*/
#define	_INS_SS_v1	0x09

/*====================================================================================================
	Superscripts Utilized in Opcode Tables

	0x0x ~ 0xfx
====================================================================================================*/

#define	_INS_SS_PREFIX	0x10

typedef struct {
	
} InstructionEncoderData;

struct _Instruction;

typedef bool (* InstructionEncoder)(struct _Instruction *ins, InstructionEncoderData *data);

typedef struct _Instruction {
	/*======================================================================
		基本字段，不允许改变顺序。
		要新增字段在“扩展字段”后面添加。
	======================================================================*/
	char		*mnemonic;		// 助记符。
	
	uint8_t		superscript;	// Opcode Map里的指令上标。

	struct {
		uint8_t		opcode_len;		// Opcode的长度。值：1、2、3。
		uint8_t		opcode1;
		uint8_t		opcode2;
		uint8_t		opcode3;
	} opcode;
	
	struct {
		uint16_t	oprd1;
		uint16_t	oprd2;
		uint16_t	oprd3;
	} oprd;

	InstructionEncoder	encoder;


	/*======================================================================
		扩展字段。
	======================================================================*/

	struct {
		bool		used;
		uint8_t		mod_rm;

	} opcode_ext;
} Instruction;

#endif
