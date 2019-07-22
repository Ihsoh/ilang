#include <testlib.ilh>

#define	_STACK_ALIGN16_BEGIN	\
	asm("pushq %rbp\n");	\
	asm("movq %rsp, %rbp\n");	\
	asm("subq $16, %rsp\n");	\
	asm("andq $0xfffffffffffffff0, %rsp\n");

#define	_STACK_ALIGN16_END	\
	asm("movq %rbp, %rsp\n");	\
	asm("popq %rbp\n");

function __tst_fdopen(fd:int, mode:*char):*struct FILE {
	// parameter 'mode'
	asm("", "set#%rdi"(mode));

	// parameter 'fd'
	asm("xorq %rsi, %rsi\n");
	asm("", "set#%esi"(fd));

	_STACK_ALIGN16_BEGIN

	asm("callq _fdopen\n");

	_STACK_ALIGN16_END
}

function __tst_malloc(sz:size_t):* {
	// parameter 'sz'
	asm("", "set#%rdi"(sz));

	_STACK_ALIGN16_BEGIN

	asm("callq _malloc\n");

	_STACK_ALIGN16_END
}

function __tst_free(ptr:*) {
	// parameter 'ptr'
	asm("", "set#%rdi"(ptr));

	_STACK_ALIGN16_BEGIN

	asm("callq _free\n");

	_STACK_ALIGN16_END
}

function __tst_vfprintf(file:*struct FILE, fmt:*char, arg:*__va_list):int {
	return 0;
}

function __tst_strcmp(a:*char, b:*char):int {
	// parameter 'b'
	asm("", "set#%rdi"(b));

	// parameter 'a'
	asm("", "set#%rsi"(a));

	_STACK_ALIGN16_BEGIN

	asm("callq _strcmp\n");

	_STACK_ALIGN16_END
}

function __tst_exit(status:int) {
	// parameter 'status'
	asm("", "set#%rdi"(cast<int64>(status)));

	_STACK_ALIGN16_BEGIN

	asm("callq _exit\n");

	_STACK_ALIGN16_END
}
