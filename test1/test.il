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

#define	byte	int8
#define	ubyte	uint8
#define	short	int16
#define	ushort	uint16

#define	size_t uint32
#define	ssize_t int32

#define	int int32
#define	uint uint32
#define	long int32
#define	ulong uint32

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

function main():int32 {
	func_1();

	if (func_2() != 123) {
		return 1;
	}

	if (func_3(1, 2) != 3) {
		return 1;
	}

	func_4(6, 1, 2, 3, 4, 5, 6);

	// var func_1_ptr:*function() = func_1;
	// func_1_ptr();

	// var func_2_ptr:*function():int = func_2;
	// if (func_2_ptr() != 123) {
	// 	return 1;
	// }

	// var func_3_ptr:*function(a:int, b:int):int = func_3;
	// if (func_3_ptr(2, 3) != 5) {
	// 	return 1;
	// }

	// var func_4_ptr:*function(n:int, ...):int = func_4;
	// func_4_ptr(6, 1, 2, 3, 4, 5, 6);

	return 0;
}
