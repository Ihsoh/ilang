#include <test.ilh>

function func_1() {
}

function func_2():int {
	return 123;
}

function func_3(a:int, b:int):int {
	return a + b;
}

function func_4(n:int, ...):int {
	return 0;
}

function main(argc:int, argv:**char):int {
	func_1();

	if (func_2() != 123) {
		return 1;
	}

	if (func_3(1, 2) != 3) {
		return 1;
	}

	func_4(6, 1, 2, 3, 4, 5, 6);

	var func_1_ptr:*function() = func_1;
	func_1_ptr();

	var func_2_ptr:*function():int = func_2;
	if (func_2_ptr() != 123) {
		return 1;
	}

	var func_3_ptr:*function(a:int, b:int):int = func_3;
	if (func_3_ptr(2, 3) != 5) {
		return 1;
	}

	var func_4_ptr:*function(n:int, ...):int = func_4;
	func_4_ptr(6, 1, 2, 3, 4, 5, 6);

	return 0;
}
