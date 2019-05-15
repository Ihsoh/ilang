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

function func_test() {
	var a:int64 = 1;
	var b:int64 = 4;
	var c:int64 = (a << b) >> 1;

	print_int64("c = ", c);
}

function main():int32 {
	func_test();
	return 0;
}
