#include <testlib.ilh>

function sum(n:int, ...):int {
	var ap:__va_list;
	var ap1:__va_list;
	__va_start(ap);
	__va_copy(ap1, ap);

	var total:int = 0;
	for (var i:int = 0; i < n; i++) {
		total += __va_arg(ap1, int);
	}

	__va_end(ap);
	__va_end(ap1);

	return total;
}

function main(argc:int, argv:**char):int {
	
	assert(sum(6, 1, 2, 3, 4, 5, 10) == 25);

	return 0;
}
