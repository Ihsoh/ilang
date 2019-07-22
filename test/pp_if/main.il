#include <testlib.ilh>

function main(argc:int, argv:**char):int {
	var a:int32;

	a = 0;
	#if 1
		a = 1;
	#endif
	if (a != 1) {
		return 1;
	}

	#if 0
		return 1;
	#endif

	a = 0;
	#if 1
		a = 1;
	#else
		a = 2;
	#endif
	if (a != 1) {
		return 1;
	}

	a = 0;
	#if 0
		a = 1;
	#else
		a = 2;
	#endif
	if (a != 2) {
		return 1;
	}

	a = 0;
	#if 1
		a = 1;
	#elif 1
		a = 2;
	#else
		a = 3;
	#endif
	if (a != 1) {
		return 1;
	}

	a = 0;
	#if 1
		a = 1;
	#elif 0
		a = 2;
	#else
		a = 3;
	#endif
	if (a != 1) {
		return 1;
	}

	a = 0;
	#if 0
		a = 1;
	#elif 1
		a = 2;
	#else
		a = 3;
	#endif
	if (a != 2) {
		return 1;
	}

	a = 0;
	#if 0
		a = 1;
	#elif 0
		a = 2;
	#else
		a = 3;
	#endif
	if (a != 3) {
		return 1;
	}

	#if 0b0
		return 1;
	#endif

	#if 0o0
		return 1;
	#endif

	#if 0x0
		return 1;
	#endif

	#if '\0'
		return 1;
	#endif

	#define	_A	0
	#if _A
		return 1;
	#endif
	#undef	_A

	#if !(1 || 0) || !(0 || 1) || !(1 || 1) || (0 || 0)
		return 1;
	#endif

	#if (1 && 0) || (0 && 1) || !(1 && 1) || (0 && 0)
		return 1;
	#endif

	#if !(1 | 0) || !(0 | 1) || !(1 | 1) || (0 | 0)
		return 1;
	#endif

	#if !(1 ^ 0) || !(0 ^ 1) || (1 ^ 1) || (0 ^ 0)
		return 1;
	#endif

	#if (1 & 0) || (0 & 1) || !(1 & 1) || (0 & 0)
		return 1;
	#endif

	#if !(1 == 1)
		return 1;
	#endif

	#if !(1 != 2)
		return 1;
	#endif

	#if !(1 < 2)
		return 1;
	#endif

	#if !(1 <= 2)
		return 1;
	#endif

	#if !(2 > 1)
		return 1;
	#endif

	#if !(2 >= 1)
		return 1;
	#endif

	#if !((1 << 2) == 4)
		return 1;
	#endif

	#if !((4 >> 2) == 1)
		return 1;
	#endif

	#if !(1 + 2 == 3)
		return 1;
	#endif

	#if !(3 - 1 == 2)
		return 1;
	#endif

	#if !(2 * 3 == 6)
		return 1;
	#endif

	#if !(6 / 2 == 3)
		return 1;
	#endif

	#if !(3 % 2 == 1)
		return 1;
	#endif

	#if !1
		return 1;
	#endif

	#if !(+1 == 1)
		return 1;
	#endif

	#if !(--1 == 1)
		return 1;
	#endif

	#if !((~0xfffffffe & 0xffffffff) == 1)
		return 1;
	#endif

	#define	_A
	#if !defined(_A)
		return 1;
	#endif
	#undef	_A

	#define	_STR_1	ABC123
	#define	_STR_2	"ABC123"
	#define	_STR_3	ABC123x
	#define	_STR_4	"ABC123x"

	#if !streq("ABC123", "ABC123")
		return 1;
	#endif

	#if streq("ABC123", "ABC123x")
		return 1;
	#endif

	#if streq("ABC123x", "ABC123")
		return 1;
	#endif

	#if !streq("ABC123", _STR_1)
		return 1;
	#endif

	#if !streq(_STR_1, "ABC123")
		return 1;
	#endif

	#if streq("ABC123x", _STR_1)
		return 1;
	#endif

	#if streq(_STR_1, "ABC123x")
		return 1;
	#endif

	#if !streq(_STR_1, _STR_1)
		return 1;
	#endif

	#if !streq("ABC123", _STR_2)
		return 1;
	#endif

	#if !streq(_STR_2, "ABC123")
		return 1;
	#endif

	#if streq("ABC123x", _STR_2)
		return 1;
	#endif

	#if streq(_STR_2, "ABC123x")
		return 1;
	#endif

	#if !streq(_STR_2, _STR_2)
		return 1;
	#endif

	#if !streq(_STR_1, _STR_2)
		return 1;
	#endif

	#if !streq(_STR_2, _STR_1)
		return 1;
	#endif

	#if streq(_STR_1, _STR_3)
		return 1;
	#endif

	#if streq(_STR_3, _STR_1)
		return 1;
	#endif

	#if streq(_STR_1, _STR_4)
		return 1;
	#endif

	#if streq(_STR_4, _STR_1)
		return 1;
	#endif

	#if streq(_STR_2, _STR_3)
		return 1;
	#endif

	#if streq(_STR_3, _STR_2)
		return 1;
	#endif

	#if streq(_STR_2, _STR_4)
		return 1;
	#endif

	#if streq(_STR_4, _STR_2)
		return 1;
	#endif

	#undef	_STR_1
	#undef	_STR_2
	#undef	_STR_3
	#undef	_STR_4

	a = 0;
	#if 0
		a = 1;
	#elif 0
		a = 2;
	#elif 1
		#if 0
			a = 3;
		#elif 1
			#if 1
				a = 4;
			#else
				a = 5;
			#endif
		#else
			a = 6;
		#endif
	#elif 0
		a = 7;
	#else
		a = 8;
	#endif
	if (a != 4) {
		return 1;
	}

	return 0;
}
