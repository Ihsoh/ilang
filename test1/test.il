function print_int8(str:*char, n:int8) dummy
function print_int16(str:*char, n:int16) dummy
function print_int32(str:*char, n:int32) dummy
function print_int64(str:*char, n:int64) dummy
function print_uint8(str:*char, n:uint8) dummy
function print_uint16(str:*char, n:uint16) dummy
function print_uint32(str:*char, n:uint32) dummy
function print_uint64(str:*char, n:uint64) dummy
function print_float(str:*char, n:float) dummy
function print_double(str:*char, n:double) dummy
function print_pointer(str:*char, ptr:*) dummy

function func_a():int64 {
	var v:int64 = 1000;
	print_int64("a = ", v);
	return 0;
}

function func_b():int64 {
	var v:int64 = 2000;
	print_int64("b = ", v);
	return 1;
}

function func_test() {
	var a:int64= 100;
	var b:int32 = 300;
	var c:int64 = 2333;
	c = func_a() || func_b();
	c += 2;

	print_int64("c = ", c);
}

function main():int32 {
	func_test();
	return 0;
}
