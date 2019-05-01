#include "asmgen_gas64.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "semantics.h"
#include "parser.h"
#include "lexer.h"

#include "asmgen.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define	_OUT_CHAR(ctx, chr)	{ fputc((chr), (ctx)->out); }
#define	_OUT_STR(ctx, str, len) { for (int __i = 0; __i < (len); __i++) { _OUT_CHAR((ctx), (str)[__i]) } }
#define	_OUT_CSTR(ctx, str) { _OUT_STR((ctx), (str), strlen((str))) }
#define	_OUT_FMT(ctx, fmt, ...) { fprintf((ctx)->out, fmt, __VA_ARGS__); }

static void _error(const char * msg) {
	assert(msg);
	fprintf(stderr, "ASMGen(GAS64) Error: %s\n", msg);
	exit(1);
}





static size_t _generate_func_tmp(
	ASMGeneratorGas64Context *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);

	size_t no = BE_FUNC_SYMBOL_NEXT_NO(func_symbol);

	rstr_appendf(rstr, "FUNC_TMP.%zu", no);

	return no;
}

static size_t _next_no(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);

	return ctx->counter++;
}

static int _new_string(
	ASMGeneratorGas64Context *ctx,
	ResizableString *name,
	const char *str,
	size_t len
) {
	assert(ctx);
	assert(str);

	// .section __TEXT,__cstring,cstring_literals
	// _STR.[X]
	// .asciz [STRING]

	size_t no = _next_no(ctx);

	if (name != NULL) {
		rstr_appendf(name, "_STR.%zu", no);
	}
	
	ResizableString unescaped_str;
	rstr_init(&unescaped_str);

	be_lexer_unescape_string(
		ctx->psrctx->lexctx,
		&unescaped_str,
		str, len
	);
	int unescaped_str_len = RSTR_LEN(&unescaped_str);

	rstr_append_with_cstr(
		ctx->head,
		".section __TEXT,__cstring,cstring_literals\n"
	);
	rstr_appendf(
		ctx->head,
		"_STR.%zu:\n",
		no
	);

	rstr_append_with_cstr(
		ctx->head,
		".asciz \""
	);
	for (int i = 0; i < unescaped_str_len; i++) {
		char chr = RSTR_CHAR(&unescaped_str, i);
		if (isprint(chr)) {
			rstr_append_with_char(ctx->head, chr);
		} else {
			rstr_appendf(ctx->head, "\\x%02X", chr);
		}
	}
	rstr_append_with_cstr(
		ctx->head,
		"\"\n\n"
	);

	rstr_free(&unescaped_str);

	return unescaped_str_len + 1;
}





#define	_ASM_REG_AX					0
#define	_ASM_REG_NAME_AL			"%al"
#define	_ASM_REG_NAME_AH			"%ah"
#define	_ASM_REG_NAME_AX			"%ax"
#define	_ASM_REG_NAME_EAX			"%eax"
#define	_ASM_REG_NAME_RAX			"%rax"

#define	_ASM_REG_BX					1
#define	_ASM_REG_NAME_BL			"%bl"
#define	_ASM_REG_NAME_BH			"%bh"
#define	_ASM_REG_NAME_BX			"%bx"
#define	_ASM_REG_NAME_EBX			"%ebx"
#define	_ASM_REG_NAME_RBX			"%rbx"

#define	_ASM_REG_CX					2
#define	_ASM_REG_NAME_CL			"%cl"
#define	_ASM_REG_NAME_CH			"%ch"
#define	_ASM_REG_NAME_CX			"%cx"
#define	_ASM_REG_NAME_ECX			"%ecx"
#define	_ASM_REG_NAME_RCX			"%rcx"

#define	_ASM_REG_DX					3
#define	_ASM_REG_NAME_DL			"%dl"
#define	_ASM_REG_NAME_DH			"%dh"
#define	_ASM_REG_NAME_DX			"%dx"
#define	_ASM_REG_NAME_EDX			"%edx"
#define	_ASM_REG_NAME_RDX			"%rdx"

#define	_ASM_REG_NAME_BPL			"%bpl"
#define	_ASM_REG_NAME_BP			"%bp"
#define	_ASM_REG_NAME_EBP			"%ebp"
#define	_ASM_REG_NAME_RBP			"%rbp"

#define	_ASM_REG_NAME_SIL			"%sil"
#define	_ASM_REG_NAME_SI			"%si"
#define	_ASM_REG_NAME_ESI			"%esi"
#define	_ASM_REG_NAME_RSI			"%rsi"

#define	_ASM_REG_NAME_DIL			"%dil"
#define	_ASM_REG_NAME_DI			"%di"
#define	_ASM_REG_NAME_EDI			"%edi"
#define	_ASM_REG_NAME_RDI			"%rdi"

#define	_ASM_REG_NAME_SPL			"%spl"
#define	_ASM_REG_NAME_SP			"%sp"
#define	_ASM_REG_NAME_ESP			"%esp"
#define	_ASM_REG_NAME_RSP			"%rsp"

#define	_ASM_REG_NAME_IP			"%ip"
#define	_ASM_REG_NAME_EIP			"%eip"
#define	_ASM_REG_NAME_RIP			"%rip"

#define	_ASM_REG_NAME_R8B			"%r8b"
#define	_ASM_REG_NAME_R8W			"%r8w"
#define	_ASM_REG_NAME_R8D			"%r8d"
#define	_ASM_REG_NAME_R8			"%r8"

#define	_ASM_REG_NAME_R9B			"%r9b"
#define	_ASM_REG_NAME_R9W			"%r9w"
#define	_ASM_REG_NAME_R9D			"%r9d"
#define	_ASM_REG_NAME_R9			"%r9"

#define	_ASM_REG_NAME_R10B			"%r10b"
#define	_ASM_REG_NAME_R10W			"%r10w"
#define	_ASM_REG_NAME_R10D			"%r10d"
#define	_ASM_REG_NAME_R10			"%r10"

#define	_ASM_REG_NAME_R11B			"%r11b"
#define	_ASM_REG_NAME_R11W			"%r11w"
#define	_ASM_REG_NAME_R11D			"%r11d"
#define	_ASM_REG_NAME_R11			"%r11"

#define	_ASM_REG_NAME_R12B			"%r12b"
#define	_ASM_REG_NAME_R12W			"%r12w"
#define	_ASM_REG_NAME_R12D			"%r12d"
#define	_ASM_REG_NAME_R12			"%r12"

#define	_ASM_REG_NAME_R13B			"%r13b"
#define	_ASM_REG_NAME_R13W			"%r13w"
#define	_ASM_REG_NAME_R13D			"%r13d"
#define	_ASM_REG_NAME_R13			"%r13"

#define	_ASM_REG_NAME_R14B			"%r14b"
#define	_ASM_REG_NAME_R14W			"%r14w"
#define	_ASM_REG_NAME_R14D			"%r14d"
#define	_ASM_REG_NAME_R14			"%r14"

#define	_ASM_REG_NAME_R15B			"%r15b"
#define	_ASM_REG_NAME_R15W			"%r15w"
#define	_ASM_REG_NAME_R15D			"%r15d"
#define	_ASM_REG_NAME_R15			"%r15"

#define	_ASM_REG_NAME_CR0			"%cr0"
#define	_ASM_REG_NAME_CR1			"%cr1"
#define	_ASM_REG_NAME_CR2			"%cr2"
#define	_ASM_REG_NAME_CR3			"%cr3"
#define	_ASM_REG_NAME_CR4			"%cr4"
#define	_ASM_REG_NAME_CR5			"%cr5"
#define	_ASM_REG_NAME_CR6			"%cr6"
#define	_ASM_REG_NAME_CR7			"%cr7"
#define	_ASM_REG_NAME_CR8			"%cr8"
#define	_ASM_REG_NAME_CR9			"%cr9"
#define	_ASM_REG_NAME_CR10			"%cr10"
#define	_ASM_REG_NAME_CR11			"%cr11"
#define	_ASM_REG_NAME_CR12			"%cr12"
#define	_ASM_REG_NAME_CR13			"%cr13"
#define	_ASM_REG_NAME_CR14			"%cr14"
#define	_ASM_REG_NAME_CR15			"%cr15"

#define	_ASM_REG_NAME_MSW			"%msw"
#define	_ASM_REG_NAME_MXCSR			"%mxcsr"

#define	_ASM_REG_NAME_DR0			"%dr0"
#define	_ASM_REG_NAME_DR1			"%dr1"
#define	_ASM_REG_NAME_DR2			"%dr2"
#define	_ASM_REG_NAME_DR3			"%dr3"
#define	_ASM_REG_NAME_DR4			"%dr4"
#define	_ASM_REG_NAME_DR5			"%dr5"
#define	_ASM_REG_NAME_DR6			"%dr6"
#define	_ASM_REG_NAME_DR7			"%dr7"
#define	_ASM_REG_NAME_DR8			"%dr8"
#define	_ASM_REG_NAME_DR9			"%dr9"
#define	_ASM_REG_NAME_DR10			"%dr10"
#define	_ASM_REG_NAME_DR11			"%dr11"
#define	_ASM_REG_NAME_DR12			"%dr12"
#define	_ASM_REG_NAME_DR13			"%dr13"
#define	_ASM_REG_NAME_DR14			"%dr14"

#define	_ASM_REG_NAME_CS			"%cs"
#define	_ASM_REG_NAME_SS			"%ss"
#define	_ASM_REG_NAME_DS			"%ds"
#define	_ASM_REG_NAME_ES			"%es"
#define	_ASM_REG_NAME_FS			"%fs"
#define	_ASM_REG_NAME_GS			"%gs"

#define	_ASM_REG_NAME_GDTR			"%gdtr"
#define	_ASM_REG_NAME_IDTR			"%idtr"
#define	_ASM_REG_NAME_TR			"%tr"
#define	_ASM_REG_NAME_LDTR			"%ldtr"

#define	_ASM_REG_NAME_FLAGS			"%flags"
#define	_ASM_REG_NAME_EFLAGS		"%eflags"
#define	_ASM_REG_NAME_RFLAGS		"%rflags"

#define	_ASM_REG_NAME_XMM0			"%xmm0"
#define	_ASM_REG_NAME_XMM1			"%xmm1"
#define	_ASM_REG_NAME_XMM2			"%xmm2"
#define	_ASM_REG_NAME_XMM3			"%xmm3"
#define	_ASM_REG_NAME_XMM4			"%xmm4"
#define	_ASM_REG_NAME_XMM5			"%xmm5"
#define	_ASM_REG_NAME_XMM6			"%xmm6"
#define	_ASM_REG_NAME_XMM7			"%xmm7"
#define	_ASM_REG_NAME_XMM8			"%xmm8"
#define	_ASM_REG_NAME_XMM9			"%xmm9"
#define	_ASM_REG_NAME_XMM10			"%xmm10"
#define	_ASM_REG_NAME_XMM11			"%xmm11"
#define	_ASM_REG_NAME_XMM12			"%xmm12"
#define	_ASM_REG_NAME_XMM13			"%xmm13"
#define	_ASM_REG_NAME_XMM14			"%xmm14"
#define	_ASM_REG_NAME_XMM15			"%xmm15"

#define	_ASM_REG_NAME_YMM0			"%ymm0"
#define	_ASM_REG_NAME_YMM1			"%ymm1"
#define	_ASM_REG_NAME_YMM2			"%ymm2"
#define	_ASM_REG_NAME_YMM3			"%ymm3"
#define	_ASM_REG_NAME_YMM4			"%ymm4"
#define	_ASM_REG_NAME_YMM5			"%ymm5"
#define	_ASM_REG_NAME_YMM6			"%ymm6"
#define	_ASM_REG_NAME_YMM7			"%ymm7"
#define	_ASM_REG_NAME_YMM8			"%ymm8"
#define	_ASM_REG_NAME_YMM9			"%ymm9"
#define	_ASM_REG_NAME_YMM10			"%ymm10"
#define	_ASM_REG_NAME_YMM11			"%ymm11"
#define	_ASM_REG_NAME_YMM12			"%ymm12"
#define	_ASM_REG_NAME_YMM13			"%ymm13"
#define	_ASM_REG_NAME_YMM14			"%ymm14"
#define	_ASM_REG_NAME_YMM15			"%ymm15"

#define	_ASM_REG_NAME_ZMM0			"%zmm0"
#define	_ASM_REG_NAME_ZMM1			"%zmm1"
#define	_ASM_REG_NAME_ZMM2			"%zmm2"
#define	_ASM_REG_NAME_ZMM3			"%zmm3"
#define	_ASM_REG_NAME_ZMM4			"%zmm4"
#define	_ASM_REG_NAME_ZMM5			"%zmm5"
#define	_ASM_REG_NAME_ZMM6			"%zmm6"
#define	_ASM_REG_NAME_ZMM7			"%zmm7"
#define	_ASM_REG_NAME_ZMM8			"%zmm8"
#define	_ASM_REG_NAME_ZMM9			"%zmm9"
#define	_ASM_REG_NAME_ZMM10			"%zmm10"
#define	_ASM_REG_NAME_ZMM11			"%zmm11"
#define	_ASM_REG_NAME_ZMM12			"%zmm12"
#define	_ASM_REG_NAME_ZMM13			"%zmm13"
#define	_ASM_REG_NAME_ZMM14			"%zmm14"
#define	_ASM_REG_NAME_ZMM15			"%zmm15"
#define	_ASM_REG_NAME_ZMM16			"%zmm16"
#define	_ASM_REG_NAME_ZMM17			"%zmm17"
#define	_ASM_REG_NAME_ZMM18			"%zmm18"
#define	_ASM_REG_NAME_ZMM19			"%zmm19"
#define	_ASM_REG_NAME_ZMM20			"%zmm20"
#define	_ASM_REG_NAME_ZMM21			"%zmm21"
#define	_ASM_REG_NAME_ZMM22			"%zmm22"
#define	_ASM_REG_NAME_ZMM23			"%zmm23"
#define	_ASM_REG_NAME_ZMM24			"%zmm24"
#define	_ASM_REG_NAME_ZMM25			"%zmm25"
#define	_ASM_REG_NAME_ZMM26			"%zmm26"
#define	_ASM_REG_NAME_ZMM27			"%zmm27"
#define	_ASM_REG_NAME_ZMM28			"%zmm28"
#define	_ASM_REG_NAME_ZMM29			"%zmm29"
#define	_ASM_REG_NAME_ZMM30			"%zmm30"
#define	_ASM_REG_NAME_ZMM31			"%zmm31"




#define	_ASM_CONST_0					"$0"

#define	_ASM_FUNC_RETURN_LABEL_PREFIX	"FUNC_RET"
#define	_ASM_FUNC_RETURN_LABEL			"RET"


/*
	获取同一个寄存器的不同宽度时的名称。
*/

#define	_REG_B(r)	(_asm_inst_reg(ctx, BE_TYPE_UINT8, (r)))
#define	_REG_W(r)	(_asm_inst_reg(ctx, BE_TYPE_UINT16, (r)))
#define	_REG_D(r)	(_asm_inst_reg(ctx, BE_TYPE_UINT32, (r)))
#define	_REG_Q(r)	(_asm_inst_reg(ctx, BE_TYPE_UINT64, (r)))

static const char * _asm_inst_reg(
	ASMGeneratorGas64Context *ctx,
	uint8_t type,
	int reg
) {
	assert(ctx);

	const char *regs[][4] = {
		{_ASM_REG_NAME_AL, _ASM_REG_NAME_AX, _ASM_REG_NAME_EAX, _ASM_REG_NAME_RAX},
		{_ASM_REG_NAME_BL, _ASM_REG_NAME_BX, _ASM_REG_NAME_EBX, _ASM_REG_NAME_RBX},
		{_ASM_REG_NAME_CL, _ASM_REG_NAME_CX, _ASM_REG_NAME_ECX, _ASM_REG_NAME_RCX},
		{_ASM_REG_NAME_DL, _ASM_REG_NAME_DX, _ASM_REG_NAME_EDX, _ASM_REG_NAME_RDX}
	};
	assert(reg < sizeof(regs) / sizeof(regs[0]));

	int idx = 0;
	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_UINT8: {
			idx = 0;
			break;
		}
		case BE_TYPE_INT16:
		case BE_TYPE_UINT16: {
			idx = 1;
			break;
		}
		case BE_TYPE_INT32:
		case BE_TYPE_UINT32:
		case BE_TYPE_FLOAT: {
			idx = 2;
			break;
		}
		case BE_TYPE_INT64:
		case BE_TYPE_UINT64:
		case BE_TYPE_DOUBLE:
		case BE_TYPE_POINTER: {
			idx = 3;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	return regs[reg][idx];
}

/*
	AT&T:	section:disp(base, index, scale)
	Intel: 	section:[base + index*scale + disp]
*/

static void _asm_inst_memref_base(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *base
) {
	assert(ctx);
	assert(rstr);
	assert(base);

	rstr_appendf(
		rstr,
		"(%s)",
		base
	);
}

static void _asm_inst_memref_base_disp(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *base,
	const char *disp
) {
	assert(ctx);
	assert(rstr);
	assert(base);
	assert(disp);

	rstr_appendf(
		rstr,
		"%s(%s)",
		disp,
		base
	);
}

/*
	AT&T:	$CONST
	Intel:	CONST
*/

static void _asm_inst_uint_const(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	uint64_t val
) {
	rstr_appendf(
		rstr,
		"$%llu",
		val
	);
}

/*
	构建指令工具函数。
*/

static void _asm_inst1(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *mnemonic,
	const char *arg1
) {
	assert(ctx);
	assert(rstr);
	assert(mnemonic);
	assert(arg1);

	rstr_appendf(
		rstr,
		"%s %s\n",
		mnemonic,
		arg1
	);
}

static void _asm_inst2(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *mnemonic,
	const char *arg1,
	const char *arg2
) {
	assert(ctx);
	assert(rstr);
	assert(mnemonic);
	assert(arg1);
	assert(arg2);

	rstr_appendf(
		rstr,
		"%s %s, %s\n",
		mnemonic,
		arg1,
		arg2
	);
}

static void _asm_inst3(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *mnemonic,
	const char *arg1,
	const char *arg2,
	const char *arg3
) {
	assert(ctx);
	assert(rstr);
	assert(mnemonic);
	assert(arg1);
	assert(arg2);
	assert(arg3);

	rstr_appendf(
		rstr,
		"%s %s, %s, %s\n",
		mnemonic,
		arg1,
		arg2,
		arg3
	);
}

/*
	MOV
*/

static void _asm_inst_mov_x_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	uint8_t type,
	const char *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	const char *mnemonic = "";

	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_UINT8: {
			mnemonic = "movb";
			break;
		}
		case BE_TYPE_INT16:
		case BE_TYPE_UINT16: {
			mnemonic = "movw";
			break;
		}
		case BE_TYPE_INT32:
		case BE_TYPE_UINT32:
		case BE_TYPE_FLOAT: {
			mnemonic = "movl";
			break;
		}
		case BE_TYPE_INT64:
		case BE_TYPE_UINT64:
		case BE_TYPE_DOUBLE:
		case BE_TYPE_POINTER: {
			mnemonic = "movq";
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	_asm_inst2(
		ctx,
		rstr,
		mnemonic,
		source,
		target
	);
}

static void _asm_inst_mov_sym_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserSymbol *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	_asm_inst_mov_x_x(
		ctx,
		rstr,
		BE_VAR_SYMBOL_GET_TYPE(target),
		RSTR_CSTR(BE_VAR_SYMBOL_GET_CODE_GEN_NAME(target)),
		source
	);
}

static void _asm_inst_mov_x_sym(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target,
	ParserSymbol *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	_asm_inst_mov_x_x(
		ctx,
		rstr,
		BE_VAR_SYMBOL_GET_TYPE(source),
		target,
		RSTR_CSTR(BE_VAR_SYMBOL_GET_CODE_GEN_NAME(source))
	);
}

static void _asm_inst_mov_sym_sym(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserSymbol *target,
	ParserSymbol *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);
	assert(BE_VAR_SYMBOL_GET_TYPE_SIZE(target) == BE_VAR_SYMBOL_GET_TYPE_SIZE(source));

	const char *reg = "";
	switch (BE_VAR_SYMBOL_GET_TYPE_SIZE(target)) {
		case 1: {
			reg = _ASM_REG_NAME_AL;
			break;
		}
		case 2: {
			reg = _ASM_REG_NAME_AX;
			break;
		}
		case 4: {
			reg = _ASM_REG_NAME_EAX;
			break;
		}
		case 8: {
			reg = _ASM_REG_NAME_RAX;
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	_asm_inst_mov_x_sym(ctx, rstr, reg, source);
	_asm_inst_mov_sym_x(ctx, rstr, target, reg);
}

/*
	LEA
*/

static void _asm_inst_lea_x_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	const char *mnemonic = "leaq";

	_asm_inst2(
		ctx,
		rstr,
		mnemonic,
		source,
		target
	);
}

static void _asm_inst_lea_x_sym(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target,
	ParserSymbol *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	_asm_inst_lea_x_x(
		ctx,
		rstr,
		target,
		RSTR_CSTR(BE_VAR_SYMBOL_GET_CODE_GEN_NAME(source))
	);
}

static void _asm_inst_lea_sym_sym(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserSymbol *target,
	ParserSymbol *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);
	assert(BE_VAR_SYMBOL_GET_TYPE(target) == BE_TYPE_POINTER);

	const char *reg = _ASM_REG_NAME_RAX;

	_asm_inst_lea_x_sym(ctx, rstr, reg, source);
	_asm_inst_mov_sym_x(ctx, rstr, target, reg);
}

/*
	ADD
*/

static void _asm_inst_add_x_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	uint8_t type,
	const char *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	const char *mnemonic = "";

	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_UINT8: {
			mnemonic = "addb";
			break;
		}
		case BE_TYPE_INT16:
		case BE_TYPE_UINT16: {
			mnemonic = "addw";
			break;
		}
		case BE_TYPE_INT32:
		case BE_TYPE_UINT32: {
			mnemonic = "addl";
			break;
		}
		case BE_TYPE_INT64:
		case BE_TYPE_UINT64:
		case BE_TYPE_POINTER: {
			mnemonic = "addq";
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	_asm_inst2(
		ctx,
		rstr,
		mnemonic,
		source,
		target
	);
}

/*
	JMP
	Jcc
*/

static void _asm_inst_jmp_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target
) {
	assert(ctx);
	assert(rstr);
	assert(target);

	const char *mnemonic = "jmp";

	_asm_inst1(ctx, rstr, mnemonic, target);
}

static void _asm_inst_je_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target
) {
	assert(ctx);
	assert(rstr);
	assert(target);

	const char *mnemonic = "je";

	_asm_inst1(ctx, rstr, mnemonic, target);
}

static void _asm_inst_jne_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *target
) {
	assert(ctx);
	assert(rstr);
	assert(target);

	const char *mnemonic = "jne";

	_asm_inst1(ctx, rstr, mnemonic, target);
}

/*
	CMP
*/

static void _asm_inst_cmp_x_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	uint8_t type,
	const char *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	const char *mnemonic = "";

	switch (type) {
		case BE_TYPE_CHAR:
		case BE_TYPE_INT8:
		case BE_TYPE_UINT8: {
			mnemonic = "cmpb";
			break;
		}
		case BE_TYPE_INT16:
		case BE_TYPE_UINT16: {
			mnemonic = "cmpw";
			break;
		}
		case BE_TYPE_INT32:
		case BE_TYPE_UINT32: {
			mnemonic = "cmpl";
			break;
		}
		case BE_TYPE_INT64:
		case BE_TYPE_UINT64:
		case BE_TYPE_POINTER: {
			mnemonic = "cmpq";
			break;
		}
		default: {
			assert(0);
			break;
		}
	}

	_asm_inst2(
		ctx,
		rstr,
		mnemonic,
		source,
		target
	);
}

static void _asm_inst_cmp_sym_x(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserSymbol *target,
	const char *source
) {
	assert(ctx);
	assert(rstr);
	assert(target);
	assert(source);

	_asm_inst_cmp_x_x(
		ctx,
		rstr,
		BE_VAR_SYMBOL_GET_TYPE(target),
		RSTR_CSTR(BE_VAR_SYMBOL_GET_CODE_GEN_NAME(target)),
		source
	);
}









static void _asm_constexpr(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr_val,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr_val);
	assert(node_constexpr);
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node_constexpr)) {
		case BE_TYPE_CHAR: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%lld",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%llu",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_FLOAT: {
			char str[128];
			float val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%X",
				*(uint32_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_DOUBLE: {
			char str[128];
			double val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_POINTER: {
			if (BE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_constexpr)) {
				BeParserConstexprString *conststr = BE_EXPR_AST_NODE_GET_CONSTSTR(node_constexpr);
				uint64_t ptr_val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);

				ResizableString conststr_name;
				rstr_init(&conststr_name);

				int len = _new_string(ctx, &conststr_name, conststr->str, conststr->len);

				rstr_appendf(
					rstr_val,
					"%s + %llu",
					RSTR_CSTR(&conststr_name),
					ptr_val
				);

				rstr_free(&conststr_name);
			} else {
				char str[128];
				uint64_t val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);
				snprintf(
					str, sizeof(str), "0x%llX",
					val
				);
				rstr_append_with_cstr(rstr_val, str);
			}
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _asm_constexpr_param(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr_val,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr_val);
	assert(node_constexpr);
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node_constexpr)) {
		case BE_TYPE_CHAR: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%lld",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "$%llu",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_FLOAT: {
			char str[128];
			float val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "$0x%X",
				*(uint32_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_DOUBLE: {
			char str[128];
			double val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "$0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_POINTER: {
			if (BE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_constexpr)) {
				BeParserConstexprString *conststr = BE_EXPR_AST_NODE_GET_CONSTSTR(node_constexpr);
				uint64_t ptr_val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);

				ResizableString conststr_name;
				rstr_init(&conststr_name);

				int len = _new_string(ctx, &conststr_name, conststr->str, conststr->len);

				ResizableString rstr_source;
				rstr_init(&rstr_source);
				_asm_inst_memref_base_disp(
					ctx,
					&rstr_source,
					_ASM_REG_NAME_RIP,
					RSTR_CSTR(&conststr_name)
				);

				_asm_inst_lea_x_x(
					ctx,
					ctx->body,
					_ASM_REG_NAME_RAX,
					RSTR_CSTR(&rstr_source)
				);

				ResizableString rstr_ptr_val;
				rstr_init(&rstr_ptr_val);
				_asm_inst_uint_const(ctx, &rstr_ptr_val, ptr_val);
				_asm_inst_add_x_x(
					ctx,
					ctx->body,
					BE_TYPE_POINTER,
					_ASM_REG_NAME_RAX,
					RSTR_CSTR(&rstr_ptr_val)
				);
				rstr_free(&rstr_ptr_val);

				rstr_append_with_cstr(
					rstr_val,
					_ASM_REG_NAME_RAX
				);

				rstr_free(&conststr_name);
				rstr_free(&rstr_source);
			} else {
				char str[128];
				uint64_t val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);
				snprintf(
					str, sizeof(str), "$0x%llX",
					val
				);
				rstr_append_with_cstr(rstr_val, str);
			}
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _asm_constexpr_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr);\
	assert(node_constexpr);
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	_asm_constexpr(ctx, rstr, node_constexpr);
}

static void _asm_zero_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case BE_NODE_TYPE_CHAR:
		case BE_NODE_TYPE_INT8:
		case BE_NODE_TYPE_UINT8:
		case BE_NODE_TYPE_INT16:
		case BE_NODE_TYPE_UINT16:
		case BE_NODE_TYPE_INT32:
		case BE_NODE_TYPE_UINT32:
		case BE_NODE_TYPE_INT64:
		case BE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		case BE_NODE_TYPE_FLOAT:
		case BE_NODE_TYPE_DOUBLE: {
			rstr_append_with_cstr(rstr, "0.0");
			break;
		}
		case BE_NODE_TYPE_STRUCT:
		case BE_NODE_TYPE_ARRAY: {
			assert(0);
			break;
		}
		case BE_NODE_TYPE_POINTER: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _asm_global_var_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_constexpr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	if (node_constexpr != NULL) {
		_asm_constexpr_initializer(ctx, rstr, node_constexpr);
	} else {
		_asm_zero_initializer(ctx, rstr, node_type);
	}
}

static void _asm_var(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_VAR);
	assert(node->nchilds > 0);

	int align = BE_VAR_AST_NODE_GET_ALIGN(node);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == BE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_identifier = node_var_item->childs[0];
		assert(node_identifier->type == BE_NODE_IDENTIFIER);
		ParserASTNode *node_type = node_var_item->childs[1];
		ParserASTNode *node_expr = NULL;

		if (node_var_item->nchilds == 3) {
			node_expr = node_var_item->childs[2];
		}

		switch (BE_VAR_AST_NODE_GET_TYPE(node)) {
			case BE_VAR_TYPE_GLOBAL: {
				// .section __DATA,__data
				// .globl [VAR_NAME]
				// .balign	[?]
				// [VAR_NAME]:
				// .[TYPE] [EXPR]

				rstr_append_with_cstr(ctx->body, ".section __DATA,__data\n");
				
				ResizableString rstr_var_name;
				rstr_init(&rstr_var_name);
				rstr_append_with_raw(
					&rstr_var_name,
					node_identifier->token->content,
					node_identifier->token->len
				);

				rstr_append_with_cstr(ctx->body, ".globl ");
				rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
				rstr_append_with_char(ctx->body, '\n');

				// .balign	[?]
				if (align > 0) {
					rstr_appendf(ctx->body, ".balign %d\n", align);
				}

				rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
				rstr_append_with_char(ctx->body, ':');
				rstr_append_with_char(ctx->body, '\n');

				switch (node_type->type) {
					case BE_NODE_TYPE_CHAR: {
						rstr_append_with_cstr(ctx->body, ".byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT8: {
						rstr_append_with_cstr(ctx->body, ".byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT8: {
						rstr_append_with_cstr(ctx->body, ".byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT16: {
						rstr_append_with_cstr(ctx->body, ".2byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT16: {
						rstr_append_with_cstr(ctx->body, ".2byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT32: {
						rstr_append_with_cstr(ctx->body, ".4byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT32: {
						rstr_append_with_cstr(ctx->body, ".4byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT64: {
						rstr_append_with_cstr(ctx->body, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT64: {
						rstr_append_with_cstr(ctx->body, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_FLOAT: {
						rstr_append_with_cstr(ctx->body, ".float ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_DOUBLE: {
						rstr_append_with_cstr(ctx->body, ".double ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_STRUCT: {
						rstr_appendf(
							ctx->body,
							".zero %zu\n",
							be_sem_calc_type_size(ctx->psrctx, node, node_type)
						);
						break;
					}
					case BE_NODE_TYPE_ARRAY: {
						rstr_appendf(
							ctx->body,
							".zero %zu\n",
							be_sem_calc_type_size(ctx->psrctx, node, node_type)
						);
						break;
					}
					case BE_NODE_TYPE_POINTER: {
						rstr_append_with_cstr(ctx->body, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->body, node_expr, node_type);
						break;
					}
					default: {
						assert(0);
						break;
					}
				}

				// 更新全局变量符号在代码生成时的名字。
				ParserSymbol *symbol = BE_VAR_ITEM_AST_NODE_GET_SYMBOL(node_var_item);
				BE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);
				ResizableString *code_gen_name = BE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);
				rstr_init(code_gen_name);
				_asm_inst_memref_base_disp(
					ctx,
					code_gen_name,
					_ASM_REG_NAME_RIP,
					RSTR_CSTR(&rstr_var_name)
				);

				rstr_free(&rstr_var_name);

				rstr_append_with_cstr(ctx->body, "\n\n");

				break;
			}
			case BE_VAR_TYPE_LOCAL: {
				ParserSymbol *symbol = BE_VAR_ITEM_AST_NODE_GET_SYMBOL(node_var_item);
				
				// int align = BE_VAR_SYMBOL_GET_ALIGN(symbol);
				int align = 8;
				assert(align);

				// size_t type_size = BE_VAR_SYMBOL_GET_TYPE_SIZE(symbol);
				size_t type_size = 8;
				assert(type_size != 0);

				if (ctx->local_var_address_counter % align > 0) {
					size_t align_padding = align - ctx->local_var_address_counter % align;
					ctx->local_var_address_counter += align_padding;
					ctx->local_var_size += align_padding;
				}

				BE_VAR_SYMBOL_SET_ADDRESS(symbol, ctx->local_var_address_counter);

				BE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);	
				ResizableString *code_gen_name = BE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);
				rstr_init(code_gen_name);
				rstr_appendf(
					code_gen_name,
					"%d(%%rbp)",
					-8 * ctx->local_var_index_counter
				);

				rstr_appendf(
					ctx->local_var_defs,
					"# local-var: type=local, index=%zu, address=%zu, align=%d, size=%zu, value=%s, name=",
					ctx->local_var_index_counter,
					ctx->local_var_address_counter,
					align,
					type_size,
					RSTR_CSTR(code_gen_name)
				);
				rstr_append_with_raw(
					ctx->local_var_defs,
					node_identifier->token->content,
					node_identifier->token->len
				);
				rstr_append_with_char(
					ctx->local_var_defs,
					'\n'
				);

				ctx->local_var_address_counter += type_size;
				ctx->local_var_size += type_size;
				ctx->local_var_index_counter++;

				break;
			}
			case BE_VAR_TYPE_STRUCT_MEMBER: {
				assert(0);
				break;
			}
			default: {
				assert(0);
			}
		}
	}
}




static ParserSymbol * _get_var_symbol_by_id_node(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_IDENTIFIER);

	ParserSymbol *symbol = parser_get_symbol_from_node(
		ctx->psrctx, node, BE_SYM_VAR, node->token
	);
	if (symbol == NULL) {
		assert(0);
	}

	return symbol;
}

static uint8_t _move_id_or_constexpr_to_reg(
	ASMGeneratorGas64Context *ctx,
	int target_reg,
	ParserASTNode *source_id_or_constexpr
) {
	assert(ctx);
	assert(source_id_or_constexpr);

	if (source_id_or_constexpr->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol = _get_var_symbol_by_id_node(ctx, source_id_or_constexpr);
		uint8_t symbol_type = BE_VAR_SYMBOL_GET_TYPE(symbol);
		
		_asm_inst_mov_x_sym(
			ctx,
			ctx->body,
			_asm_inst_reg(ctx, symbol_type, target_reg),
			symbol
		);

		return symbol_type;
	} else if (source_id_or_constexpr->type == BE_NODE_EXPR) {
		ResizableString rstr;
		rstr_init(&rstr);
		_asm_constexpr_param(ctx, &rstr, source_id_or_constexpr);
		uint8_t type = BE_EXPR_AST_NODE_GET_TYPE(source_id_or_constexpr);
		
		_asm_inst_mov_x_x(
			ctx,
			ctx->body,
			type,
			_asm_inst_reg(ctx, type, target_reg),
			RSTR_CSTR(&rstr)
		);

		rstr_free(&rstr);
		return type;
	} else {
		assert(0);
	}
}

static void _asm_stat_var(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_VAR);

	_asm_var(ctx, node);
}

static void _asm_stat_assign(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASSIGN);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);

	ParserASTNode *node_source = node->childs[1];
	if (node_source->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);
		_asm_inst_mov_sym_sym(ctx, ctx->body, symbol_target, symbol_source);
	} else if (node_source->type == BE_NODE_EXPR) {
		ResizableString rstr_source;
		rstr_init(&rstr_source);
		_asm_constexpr_param(ctx, &rstr_source, node_source);
		
		uint8_t type_target = BE_VAR_SYMBOL_GET_TYPE(symbol_target);
		_asm_inst_mov_x_x(
			ctx,
			ctx->body,
			type_target,
			_asm_inst_reg(ctx, type_target, _ASM_REG_AX),
			RSTR_CSTR(&rstr_source)
		);

		_asm_inst_mov_sym_x(
			ctx,
			ctx->body,
			symbol_target,
			_asm_inst_reg(ctx, type_target, _ASM_REG_AX)
		);

		rstr_free(&rstr_source);
	} else {
		assert(0);
	}
}

static void _out_literal_string(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node
) {
	assert(ctx);
	assert(rstr);
	assert(node);
	assert(node->type == BE_NODE_LITERAL_STRING);
	
	ResizableString unescaped_str;
	rstr_init(&unescaped_str);
	be_lexer_unescape_string(
		ctx->psrctx->lexctx,
		&unescaped_str,
		node->token->content + 1,
		node->token->len - 2
	);

	rstr_append_with_cstr(
		rstr,
		RSTR_CSTR(&unescaped_str)
	);

	rstr_free(&unescaped_str);
}

static void _asm_stat_asm(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM);
	assert(node->nchilds == 1);

	ParserASTNode *node_asm_code = node->childs[0];
	assert(node_asm_code->type == BE_NODE_LITERAL_STRING);

	_out_literal_string(ctx, ctx->body, node_asm_code);
}

static void _asm_stat_asm_set_reg(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM_SET_REG);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_LITERAL_STRING);

	ResizableString rstr_reg_target;
	rstr_init(&rstr_reg_target);
	_out_literal_string(ctx, &rstr_reg_target, node_target);

	ParserASTNode *node_source = node->childs[1];
	if (node_source->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);

		_asm_inst_mov_x_sym(
			ctx,
			ctx->body,
			RSTR_CSTR(&rstr_reg_target),
			symbol_source
		);
	} else if (node_source->type == BE_NODE_EXPR) {
		ResizableString rstr_source;
		rstr_init(&rstr_source);
		_asm_constexpr_param(ctx, &rstr_source, node_source);

		uint8_t type_source = BE_EXPR_AST_NODE_GET_TYPE(node_source);
		const char *tmp_reg = _asm_inst_reg(ctx, type_source, _ASM_REG_AX);
		_asm_inst_mov_x_x(
			ctx,
			ctx->body,
			type_source,
			tmp_reg,
			RSTR_CSTR(&rstr_source)
		);

		_asm_inst_mov_x_x(
			ctx,
			ctx->body,
			type_source,
			RSTR_CSTR(&rstr_reg_target),
			tmp_reg
		);

		rstr_free(&rstr_source);
	} else {
		assert(0);
	}

	rstr_free(&rstr_reg_target);
}

static void _asm_stat_asm_get_reg(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_ASM_GET_REG);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);

	ParserASTNode *node_source = node->childs[1];
	assert(node_source->type == BE_NODE_LITERAL_STRING);

	ResizableString rstr_reg;
	rstr_init(&rstr_reg);
	_out_literal_string(ctx, &rstr_reg, node_source);

	_asm_inst_mov_sym_x(
		ctx,
		ctx->body,
		symbol_target,
		RSTR_CSTR(&rstr_reg)
	);

	rstr_free(&rstr_reg);
}

static void __out_label(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *prefix,
	const char *label
) {
	assert(ctx);
	assert(rstr);
	assert(label);

	ParserSymbol *func_symbol = ctx->func_symbol;
	assert(func_symbol);
	ParserASTNode *func_name_node = BE_FUNC_SYMBOL_GET_FUNC_NAME_NODE(func_symbol);
	assert(func_name_node);

	rstr_append_with_cstr(rstr, "_LABEL.");
	rstr_append_with_raw(rstr, func_name_node->token->content, func_name_node->token->len);
	rstr_append_with_char(rstr, '.');
	if (prefix != NULL) {
		rstr_appendf(rstr, "%s.", prefix);
	}
	rstr_append_with_cstr(rstr, label);
}

static void _out_label(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	const char *prefix,
	ParserASTNode *node_id
) {
	assert(ctx);
	assert(rstr);
	assert(node_id);

	ResizableString rstr_id;
	rstr_init_with_raw(&rstr_id, node_id->token->content, node_id->token->len);
	__out_label(ctx, rstr, prefix, RSTR_CSTR(&rstr_id));
	rstr_free(&rstr_id);
}

static void _asm_stat_label(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_LABEL);
	assert(node->nchilds == 1);

	ParserASTNode *node_id = node->childs[0];
	assert(node_id->type == BE_NODE_IDENTIFIER);

	_out_label(ctx, ctx->body, NULL, node_id);
	rstr_append_with_cstr(ctx->body, ":\n");
}

static void _asm_stat_br(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_BR);
	assert(node->nchilds == 1);

	ParserASTNode *node_id = node->childs[0];
	assert(node_id->type == BE_NODE_IDENTIFIER);

	ResizableString rstr_id;
	rstr_init(&rstr_id);
	_out_label(ctx, &rstr_id, NULL, node_id);

	_asm_inst_jmp_x(ctx, ctx->body, RSTR_CSTR(&rstr_id));

	rstr_free(&rstr_id);
}

static void _asm_stat_cbr(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_CBR);
	assert(node->nchilds == 3);

	ParserASTNode *node_id_cond = node->childs[0];

	ParserASTNode *node_label_true = node->childs[1];
	assert(node_label_true->type == BE_NODE_IDENTIFIER);
	ResizableString rstr_label_true;
	rstr_init(&rstr_label_true);
	_out_label(ctx, &rstr_label_true, NULL, node_label_true);

	ParserASTNode *node_label_false = node->childs[2];
	assert(node_label_false->type == BE_NODE_IDENTIFIER);
	ResizableString rstr_label_false;
	rstr_init(&rstr_label_false);
	_out_label(ctx, &rstr_label_false, NULL, node_label_false);


	if (node_id_cond->type == BE_NODE_IDENTIFIER) {
		ParserSymbol *symbol_id_cond = _get_var_symbol_by_id_node(ctx, node_id_cond);
		_asm_inst_cmp_sym_x(ctx, ctx->body, symbol_id_cond, _ASM_CONST_0);
	} else if (node_id_cond->type == BE_NODE_EXPR) {
		ResizableString rstr_id_cond;
		rstr_init(&rstr_id_cond);
		_asm_constexpr_param(ctx, &rstr_id_cond, node_id_cond);

		uint8_t type_id_cond = BE_EXPR_AST_NODE_GET_TYPE(node_id_cond);
		const char *tmp_reg = _asm_inst_reg(ctx, type_id_cond, _ASM_REG_AX);
		_asm_inst_mov_x_x(
			ctx,
			ctx->body,
			type_id_cond,
			tmp_reg,
			RSTR_CSTR(&rstr_id_cond)
		);

		_asm_inst_cmp_x_x(
			ctx,
			ctx->body,
			type_id_cond,
			tmp_reg,
			_ASM_CONST_0
		);

		rstr_free(&rstr_id_cond);
	} else {
		assert(0);
	}

	_asm_inst_je_x(ctx, ctx->body, RSTR_CSTR(&rstr_label_false));
	_asm_inst_jne_x(ctx, ctx->body, RSTR_CSTR(&rstr_label_true));

	rstr_free(&rstr_label_true);
	rstr_free(&rstr_label_false);
}

static void _asm_stat_return(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_RETURN);
	assert(node->nchilds == 0
			|| node->nchilds == 1);

	if (node->nchilds == 1) {
		ParserASTNode *node_ret_val =  node->childs[0];
		if (node_ret_val->type == BE_NODE_IDENTIFIER) {
			ParserSymbol *symbol_ret_val = _get_var_symbol_by_id_node(ctx, node_ret_val);
			uint8_t type_ret_val = BE_VAR_SYMBOL_GET_TYPE(symbol_ret_val);
			_asm_inst_mov_x_sym(
				ctx,
				ctx->body,
				_asm_inst_reg(ctx, type_ret_val, _ASM_REG_AX),
				symbol_ret_val
			);
		} else if (node_ret_val->type == BE_NODE_EXPR) {
			ResizableString rstr_ret_val;
			rstr_init(&rstr_ret_val);
			_asm_constexpr_param(ctx, &rstr_ret_val, node_ret_val);

			uint8_t type_ret_val = BE_EXPR_AST_NODE_GET_TYPE(node_ret_val);
			_asm_inst_mov_x_x(
				ctx,
				ctx->body,
				type_ret_val,
				_asm_inst_reg(ctx, type_ret_val, _ASM_REG_AX),
				RSTR_CSTR(&rstr_ret_val)
			);

			rstr_free(&rstr_ret_val);
		} else {
			assert(0);
		}
	}

	ResizableString rstr_ret;
	rstr_init(&rstr_ret);
	__out_label(
		ctx,
		&rstr_ret,
		_ASM_FUNC_RETURN_LABEL_PREFIX,
		_ASM_FUNC_RETURN_LABEL
	);
	_asm_inst_jmp_x(ctx, ctx->body, RSTR_CSTR(&rstr_ret));
	rstr_free(&rstr_ret);
}

static void _asm_stat_ref(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_REF);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);

	ParserASTNode *node_source = node->childs[1];
	assert(node_source->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_source = _get_var_symbol_by_id_node(ctx, node_source);

	_asm_inst_lea_sym_sym(
		ctx,
		ctx->body,
		symbol_target,
		symbol_source
	);
}

static void _asm_stat_store(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_STORE);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	uint8_t type_target = _move_id_or_constexpr_to_reg(
		ctx,
		_ASM_REG_AX,
		node_target
	);
	assert(type_target == BE_TYPE_POINTER);

	ParserASTNode *node_source = node->childs[1];
	uint8_t type_source = _move_id_or_constexpr_to_reg(
		ctx,
		_ASM_REG_BX,
		node_source
	);

	ResizableString rstr_memref;
	rstr_init(&rstr_memref);
	_asm_inst_memref_base(ctx, &rstr_memref, _ASM_REG_NAME_RAX);

	_asm_inst_mov_x_x(
		ctx,
		ctx->body,
		type_source,
		RSTR_CSTR(&rstr_memref),
		_asm_inst_reg(ctx, type_source, _ASM_REG_BX)
	);

	rstr_free(&rstr_memref);
}

static void _asm_stat_load(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_STAT_LOAD);
	assert(node->nchilds == 2);

	ParserASTNode *node_target = node->childs[0];
	assert(node_target->type == BE_NODE_IDENTIFIER);
	ParserSymbol *symbol_target = _get_var_symbol_by_id_node(ctx, node_target);
	uint8_t type_target = BE_VAR_SYMBOL_GET_TYPE(symbol_target);

	ParserASTNode *node_source = node->childs[1];
	uint8_t type_source = _move_id_or_constexpr_to_reg(
		ctx,
		_ASM_REG_AX,
		node_source
	);
	assert(type_source == BE_TYPE_POINTER);

	ResizableString rstr_memref;
	rstr_init(&rstr_memref);
	_asm_inst_memref_base(ctx, &rstr_memref, _ASM_REG_NAME_RAX);

	const char *tmp_reg = _asm_inst_reg(ctx, type_target, _ASM_REG_AX);

	_asm_inst_mov_x_x(
		ctx,
		ctx->body,
		type_target,
		tmp_reg,
		RSTR_CSTR(&rstr_memref)
	);

	_asm_inst_mov_sym_x(
		ctx,
		ctx->body,
		symbol_target,
		tmp_reg
	);

	rstr_free(&rstr_memref);
}






static void _asm_stat(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node_stat
) {
	assert(ctx);
	assert(node_stat);

	switch (node_stat->type) {
		case BE_NODE_VAR: {
			_asm_stat_var(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_ASSIGN: {
			_asm_stat_assign(ctx, node_stat);
			break;
		}

		case BE_NODE_STAT_ASM: {
			_asm_stat_asm(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_ASM_SET_REG: {
			_asm_stat_asm_set_reg(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_ASM_GET_REG: {
			_asm_stat_asm_get_reg(ctx, node_stat);
			break;
		}

		case BE_NODE_STAT_LABEL: {
			_asm_stat_label(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_BR: {
			_asm_stat_br(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_CBR: {
			_asm_stat_cbr(ctx, node_stat);
			break;
		}

		case BE_NODE_STAT_RETURN: {
			_asm_stat_return(ctx, node_stat);
			break;
		}

		case BE_NODE_STAT_REF: {
			_asm_stat_ref(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_STORE: {
			_asm_stat_store(ctx, node_stat);
			break;
		}
		case BE_NODE_STAT_LOAD: {
			_asm_stat_load(ctx, node_stat);
			break;
		}




		case BE_NODE_STAT_DUMMY: {
			
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _asm_stats_block(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node_stats_block
) {
	assert(ctx);
	assert(node_stats_block);
	assert(node_stats_block->type == BE_NODE_STATS_BLOCK);

	for (int i = 0; i < node_stats_block->nchilds; i++) {
		ParserASTNode *node_stat = node_stats_block->childs[i];
		_asm_stat(ctx, node_stat);
	}
}

static void _asm_func(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_FUNC);

	ctx->func_symbol = BE_FUNC_AST_NODE_GET_SYMBOL(node);
	assert(ctx->func_symbol);

	ParserASTNode *node_identifier = NULL;
	ParserASTNode *node_func_params = NULL;
	ParserASTNode *node_return_type = NULL;
	ParserASTNode *node_body = NULL;

	int nchilds = node->nchilds;
	if (nchilds == 3) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_body = node->childs[2];
	} else if (nchilds == 4) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_return_type = node->childs[2];
		node_body = node->childs[3];
	} else {
		assert(0);
	}
	assert(node_identifier->type == BE_NODE_IDENTIFIER);
	assert(node_func_params->type == BE_NODE_FUNC_PARAMS);
	if (node_func_params->nchilds > 0) {
		assert(node_func_params->childs[0]->type == BE_NODE_FUNC_PARAMS_ITEM);
	}

	rstr_append_with_char(ctx->body, '\n');

	if (node_body->type == BE_NODE_STATS_BLOCK) {
		// .section	__TEXT,__text,regular,pure_instructions
		rstr_append_with_cstr(ctx->body, ".section	__TEXT,__text,regular,pure_instructions\n");

		// .globl _FUNCTION_NAME
		rstr_append_with_cstr(ctx->body, ".globl ");
		rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
		rstr_append_with_char(ctx->body, '\n');

		// .p2align	4, 0x90
		// TODO: ...

		// _FUNCTION_NAME:
		rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
		rstr_append_with_char(ctx->body, ':');
		rstr_append_with_char(ctx->body, '\n');

		// pushq %rbp
		// movq %rsp, %rbp
		rstr_append_with_cstr(ctx->body, "pushq %rbp\nmovq %rsp, %rbp\n\n");

		/*
			Position			Contents				Frame
			------------------------------------------------------
			8n+16(%rbp)			param N
								...						previous
			16(%rbp)			param 0			
			------------------------------------------------------
			8(%rbp)				return address
			0(%rbp)				previous %rbp value
			-8(%rbp)			local variable 0		current
			-16(%rbp)			local variable 1
			-24(%rbp)			local variable 2
								...
		*/
		ctx->local_var_address_counter = 0;
		ctx->local_var_size = 0;
		ctx->local_var_index_counter = 1;
		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_param = node_func_params->childs[i];
			if (node_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				continue;
			}

			ParserASTNode *node_param_id = node_param->childs[0];
			ParserASTNode *node_param_type = node_param->childs[1];
			ParserSymbol *symbol = BE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_param);

			// int align = BE_VAR_SYMBOL_GET_ALIGN(symbol);
			int align = 8;
			assert(align);

			// size_t type_size = BE_VAR_SYMBOL_GET_TYPE_SIZE(symbol);
			size_t type_size = 8;
			assert(type_size != 0);

			if (ctx->local_var_address_counter % align > 0) {
				size_t align_padding = align - ctx->local_var_address_counter % align;
				ctx->local_var_address_counter += align_padding;
				ctx->local_var_size += align_padding;
			}

			BE_VAR_SYMBOL_SET_ADDRESS(symbol, ctx->local_var_address_counter);

			BE_VAR_SYMBOL_SET_HAS_CODE_GEN_NAME(symbol, true);	
			ResizableString *code_gen_name = BE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);
			rstr_init(code_gen_name);
			rstr_appendf(
				code_gen_name,
				"%d(%%rbp)",
				-8 * ctx->local_var_index_counter
			);

			rstr_appendf(
				ctx->body,
				"# local-var: type=param, index=%zu, address=%zu, align=%d, size=%zu, value=%s, name=",
				ctx->local_var_index_counter,
				ctx->local_var_address_counter,
				align,
				type_size,
				RSTR_CSTR(code_gen_name)
			);
			rstr_append_with_raw(
				ctx->body,
				node_param_id->token->content,
				node_param_id->token->len
			);
			rstr_append_with_char(
				ctx->body,
				'\n'
			);

			ctx->local_var_address_counter += type_size;
			ctx->local_var_size += type_size;
			ctx->local_var_index_counter++;
		}

		ResizableString *body = ctx->body;
		ctx->body = rstr_new();
		ctx->local_var_defs = rstr_new();

		_asm_stats_block(ctx, node_body);

		rstr_appendf(
			ctx->local_var_defs,
			"# local-var-space: total-size=%zu\n",
			ctx->local_var_size
		);

		rstr_appendf(
			ctx->local_var_defs,
			"subq $%zu, %%rsp\n\n",
			ctx->local_var_size
		);

		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_param = node_func_params->childs[i];
			if (node_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				continue;
			}

			ParserASTNode *node_param_type = node_param->childs[1];
			ParserSymbol *symbol = BE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_param);

			ResizableString *code_gen_name = BE_VAR_SYMBOL_GET_CODE_GEN_NAME(symbol);

			ResizableString rstr_source;
			rstr_init(&rstr_source);
			rstr_appendf(
				&rstr_source,
				"%d(%%rbp)",
				16 + 8 * i
			);

			_asm_inst_mov_x_x(
				ctx,
				ctx->local_var_defs,
				BE_TYPE_UINT64,
				_ASM_REG_NAME_RAX,
				RSTR_CSTR(&rstr_source)
			);

			_asm_inst_mov_x_x(
				ctx,
				ctx->local_var_defs,
				BE_TYPE_UINT64,
				RSTR_CSTR(code_gen_name),
				_ASM_REG_NAME_RAX
			);

			rstr_free(&rstr_source);
		}

		rstr_append_with_rstr(body, ctx->local_var_defs);
		rstr_append_with_cstr(body, "\n# code:\n");
		rstr_append_with_rstr(body, ctx->body);
	
		rstr_free(ctx->body);
		ctx->body = body;
		rstr_free(ctx->local_var_defs);
		ctx->local_var_defs = NULL;



		// 用于return指令的标签。
		__out_label(
			ctx,
			ctx->body,
			_ASM_FUNC_RETURN_LABEL_PREFIX,
			_ASM_FUNC_RETURN_LABEL
		);
		rstr_append_with_cstr(ctx->body, ":\n");

		// popq	%rbp
		// retq
		rstr_append_with_cstr(ctx->body, "\npopq %rbp\nretq\n");

		rstr_append_with_char(ctx->body, '\n');

	} else if (node_body->type == BE_NODE_DUMMY) {
		return;
	} else {
		assert(0);
	}
}






static void _asm_module_item(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	ctx->func_symbol = NULL;

	switch (node->type) {
		case BE_NODE_VAR: {
			_asm_var(ctx, node);
			break;
		}
		case BE_NODE_STRUCT: {
			// _asm_struct(ctx, node);
			break;
		}
		case BE_NODE_FUNC: {
			_asm_func(ctx, node);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _asm_module(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_MODULE);

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->global));

	for (int i = 0; i < node->nchilds; i++) {
		_asm_module_item(ctx, node->childs[i]);
	}

}




void be_asmgen_gas64_generate(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == BE_NODE_MODULE);
	assert(ctx->out);

	_asm_module(ctx, ctx->psrctx->ast);

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->head));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->body));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->foot));
}

ASMGeneratorGas64Context * be_asmgen_gas64_new_context(
	ParserContext *psrctx,
	FILE *out,
	int platform
) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	assert(
		platform == BE_ASMGEN_PLATFORM_WINDOWS
			|| platform == BE_ASMGEN_PLATFORM_SYSTEM_V
			|| platform == BE_ASMGEN_PLATFORM_ISYSTEM
	);
	
	ASMGeneratorGas64Context *ctx = (ASMGeneratorGas64Context *)malloc(sizeof(ASMGeneratorGas64Context));
	if (ctx == NULL) {
		_error("cannot allocate memory for a new context!");
	}

	ctx->psrctx = psrctx;
	ctx->out = out;

	ctx->global = rstr_new();
	ctx->head = rstr_new();
	ctx->body = rstr_new();
	ctx->foot = rstr_new();
	ctx->local_var_defs = NULL;

	ctx->local_var_address_counter = 0;
	ctx->local_var_size = 0;

	rstr_init(ctx->global);
	rstr_init(ctx->head);
	rstr_init(ctx->body);
	rstr_init(ctx->foot);

	ctx->counter = 0;
	ctx->func_symbol = NULL;

	ctx->platform = platform;

	return ctx;
}

void be_asmgen_gas64_free_context(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);
	assert(ctx->global);
	assert(ctx->head);
	assert(ctx->body);
	assert(ctx->foot);

	rstr_free(ctx->global);
	rstr_free(ctx->head);
	rstr_free(ctx->body);
	rstr_free(ctx->foot);

	free(ctx->global);
	free(ctx->head);
	free(ctx->body);
	free(ctx->foot);

	ctx->global = NULL;
	ctx->head = NULL;
	ctx->body = NULL;
	ctx->foot = NULL;

	free(ctx);
}
