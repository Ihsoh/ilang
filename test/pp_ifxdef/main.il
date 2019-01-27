#include <test.ilh>

function main(argc:int, argv:**char):int {
	var a:int32;

	#define _A
	#define _B

	a = 0;
	#ifdef _NOTHING
		a = 1;
	#elifdef _A
		#ifdef _A
			#ifdef _NOTHING
				a = 2;
			#elifdef _NOTHING
				a = 3;
			#elifndef _NOTHING
				#ifdef _NOTHING
					a = 4;
				#elifdef _NOTHING
					a = 5;
				#elifndef _A
					a = 6;
				#else
					a = 7;
				#endif
			#else
				a = 8;
			#endif
		#elifdef _B
			a = 9;
		#elifndef _NOTHING
			a = 10;
		#else
			a = 11;
		#endif
	#elifdef _B
		a = 12;
	#elifndef _NOTHING
		a = 13;
	#elifndef _NOTHING
		a = 14;
	#else
		a = 15;
	#endif
	if (a != 7) {
		return 1;
	}

	return 0;
}
