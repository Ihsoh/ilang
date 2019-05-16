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
function print_string(str:*char) dummy

function func_test(n:int64, ...) {
	var a:__va_list;
	var b:__va_list;

	__va_start(a);

	__va_copy(b, a);

	for (var i:int64 = 0; i < n; i++) {
		print_int64("a = ", __va_arg(a, int64));
	}

	for (var i:int64 = 0; i < n; i++) {
		print_int64("b = ", __va_arg(b, int64));
	}

	__va_end(a);
}

function main():int32 {
	func_test(2, 10, 20);
	return 0;
}
