#include <test.ilh>

#undef	_NOTHING

#define	_A	1
#undef	_A

#define	_B	2
#undef	_B
#define	_B	3

#define	_ADD(_a, _b)	(_a * _b)
#undef	_ADD
#define	_ADD(_a, _b)	(_a + _b)

function main(argc:int, argv:**char):int {
	#ifdef _A
	return 1;
	#endif

	if (_B != 3) {
		return 1;
	}

	if (_ADD(2, 3) != 5) {
		return 1;
	}

	return 0;
}
