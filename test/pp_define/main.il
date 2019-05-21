#include <test.ilh>

#define	_A	1

#define	_STR_A	#_A

#define	_B	100

#define	_C	(_A + _B)

#define	_ZERO()	0

#define	_INC(_var)	(++_var)

#define	_ADD(_a, _b)	(_a + _b)

#define	_ADD_ASSIGN(_var, _val)	(_var = _ADD(_var, _val))

#define	_SUM(_result, _start, _end)	_result = _ZERO(); for (var _i:int32 = _start; _i <= _end; _INC(_i)) { _ADD_ASSIGN(_result, _i); }

function main(argc:int, argv:**char):int {
	if (tst_strcmp(_STR_A, "1") != 0) {
		return 1;
	}

	if (tst_strcmp(#_B, "100") != 0) {
		return 1;
	}

	if (_ZERO() != 0) {
		return 1;
	}

	if (_A != 1) {
		return 1;
	}

	if (_B != 100) {
		return 1;
	}

	if (_C != _ADD(_A, _B)) {
		return 1;
	}

	var result:int32;
	_SUM(result, _A, _B)
	if (result != 5050) {
		return 1;
	}

	return 0;
}
