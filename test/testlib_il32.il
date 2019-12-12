#include <testlib.ilh>

#define	_SET_PARAMS_1(p1)	{	\
	asm("pushl	%eax\n", "set#%eax"((p1)));	\
}

#define	_SET_PARAMS_2(p1, p2)	{	\
	_SET_PARAMS_1(p2)	\
	_SET_PARAMS_1(p1)	\
}

#define	_SET_PARAMS_3(p1, p2, p3)	{	\
	_SET_PARAMS_1(p3)	\
	_SET_PARAMS_1(p2)	\
	_SET_PARAMS_1(p1)	\
}

#define	_SET_PARAMS_4(p1, p2, p3, p4)	{	\
	_SET_PARAMS_1(p4)	\
	_SET_PARAMS_1(p3)	\
	_SET_PARAMS_1(p2)	\
	_SET_PARAMS_1(p1)	\
}

#define	_SET_PARAMS_5(p1, p2, p3, p4, p5)	{	\
	_SET_PARAMS_1(p5)	\
	_SET_PARAMS_1(p4)	\
	_SET_PARAMS_1(p3)	\
	_SET_PARAMS_1(p2)	\
	_SET_PARAMS_1(p1)	\
}

#define	_SET_PARAMS_6(p1, p2, p3, p4, p5, p6)	{	\
	_SET_PARAMS_1(p6)	\
	_SET_PARAMS_1(p5)	\
	_SET_PARAMS_1(p4)	\
	_SET_PARAMS_1(p3)	\
	_SET_PARAMS_1(p2)	\
	_SET_PARAMS_1(p1)	\
}

#define	_CLEAR_PARAMS_1	{	\
	asm("addl 	$4, %esp\n");	\
}

#define	_CLEAR_PARAMS_2	{	\
	asm("addl 	$8, %esp\n");	\
}

#define	_CLEAR_PARAMS_3	{	\
	asm("addl 	$12, %esp\n");	\
}

#define	_CLEAR_PARAMS_4	{	\
	asm("addl 	$16, %esp\n");	\
}

#define	_CLEAR_PARAMS_5	{	\
	asm("addl 	$20, %esp\n");	\
}

#define	_CLEAR_PARAMS_6	{	\
	asm("addl 	$24, %esp\n");	\
}

#define	_GET_RET_VALUE(r)	{	\
	asm("", "get#%eax"((r)));	\
}

function __tst_fdopen(fd:int, mode:*char):*struct FILE {
	var r:*struct FILE = NULL;

	_SET_PARAMS_2(fd, mode)
	asm("calll	fdopen\n");
	_GET_RET_VALUE(r)
	_CLEAR_PARAMS_2

	return r;
}

function __tst_malloc(sz:size_t):* {
	var r:* = NULL;

	_SET_PARAMS_1(sz)
	asm("calll	malloc\n");
	_GET_RET_VALUE(r)
	_CLEAR_PARAMS_1

	return r;
}

function __tst_free(ptr:*) {
	_SET_PARAMS_1(ptr)
	asm("calll	free\n");
	_CLEAR_PARAMS_1
}

function __tst_strcmp(a:*char, b:*char):int {
	var r:int = 0;

	_SET_PARAMS_2(a, b)
	asm("calll	strcmp\n");
	_GET_RET_VALUE(r)
	_CLEAR_PARAMS_2

	return r;
}

function __tst_exit(status:int) {
	_SET_PARAMS_1(status)
	asm("calll	exit\n");
	_CLEAR_PARAMS_1
}

function __tst_fputs(str:*char, fptr:*struct FILE):int {
	var r:int = 0;

	_SET_PARAMS_2(str, fptr)
	asm("calll	fputs\n");
	_GET_RET_VALUE(r)
	_CLEAR_PARAMS_2

	return r;
}

function __tst_fputi(n:int, fptr:*struct FILE):int {
	var r:int = 0;

	_SET_PARAMS_3(fptr, "%d", n)
	asm("calll	fprintf\n");
	_GET_RET_VALUE(r)
	_CLEAR_PARAMS_3

	return r;
}

function __tst_assert(file:*char, line:int) {
	_SET_PARAMS_4(stderr, "ASSERT FAILED(%s:%d)!\n", file, line)
	asm("calll	fprintf\n");
	_CLEAR_PARAMS_4

	__tst_exit(1);
}
