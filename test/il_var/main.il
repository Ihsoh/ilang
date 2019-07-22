#include <testlib.ilh>

struct A {
	var a:int32;
	var b:int32;
}

var global_char:char;
var global_char_1:char = 'A';
var global_char_2:char = 'A' + 1;

var global_int8:int8;
var global_int8_1:int8 = 100;
var global_int8_2:int8 = 100 + 1;

var global_int16:int16;
var global_int16_1:int16 = 100;
var global_int16_2:int16 = 100 + 1;

var global_int32:int32;
var global_int32_1:int32 = 100;
var global_int32_2:int32 = 100 + 1;

var global_int64:int64;
var global_int64_1:int64 = 100;
var global_int64_2:int64 = 100 + 1;

var global_uint8:uint8;
var global_uint8_1:uint8 = 100;
var global_uint8_2:uint8 = 100 + 1;

var global_uint16:uint16;
var global_uint16_1:uint16 = 100;
var global_uint16_2:uint16 = 100 + 1;

var global_uint32:uint32;
var global_uint32_1:uint32 = 100;
var global_uint32_2:uint32 = 100 + 1;

var global_uint64:uint64;
var global_uint64_1:uint64 = 100;
var global_uint64_2:uint64 = 100 + 1;

var global_float:float;
var global_float_1:float = 100;
var global_float_2:float = 100 + 1;
var global_float_3:float = 100.1;
var global_float_4:float = 100.1 + 0.2;

var global_double:double;
var global_double_1:double = 100;
var global_double_2:double = 100 + 1;
var global_double_3:double = 100.1;
var global_double_4:double = 100.1 + 0.2;

var global_struct_A:struct A;

var global_char_arr_10:[10]char;
var global_char_arr_10_100:[10][100]char;
var global_struct_A_arr_10:[10]struct A;
var global_struct_A_arr_10_100:[10][100]struct A;
var global_char_ptr_arr_10:[10]*char;
var global_char_ptr_arr_10_100:[10][100]*char;

var global_func_ptr1:*function();
var global_func_ptr2:*function(a:int32, b:int32):int32;
var global_func_ptr3:*function(fmt:*char, ...):int32;
var global_func_ptr4:*function() = 0;
var global_func_ptr5:*function(a:int32, b:int32):int32 = 0;
var global_func_ptr6:*function(fmt:*char, ...):int32 = 0;

var global_ptr:*;
var global_ptr_1:* = 0;

var global_char_ptr:*char;
var global_char_ptr_1:*char = 0;

var global_struct_A_ptr:*struct A;
var global_struct_A_ptr_1:*struct A = 0;

var global_char_arr_10_ptr:*[10]char;
var global_char_arr_10_ptr_1:*[10]char = 0;

var global_a:int32, global_b:int32 = 1;

function func_1() {
	var i:int32;
}

function func_2(a:int32, b:int32):int32 {
	return 0;
}

function func_3(fmt:*char, ...):int32 {
	return 0;
}

function main(argc:int, argv:**char):int {
	var local_char:char;
	var local_char_1:char = 'A';
	var local_char_2:char = 'A' + 1;
	var local_char_3:char = local_char_2 + 1;

	var local_int8:int8;
	var local_int8_1:int8 = 100;
	var local_int8_2:int8 = 100 + 1;
	var local_int8_3:int8 = local_int8_2 + 1;

	var local_int16:int16;
	var local_int16_1:int16 = 100;
	var local_int16_2:int16 = 100 + 1;
	var local_int16_3:int16 = local_int16_2 + 1;

	var local_int32:int32;
	var local_int32_1:int32 = 100;
	var local_int32_2:int32 = 100 + 1;
	var local_int32_3:int32 = local_int32_2 + 1;

	var local_int64:int64;
	var local_int64_1:int64 = 100;
	var local_int64_2:int64 = 100 + 1;
	var local_int64_3:int64 = local_int64_2 + 1;

	var local_uint8:uint8;
	var local_uint8_1:uint8 = 100;
	var local_uint8_2:uint8 = 100 + 1;
	var local_uint8_3:uint8 = local_uint8_2 + 1;

	var local_uint16:uint16;
	var local_uint16_1:uint16 = 100;
	var local_uint16_2:uint16 = 100 + 1;
	var local_uint16_3:uint16 = local_uint16_2 + 1;

	var local_uint32:uint32;
	var local_uint32_1:uint32 = 100;
	var local_uint32_2:uint32 = 100 + 1;
	var local_uint32_3:uint32 = local_uint32_2 + 1;

	var local_uint64:uint64;
	var local_uint64_1:uint64 = 100;
	var local_uint64_2:uint64 = 100 + 1;
	var local_uint64_3:uint64 = local_uint64_2 + 1;

	var local_float:float;
	var local_float_1:float = 100;
	var local_float_2:float = 100 + 1;
	var local_float_3:float = 100.1;
	var local_float_4:float = 100.1 + 0.2;
	var local_float_5:float = local_float_4 + 0.2;

	var local_double:double;
	var local_double_1:double = 100;
	var local_double_2:double = 100 + 1;
	var local_double_3:double = 100.1;
	var local_double_4:double = 100.1 + 0.2;
	var local_double_5:double = local_double_4 + 0.2;

	var local_struct_A:struct A;

	var local_char_arr_10:[10]char;
	var local_char_arr_10_100:[10][100]char;
	var local_struct_A_arr_10:[10]struct A;
	var local_struct_A_arr_10_100:[10][100]struct A;
	var local_char_ptr_arr_10:[10]*char;
	var local_char_ptr_arr_10_100:[10][100]*char;

	var local_func_ptr1:*function();
	var local_func_ptr2:*function(a:int32, b:int32):int32;
	var local_func_ptr3:*function(fmt:*char, ...):int32;
	var local_func_ptr4:*function() = 0;
	var local_func_ptr5:*function(a:int32, b:int32):int32 = 0;
	var local_func_ptr6:*function(fmt:*char, ...):int32 = 0;
	var local_func_ptr7:*function() = func_1;
	var local_func_ptr8:*function(a:int32, b:int32):int32 = func_2;
	var local_func_ptr9:*function(fmt:*char, ...):int32 = func_3;

	var local_ptr:*;
	var local_ptr_1:* = 0;
	var local_ptr_2:* = local_ptr_1 + 1;

	var local_char_ptr:*char;
	var local_char_ptr_1:*char = 0;
	var local_char_ptr_2:*char = local_char_ptr_1 + 1;

	var local_struct_A_ptr:*struct A;
	var local_struct_A_ptr_1:*struct A = 0;
	var local_struct_A_ptr_2:*struct A = local_struct_A_ptr_1 + 1;

	var local_char_arr_10_ptr:*[10]char;
	var local_char_arr_10_ptr_1:*[10]char = 0;
	var local_char_arr_10_ptr_2:*[10]char = local_char_arr_10_ptr_1 + 1;

	var local_a:int32, local_b:int32 = 1, local_c:int32 = local_b;

	if (0) {
		var i:int32;
	}

	while (0) {
		var i:int32;
	}

	do {
		var i:int32;
	} while (0);

	for (var i:int32;0;) {
	}

	for (var i:int32, i1:int32 = 1, i2:int32 = i1;0;) {
	}

	for (;0;) {
		var i:int32;
	}

	return 0;
}
