#include <testlib.ilh>

function main(argc:int, argv:**char):int {
	var a:int32;

	#define _A
	#define _B

	a = 0;
	#if 1
		a = 1;
	#elif defined(_A)
		a = 2;
	#elifdef _B
		a = 3;
	#elifndef _NOTHING
		a = 4;
	#else
		a = 5;
	#endif
	if (a != 1) {
		return 1;
	}

	a = 0;
	#ifdef _A
		a = 1;
	#elif defined(_A)
		a = 2;
	#elifdef _B
		a = 3;
	#elifndef _NOTHING
		a = 4;
	#else
		a = 5;
	#endif
	if (a != 1) {
		return 1;
	}

	a = 0;
	#ifndef _NOTHING
		a = 1;
	#elif defined(_A)
		a = 2;
	#elifdef _B
		a = 3;
	#elifndef _NOTHING
		a = 4;
	#else
		a = 5;
	#endif
	if (a != 1) {
		return 1;
	}

	return 0;
}
