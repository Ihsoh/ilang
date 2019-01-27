#include <test.ilh>

function test_1() {
	var total:int = 0;
	var i:int = 1;
label _loop;
	total += i;
	++i;
	if (i > 100) {
		goto _end;
	}
	goto _loop;
label _end;
	if (total != 5050) {
		assert(0);
	}
}

function test_2() {
	var total:int = 0;
	var i:int = 1;
_loop:
	total += i;
	++i;
	if (i > 100) {
		goto _end;
	}
	goto _loop;
_end:
	if (total != 5050) {
		assert(0);
	}
}

function main(argc:int, argv:**char):int {
	test_1();
	test_2();

	return 0;
}
