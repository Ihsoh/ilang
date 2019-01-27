
struct A {
	var a:int8, b:int16, c:int32;
	var d:int64;
}

struct B {
	var a:int8;
	var b:struct A;
	var c:int32;
	var d:int64;
}

var global_var_char:char;
var global_var_int8:int8;
var global_var_uint8:uint8;
var global_var_int16:int16;
var global_var_uint16:uint16;
var global_var_int32:int32;
var global_var_uint32:uint32;
var global_var_int64:int64;
var global_var_uint64:uint64;
var global_var_float:float;
var global_var_double:double;
var global_var_struct_A:struct A;
var global_var_char_array_2:[2]char;
var global_var_char_array_2_3:[2][3]char;
var global_var_func1:*function (a:int32, b:int32);
var global_var_func2:*function (a:int32, b:int32, c:*function (a:int32, b:int32));
var global_var_ptr:*;
var global_var_char_ptr:*char;

var global_var_inited_char_1:char = 'a';
var global_var_inited_char_2:char = 'a' + 1;
var global_var_inited_char_3:char = 1;

var global_var_inited_int8_ptr:*int8 = cast<*int8>(12345678);

var global_var_string_1:*char = "abcdef\nwowowo~~~\n";
var global_var_string_2:*char = "abcdef\nwowowo~~~\n" + 2;
var global_var_string_3:*int32 = cast<*int32>("abcdef\nwowowo~~~\n");
var global_var_string_4:*int32 = cast<*int32>(
	cast<*int64>(
		cast<*int32>("abcdef\nwowowo~~~\n") + 1
	) + 1
);

function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function printf(fmt:*char, ...) dummy

function add_float_float(a:float, b:float):float {
	// return a + b;
	return 123;
}

function add_int32_int32(a:int32, b:int32):int32 {
	// return a + b;
	return 123;
}

function foo(a:int32, b:int32):int32 {
	var c:int32 = 1;
	var d:int32 = 2;
	var e:*char = "abc";
	var f:*char = e;
	var g:*char = global_var_string_1;
	var h:struct B;
	var i:int8 = h.b.a;
	var j:*struct B;
	var k:int8 = j->b.a;
}

function bar() {
	var a:[2][3]int32;
	var b:int32 = a[1][2];
}

function xyz() {
	var a:**int32;
	var b:int32 = a[1][2];
}

function qwe() {
	var a:int32 = 123;
	var b:int16 = a;
}

function add(a:int64, b:int64):int64 {
}

function main(argc:int32, argv:**char):int32 {
	// var result1:int32 = add(1, 2);
	// var fptr:*function(a:int64, b:int64):int64 = add;
	// var result2:int32 = fptr(3, 4);

	// var a:int32 = 1;
	// var b:int32 = -a;

	// var c:uint32 = 1;
	// var d:uint32 = -c;

	// var e:float = 1;
	// var f:float = -e;

	var a:int32 = 1;
	var b:int32 = 3;

	printf(
		"val = %d, %d, %d\n",
		a & b,
		a ^ b,
		a | b
	);

	return 0;
}
