var g_char_1:char;
var g_char_2:char = 'a';

var g_uint8_1:uint8;
var g_uint8_2:uint8 = 1;

var g_uint16_1:uint16;
var g_uint16_2:uint16 = 2;

var g_uint32_1:uint32;
var g_uint32_2:uint32 = 3;

var g_uint64_1:uint64;
var g_uint64_2:uint64 = 4;

var g_int8_1:int8;
var g_int8_2:int8 = 1;

var g_int16_1:int16;
var g_int16_2:int16 = 2;

var g_int32_1:int32;
var g_int32_2:int32 = 3;

var g_int64_1:int64;
var g_int64_2:int64 = -4;

var g_float_1:float;
var g_float_2:float = 1.23;

var g_double_1:double;
var g_double_2:double = 2.34;

var g_int64_ptr_1:*int64;
var g_int64_ptr_2:*int64 = 1234;

var g_string_1:*char;
var g_string_2:*char = "abc";
var g_string_3:*char = "abcdef" + 2;
var g_string_4:* = "def";
var g_string_5:* = "defghi" + 3;
var g_string_6:*char = "\ta bc\r\ndef\0ghi";

struct A dummy
var g_A_ptr_1:*struct A;
var g_A_ptr_2:*struct A = 0;

struct A {
	var a:int64;
	var b:int64;
}
var g_A:struct A;

struct B {
	var c:struct A;
	var d:*struct A;
}

function func_a() dummy
function func_b(a:int64, b:int64):int64 dummy
function func_c(a:int64, ...):int64 dummy

function func_add(n1:int64, n2:int64):int64 {

}

function func_test() {
	var a:int64, b:int64 = 1;
	var c:*char = "abc";
	var d:*char = "def" + 2;
	var e:struct B;
	var f:int64 = e.d->b;
	var g:[2][3]int64;
	var h:int64 = g[1][2];
	var i:int64 = func_add(1, 2);
	var j:*function(n1:int64, n2:int64):int64 = 0;
	var k:int64 = j(3, 4);
}
