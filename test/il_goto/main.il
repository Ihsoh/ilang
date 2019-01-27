#include <test.ilh>

function main(argc:int, argv:**char):int {
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
		return 1;
	}

	return 0;
}
