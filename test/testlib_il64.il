#include <testlib.ilh>

#define	_STACK_ALIGN16_BEGIN	\
	asm("pushq %rbp\n");	\
	asm("movq %rsp, %rbp\n");	\
	asm("subq $16, %rsp\n");	\
	asm("andq $0xfffffffffffffff0, %rsp\n");

#define	_STACK_ALIGN16_END	\
	asm("movq %rbp, %rsp\n");	\
	asm("popq %rbp\n");

#define	_SET_PARAMS_1(p1) {	\
	asm("", "set#%rdi"((p1)));	\
}

#define	_SET_PARAMS_2(p1, p2) {	\
	asm("", "set#%rdi"((p1)));	\
	asm("", "set#%rsi"((p2)));	\
}

#define	_SET_PARAMS_3(p1, p2, p3) {	\
	asm("", "set#%rdi"((p1)));	\
	asm("", "set#%rsi"((p2)));	\
	asm("", "set#%rdx"((p3)));	\
}

#define	_SET_PARAMS_4(p1, p2, p3, p4) {	\
	asm("", "set#%rdi"((p1)));	\
	asm("", "set#%rsi"((p2)));	\
	asm("", "set#%rdx"((p3)));	\
	asm("", "set#%rcx"((p4)));	\
}

#define	_SET_PARAMS_5(p1, p2, p3, p4, p5) {	\
	asm("", "set#%rdi"((p1)));	\
	asm("", "set#%rsi"((p2)));	\
	asm("", "set#%rdx"((p3)));	\
	asm("", "set#%rcx"((p4)));	\
	asm("", "set#%r8"((p5)));	\
}

#define	_SET_PARAMS_6(p1, p2, p3, p4, p5, p6) {	\
	asm("", "set#%rdi"((p1)));	\
	asm("", "set#%rsi"((p2)));	\
	asm("", "set#%rdx"((p3)));	\
	asm("", "set#%rcx"((p4)));	\
	asm("", "set#%r8"((p5)));	\
	asm("", "set#%r9"((p6)));	\
}




function __tst_fdopen(fd:int, mode:*char):*struct FILE {
	_SET_PARAMS_2(cast<int64>(fd), mode)

	_STACK_ALIGN16_BEGIN

	asm("callq _fdopen\n");

	_STACK_ALIGN16_END
}

function __tst_malloc(sz:size_t):* {
	_SET_PARAMS_1(sz)

	_STACK_ALIGN16_BEGIN

	asm("callq _malloc\n");

	_STACK_ALIGN16_END
}

function __tst_free(ptr:*) {
	_SET_PARAMS_1(ptr)

	_STACK_ALIGN16_BEGIN

	asm("callq _free\n");

	_STACK_ALIGN16_END
}

function __tst_strcmp(a:*char, b:*char):int {
	_SET_PARAMS_2(a, b)

	_STACK_ALIGN16_BEGIN

	asm("callq _strcmp\n");

	_STACK_ALIGN16_END
}

function __tst_exit(status:int) {
	_SET_PARAMS_1(cast<int64>(status))

	_STACK_ALIGN16_BEGIN

	asm("callq _exit\n");

	_STACK_ALIGN16_END
}

function __tst_fputs(str:*char, fptr:*struct FILE):int {
	_SET_PARAMS_2(str, fptr)

	_STACK_ALIGN16_BEGIN

	asm("callq _fputs\n");

	_STACK_ALIGN16_END
}

function __tst_fputi(n:int, fptr:*struct FILE):int {
	_SET_PARAMS_3(fptr, "%d", cast<int64>(n))

	_STACK_ALIGN16_BEGIN

	asm("callq _fprintf\n");

	_STACK_ALIGN16_END
}

function __tst_assert(file:*char, line:int) {
	_SET_PARAMS_4(stderr, "ASSERT FAILED(%s:%d)!\n", file, cast<int64>(line))

	_STACK_ALIGN16_BEGIN

	asm("callq _fprintf\n");

	_STACK_ALIGN16_END
		
	__tst_exit(1);
}
