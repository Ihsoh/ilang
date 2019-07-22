#include <testlib.ilh>

function func_void_1() {
}

function func_void_2(a:int, b:int) {
}

function func_void_3(a:int, b:int, ...) {
}

function func_char_1():char {
	return 'a';
}

function func_char_2(a:int, b:int):char {
	return 'b';
}

function func_char_3(a:int, b:int, ...):char {
	return 'c';
}

function func_int8_1():int8 {
	return 1;
}

function func_int8_2(a:int, b:int):int8 {
	return 2;
}

function func_int8_3(a:int, b:int, ...):int8 {
	return 3;
}

function func_int16_1():int16 {
	return 1;
}

function func_int16_2(a:int, b:int):int16 {
	return 2;
}

function func_int16_3(a:int, b:int, ...):int16 {
	return 3;
}

function func_int32_1():int32 {
	return 1;
}

function func_int32_2(a:int, b:int):int32 {
	return 2;
}

function func_int32_3(a:int, b:int, ...):int32 {
	return 3;
}

function func_int64_1():int64 {
	return 1;
}

function func_int64_2(a:int, b:int):int64 {
	return 2;
}

function func_int64_3(a:int, b:int, ...):int64 {
	return 3;
}

function func_uint8_1():uint8 {
	return 1;
}

function func_uint8_2(a:int, b:int):uint8 {
	return 2;
}

function func_uint8_3(a:int, b:int, ...):uint8 {
	return 3;
}

function func_uint16_1():uint16 {
	return 1;
}

function func_uint16_2(a:int, b:int):uint16 {
	return 2;
}

function func_uint16_3(a:int, b:int, ...):uint16 {
	return 3;
}

function func_uint32_1():uint32 {
	return 1;
}

function func_uint32_2(a:int, b:int):uint32 {
	return 2;
}

function func_uint32_3(a:int, b:int, ...):uint32 {
	return 3;
}

function func_uint64_1():uint64 {
	return 1;
}

function func_uint64_2(a:int, b:int):uint64 {
	return 2;
}

function func_uint64_3(a:int, b:int, ...):uint64 {
	return 3;
}

function func_float_1():float {
	return 1;
}

function func_float_2(a:int, b:int):float {
	return 2;
}

function func_float_3(a:int, b:int, ...):float {
	return 3;
}

function func_double_1():double {
	return 1;
}

function func_double_2(a:int, b:int):double {
	return 2;
}

function func_double_3(a:int, b:int, ...):double {
	return 3;
}

function func_ptr_1():* {
	return cast<*>(1);
}

function func_ptr_2(a:int, b:int):* {
	return cast<*>(2);
}

function func_ptr_3(a:int, b:int, ...):* {
	return cast<*>(3);
}

function func_char_ptr_1():*char {
	return cast<*char>(1);
}

function func_char_ptr_2(a:int, b:int):*char {
	return cast<*char>(2);
}

function func_char_ptr_3(a:int, b:int, ...):*char {
	return cast<*char>(3);
}

function main(argc:int, argv:**char):int {

	func_void_1();
	func_void_2(1, 2);
	func_void_3(1, 2, 3, 5);

	if (func_char_1() != 'a') {
		return 1;
	}
	if (func_char_2(1, 2) != 'b') {
		return 1;
	}
	if (func_char_3(1, 2, 3, 4) != 'c') {
		return 1;
	}

	if (func_int8_1() != 1) {
		return 1;
	}
	if (func_int8_2(1, 2) != 2) {
		return 1;
	}
	if (func_int8_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_int16_1() != 1) {
		return 1;
	}
	if (func_int16_2(1, 2) != 2) {
		return 1;
	}
	if (func_int16_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_int32_1() != 1) {
		return 1;
	}
	if (func_int32_2(1, 2) != 2) {
		return 1;
	}
	if (func_int32_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_int64_1() != 1) {
		return 1;
	}
	if (func_int64_2(1, 2) != 2) {
		return 1;
	}
	if (func_int64_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_uint8_1() != 1) {
		return 1;
	}
	if (func_uint8_2(1, 2) != 2) {
		return 1;
	}
	if (func_uint8_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_uint16_1() != 1) {
		return 1;
	}
	if (func_uint16_2(1, 2) != 2) {
		return 1;
	}
	if (func_uint16_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_uint32_1() != 1) {
		return 1;
	}
	if (func_uint32_2(1, 2) != 2) {
		return 1;
	}
	if (func_uint32_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_uint64_1() != 1) {
		return 1;
	}
	if (func_uint64_2(1, 2) != 2) {
		return 1;
	}
	if (func_uint64_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_float_1() != 1) {
		return 1;
	}
	if (func_float_2(1, 2) != 2) {
		return 1;
	}
	if (func_float_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_double_1() != 1) {
		return 1;
	}
	if (func_double_2(1, 2) != 2) {
		return 1;
	}
	if (func_double_3(1, 2, 3, 4) != 3) {
		return 1;
	}

	if (func_ptr_1() != cast<*>(1)) {
		return 1;
	}
	if (func_ptr_2(1, 2) != cast<*>(2)) {
		return 1;
	}
	if (func_ptr_3(1, 2, 3, 4) != cast<*>(3)) {
		return 1;
	}

	if (func_char_ptr_1() != cast<*char>(1)) {
		return 1;
	}
	if (func_char_ptr_2(1, 2) != cast<*char>(2)) {
		return 1;
	}
	if (func_char_ptr_3(1, 2, 3, 4) != cast<*char>(3)) {
		return 1;
	}

	return 0;
}
