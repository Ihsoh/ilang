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


function main():int32 {

	print_int8("print_int8 1 = ", 123);
	print_int16("print_int16 1 = ", 12345);
	print_int32("print_int32 1 = ", 1234567890);
	print_int64("print_int64 1 = ", 123456789012);

	print_int8("print_int8 2 = ", -123);
	print_int16("print_int16 2 = ", -12345);
	print_int32("print_int32 2 = ", -1234567890);
	print_int64("print_int64 2 = ", -123456789012);

	print_uint8("print_uint8 = ", 123);
	print_uint16("print_uint16 = ", 12345);
	print_uint32("print_uint32 = ", 1234567890);
	print_uint64("print_uint64 = ", 123456789012);

	print_float("print_float = ", 123.456f);

	print_double("print_double = ", 654.321);

	print_pointer("print_pointer = ", cast<*>(0x12345678));

	print_string("print_string = TEST STRING");

	int64 a_1 = 12345678900;
	int64 a_2 = 1000;
	int64 a_3 = a_1 * a_2;
	print_int64("12345678900 * 1000 = ", a_3);

	int64 b_1 = 12345678900;
	int64 b_2 = 1000;
	int64 b_3 = b_1 / b_2;
	print_int64("12345678900 / 1000 = ", b_3);

	int64 c_1 = 12345678900;
	int64 c_2 = 3;
	int64 c_3 = c_1 % c_2;
	print_int64("12345678900 %% 3 = ", c_3);

	return 0;
}
