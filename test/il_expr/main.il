#include <testlib.ilh>

var global_expr_atom_uint:int = 1;
var global_expr_atom_real:float = 1.23;
var global_expr_atom_char:char = 'a';
var global_expr_atom_string:*char = "abc";

var global_constexpr_1:int = 1 + 2 * 3;
var global_constexpr_2:int = (1 + 2) * 3;
var global_constexpr_3:float = 2.0 + 2.0;
var global_constexpr_4:double = 2.0 + 2.0;
var global_constexpr_5:*char = "abcdef" + 1;
var global_constexpr_6:*char = "abcdef" + 3 - 1;

function test_atom() {
	var expr_atom_uint:int = 1;
	var expr_atom_real:float = 1.23;
	var expr_atom_char:char = 'a';
	var expr_atom_string:*char = "abc";
	var expr_atom_id:int = expr_atom_uint;
}

function test_parenthesis() {
	var expr_paren_1:int = (1);
	var expr_paren_2:int = (1 + 1);

	var expr_paren_3:int = 1 + 2 * 3;
	if (expr_paren_3 != 7) {
		tst_exit(1);
	}
	
	var expr_paren_4:int = (1 + 2) * 3;
	if (expr_paren_4 != 9) {
		tst_exit(1);
	}

	var a:int = 1;
	var b:int = 2;
	var c:int = 3;
	var expr_paren_5:int = (a + b) * c;
	if (expr_paren_5 != 9) {
		tst_exit(1);
	}

	//					   1 + ((1 + 2) * (1 + 2) + (1 + 2) * (1 + 2)) * (1 + 2) + 2 + 2
	var expr_paren_6:int = a + ((a + b) * (a + b) + (a + b) * (a + b)) * (a + b) + b + b;
	if (expr_paren_6 != 59) {
		tst_exit(1);
	}
}

struct A {
	var a:int;
	var b:int;
}

struct B {
	var x:int;
	var y:struct A;
	var z:*struct A;
}

function test_member() {
	var a:struct A;
	a.a = 1;
	a.b = 2;
	if ((a.a + a.b + 1) * (a.a + a.b + 2) != 20) {
		tst_exit(1);
	}

	var b:struct B;
	b.x = 3;
	b.y.a = 4;
	b.y.b = 5;
	b.z = &a;
	if (b.x + b.y.a + b.y.b + b.z->a + b.z->b != 15) {
		tst_exit(1);
	}
}

function test_arridx() {
	var a:[10]int;
	var b:[10][10]int;
	
	for (var i:int = 0; i < 10; ++i) {
		a[i] = 1 + i;
	}
	var total1:int = 0;
	for (var i:int = 0; i < 10; ++i) {
		total1 += a[i];
	}
	if (total1 != 55) {
		tst_exit(1);
	}

	for (var i:int = 0; i < 10; ++i) {
		for (var j:int = 0; j < 10; ++j) {
			b[i][j] = i * 10 + (1 + j);
		}
	}
	var total2:int = 0;
	for (var i:int = 0; i < 10; ++i) {
		for (var j:int = 0; j < 10; ++j) {
			total2 += b[i][j];
		}
	}
	if (total2 != 5050) {
		tst_exit(1);
	}
}

function foo() {
}

function add(a:int, b:int):int {
	return a + b;
}

function mul(a:int, b:int):int {
	return a * b;
}

function test_func_call() {
	foo();

	var result1:int = 1 + mul(add(1, 2), add(1, 2));
	if (result1 != 10) {
		tst_exit(1);
	}

	var add_ptr:*function(a:int, b:int):int = add;
	var mul_ptr:*function(a:int, b:int):int = mul;
	var result2:int = 1 + mul_ptr(add_ptr(2, 3), add_ptr(2, 3));
	if (result2 != 26) {
		tst_exit(1);
	}
}

struct Point2D {
	var x:int, y:int;
}

function test_ref_and_deref() {

	#define	_A(type, val)	\
	{	\
		var val_##type:type = 0;	\
		*&val_##type = val;	\
		assert(val_##type == val);	\
		var ptr_##type:*type = &val_##type;	\
		*ptr_##type = val * 10;	\
		assert(val_##type == val * 10);	\
	}

	_A(char, 1)
	_A(int8, 2)
	_A(int16, 3)
	_A(int32, 4)
	_A(int64, 5)
	_A(uint8, 6)
	_A(uint16, 7)
	_A(uint32, 8)
	_A(uint64, 9)
	_A(float, 8)
	_A(double, 9)

	{
		var val_struct_Point2D:struct Point2D;
		val_struct_Point2D.x = 0;
		val_struct_Point2D.y = 0;
		var ptr_struct_Point2D:*struct Point2D = &val_struct_Point2D;
		ptr_struct_Point2D->x = 1;
		ptr_struct_Point2D->y = 2;
		assert(val_struct_Point2D.x == 1);
		assert(val_struct_Point2D.y == 2);
		(*ptr_struct_Point2D).x = 10;
		(*ptr_struct_Point2D).y = 20;
		assert(val_struct_Point2D.x == 10);
		assert(val_struct_Point2D.y == 20);
	}

	{
		var val_arr_10_int:[10]int;
		for (var i:int = 0; i < sizeof(val_arr_10_int) / sizeof(int); ++i) {
			val_arr_10_int[i] = i;
		}
		var ptr_arr_10_int:*[10]int = &val_arr_10_int;
		for (var i:int = 0; i < sizeof(val_arr_10_int) / sizeof(int); ++i) {
			(*ptr_arr_10_int)[i] *= 10;
		}
		for (var i:int = 0; i < sizeof(val_arr_10_int) / sizeof(int); ++i) {
			assert(val_arr_10_int[i] == i * 10);
		}
		var ptr_int:*int = &(val_arr_10_int[0]);
		for (var i:int = 0; i < sizeof(val_arr_10_int) / sizeof(int); ++i) {
			ptr_int[i] *= 20;
		}
		for (var i:int = 0; i < sizeof(val_arr_10_int) / sizeof(int); ++i) {
			assert(val_arr_10_int[i] == i * 10 * 20);
		}
	}

	{
		var val_arr_20_10_int:[20][10]int;
		for (var i:int = 0; i < sizeof(val_arr_20_10_int) / sizeof([10]int); ++i) {
			for (var j:int = 0; j < sizeof(val_arr_20_10_int[i]) / sizeof(int); ++j) {
				val_arr_20_10_int[i][j] = i * j;
			}
		}
		var ptr_arr_20_10_int:*[20][10]int = &val_arr_20_10_int;
		for (var i:int = 0; i < sizeof(*ptr_arr_20_10_int) / sizeof([10]int); ++i) {
			for (var j:int = 0; j < sizeof((*ptr_arr_20_10_int)[i]) / sizeof(int); ++j) {
				(*ptr_arr_20_10_int)[i][j] *= 30;
			}
		}
		for (var i:int = 0; i < sizeof(val_arr_20_10_int) / sizeof([10]int); ++i) {
			var ptr_arr_10_int:*[10]int = &(val_arr_20_10_int[i]);
			for (var j:int = 0; j < sizeof(*ptr_arr_10_int) / sizeof(int); ++j) {
				var ptr_int:*int = &((*ptr_arr_10_int)[j]);
				*ptr_int *= 40;
			}
		}
		for (var i:int = 0; i < sizeof(val_arr_20_10_int) / sizeof([10]int); ++i) {
			for (var j:int = 0; j < sizeof(val_arr_20_10_int[i]) / sizeof(int); ++j) {
				assert(val_arr_20_10_int[i][j] == i * j * 30 * 40);
			}
		}
	}

	{
		var val_a:int = 0;
		var ptr_a:*int = &val_a;
		var ptr_b:**int = &ptr_a;
		var ptr_c:***int = &ptr_b;
		***ptr_c = 123;
		assert(val_a == 123);
	}

	#undef	_A
}

function test_cast() {
	var val_char:char = 1;
	var val_int8:int8 = 2;
	var val_int16:int16 = 3;
	var val_int32:int32 = 4;
	var val_int64:int64 = 5;
	var val_uint8:uint8 = 6;
	var val_uint16:uint16 = 7;
	var val_uint32:uint32 = 8;
	var val_uint64:uint64 = 9;
	var val_float:float = 10.0;
	var val_double:double = 11.0;

	#define	_A(type_target, type_source, value)	\
	{	\
		var _tmp:type_target = cast<type_target>(val_##type_source);	\
		assert(_tmp == (value));	\
	}

	#define	_B(type_source, value)	\
	{	\
		_A(char, type_source, value)	\
		_A(int8, type_source, value)	\
		_A(int16, type_source, value)	\
		_A(int32, type_source, value)	\
		_A(int64, type_source, value)	\
		_A(uint8, type_source, value)	\
		_A(uint16, type_source, value)	\
		_A(uint32, type_source, value)	\
		_A(uint64, type_source, value)	\
		_A(float, type_source, value)	\
		_A(double, type_source, value)	\
	}
	
	_B(char, 1)
	_B(int8, 2)
	_B(int16, 3)
	_B(int32, 4)
	_B(int64, 5)
	_B(uint8, 6)
	_B(uint16, 7)
	_B(uint32, 8)
	_B(uint64, 9)
	_B(float, 10)
	_B(double, 11)

	var ptr_void:* = 123;
	var ptr_int:*int = cast<*int>(ptr_void);
	assert(ptr_int == cast<*int>(123));

	#undef	_A
	#undef	_B
}

struct NotPackedStruct_1 {
	var a:int8;
	var b:int32;
}

struct NotPackedStruct_2 {
	var a:int8;
	var b:int32;
	var c:int8;
}

packed struct PackedStruct_1 {
	var a:int8;
	var b:int32;
}

packed struct PackedStruct_2 {
	var a:int8;
	var b:int32;
	var c:int8;
}

function test_unary() {
	var val_not_1:int = !0;
	if (val_not_1 != 1) {
		tst_exit(1);
	}
	var val_not_2:int = !1;
	if (val_not_2 != 0) {
		tst_exit(1);
	}

	var val_bnot_1:uint8 = ~0x0f;
	if (val_bnot_1 != 0xf0) {
		tst_exit(1);
	}

	var val_positive_1:int = +1;
	if (val_positive_1 != 1) {
		tst_exit(1);
	}

	var val_negative_1:int = -1;
	if (val_negative_1 != 0 - 1) {
		tst_exit(1);
	}

	var val_inc_left_1:int = 0;
	if (++val_inc_left_1 != 1) {
		tst_exit(1);
	}
	if (val_inc_left_1 != 1) {
		tst_exit(1);
	}

	var val_dec_left_1:int = 0;
	if (--val_dec_left_1 != -1) {
		tst_exit(1);
	}
	if (val_dec_left_1 != -1) {
		tst_exit(1);
	}

	var val_inc_right_1:int = 0;
	if (val_inc_right_1++ != 0) {
		tst_exit(1);
	}
	if (val_inc_right_1 != 1) {
		tst_exit(1);
	}

	var val_dec_right_1:int = 0;
	if (val_dec_right_1-- != 0) {
		tst_exit(1);
	}
	if (val_dec_right_1 != -1) {
		tst_exit(1);
	}

	assert(sizeof(char) == 1);
	assert(sizeof(int8) == 1);
	assert(sizeof(int16) == 2);
	assert(sizeof(int32) == 4);
	assert(sizeof(int64) == 8);
	assert(sizeof(uint8) == 1);
	assert(sizeof(uint16) == 2);
	assert(sizeof(uint32) == 4);
	assert(sizeof(uint64) == 8);
	assert(sizeof(float) == 4);
	assert(sizeof(double) == 8);
	assert(sizeof(struct Point2D) == sizeof(int) + sizeof(int));
	
	assert(sizeof(struct NotPackedStruct_1) == sizeof(int8) + 3 + sizeof(int32));
	assert(sizeof(struct NotPackedStruct_2) == sizeof(int8) + 3 + sizeof(int32) + sizeof(int8) + 3);

	assert(sizeof(struct PackedStruct_1) == sizeof(int8) + sizeof(int32));
	assert(sizeof(struct PackedStruct_2) == sizeof(int8) + sizeof(int32) + sizeof(int8));

	assert(sizeof([10]int) == 10 * sizeof(int));
	assert(sizeof([20][10]int) == 20 * 10 * sizeof(int));
	assert(sizeof([30][20][10]int) == 30 * 20 * 10 * sizeof(int));
	assert(sizeof([30][20][10]struct Point2D) == 30 * 20 * 10 * (sizeof(int) + sizeof(int)));
	assert(sizeof(function(a:int, b:int)) == 0);
	#if streq(__ARCH__, "32")
		assert(sizeof(*) == 4);
		assert(sizeof(*char) == 4);
	#elif streq(__ARCH__, "64")
		assert(sizeof(*) == 8);
		assert(sizeof(*char) == 8);
	#else
		#error unsupported architecture!
	#endif
	assert(sizeof(foo()) == 1);

	test_ref_and_deref();

	test_cast();
}

function test_mul_div_mod() {
	{
		var a:int = 2;
		var b:int = 3;
		assert(a * b == 2 * 3);
		var c:float = 2.0;
		var d:float = 3.0;
		assert(c * d == 2.0 * 3.0);
		var e:double = 2.0;
		var f:double = 3.0;
		assert(e * f == 2.0 * 3.0);
	}
	{
		var a:int = 6;
		var b:int = 3;
		assert(a / b == 6 / 3);
		var c:float = 6.0;
		var d:float = 3.0;
		assert(c / d == 6.0 / 3.0);
		var e:double = 6.0;
		var f:double = 3.0;
		assert(e / f == 6.0 / 3.0);
	}
	{
		var a:int = 3;
		var b:int = 2;
		assert(a % b == 3 % 2);
	}
}

function test_add_sub() {
	{
		var a:int = 2;
		var b:int = 3;
		assert(a + b == 2 + 3);
		var c:float = 2.0;
		var d:float = 3.0;
		assert(c + d == 2.0 + 3.0);
		var e:double = 2.0;
		var f:double = 3.0;
		assert(e + f == 2.0 + 3.0);
		var str:*char = "abcdef";
		str = str + 2;
		assert(tst_strcmp(str, "cdef") == 0);
		var arr_3_int:[3]int;
		arr_3_int[0] = 100;
		arr_3_int[1] = 200;
		arr_3_int[2] = 300;
		var ptr_int:*int = &(arr_3_int[0]);
		ptr_int = ptr_int + 2;
		assert(*ptr_int == 300);
	}
	{
		var a:int = 3;
		var b:int = 2;
		assert(a - b == 3 - 2);
		var c:float = 3.0;
		var d:float = 2.0;
		assert(c - d == 3.0 - 2.0);
		var e:double = 3.0;
		var f:double = 2.0;
		assert(e - f == 3.0 - 2.0);
		var str:*char = "abcdef" + 2;
		str = str - 1;
		assert(tst_strcmp(str, "bcdef") == 0);
		var arr_3_int:[3]int;
		arr_3_int[0] = 100;
		arr_3_int[1] = 200;
		arr_3_int[2] = 300;
		var ptr_int:*int = &(arr_3_int[0]) + 2;
		ptr_int = ptr_int - 1;
		assert(*ptr_int == 200);
	}
}

function test_shift() {
	{
		var a:int = 1;
		assert(a << 2 == 1 << 2);
		var b:int = 2;
		assert(a << b == 1 << 2);
	}
	{
		var a:int = 4;
		assert(a >> 1 == 4 >> 1);
		var b:int = 1;
		assert(a >> b == 4 >> 1);
	}
}

function test_comparison() {
	{
		{
			var a:int = 2;
			var b:int = 3;
			assert(a < b);
			assert(2 < 3);
			assert(2 < b);
			assert(a < 3);
		}
		{
			var a:float = 2.0;
			var b:float = 3.0;
			assert(a < b);
			assert(2.0 < 3.0);
			assert(2.0 < b);
			assert(a < 3.0);
		}
		{
			var a:double = 2.0;
			var b:double = 3.0;
			assert(a < b);
			assert(2.0 < 3.0);
			assert(2.0 < b);
			assert(a < 3.0);
		}
	}

	{
		{
			var a:int = 2;
			var b:int = 3;
			assert(a <= b);
			assert(2 <= 3);
			assert(2 <= b);
			assert(a <= 3);
		}
		{
			var a:float = 2.0;
			var b:float = 3.0;
			assert(a <= b);
			assert(2.0 <= 3.0);
			assert(2.0 <= b);
			assert(a <= 3.0);
		}
		{
			var a:double = 2.0;
			var b:double = 3.0;
			assert(a <= b);
			assert(2.0 <= 3.0);
			assert(2.0 <= b);
			assert(a <= 3.0);
		}
	}

	{
		{
			var a:int = 3;
			var b:int = 2;
			assert(a > b);
			assert(3 > 2);
			assert(3 > b);
			assert(a > 2);
		}
		{
			var a:float = 3.0;
			var b:float = 2.0;
			assert(a > b);
			assert(3.0 > 2.0);
			assert(3.0 > b);
			assert(a > 2.0);
		}
		{
			var a:double = 3.0;
			var b:double = 2.0;
			assert(a > b);
			assert(3.0 > 2.0);
			assert(3.0 > b);
			assert(a > 2.0);
		}
	}

	{
		{
			var a:int = 3;
			var b:int = 2;
			assert(a >= b);
			assert(3 >= 2);
			assert(3 >= b);
			assert(a >= 2);
		}
		{
			var a:float = 3.0;
			var b:float = 2.0;
			assert(a >= b);
			assert(3.0 >= 2.0);
			assert(3.0 >= b);
			assert(a >= 2.0);
		}
		{
			var a:double = 3.0;
			var b:double = 2.0;
			assert(a >= b);
			assert(3.0 >= 2.0);
			assert(3.0 >= b);
			assert(a >= 2.0);
		}
	}

	{
		{
			var a:int = 2;
			var b:int = 2;
			assert(a == b);
			assert(2 == 2);
			assert(2 == b);
			assert(a == 2);
		}
		{
			var a:float = 2.0;
			var b:float = 2.0;
			assert(a == b);
			assert(2.0 == 2.0);
			assert(2.0 == b);
			assert(a == 2.0);
		}
		{
			var a:double = 2.0;
			var b:double = 2.0;
			assert(a == b);
			assert(2.0 == 2.0);
			assert(2.0 == b);
			assert(a == 2.0);
		}
		{
			var a:* = 2;
			var b:* = 2;
			assert(a == b);
			assert(cast<*>(2) == cast<*>(2));
			assert(cast<*>(2) == b);
			assert(a == cast<*>(2));
		}
	}

	{
		{
			var a:int = 2;
			var b:int = 3;
			assert(a != b);
			assert(2 != 3);
			assert(2 != b);
			assert(a != 3);
		}
		{
			var a:float = 2.0;
			var b:float = 3.0;
			assert(a != b);
			assert(2.0 != 3.0);
			assert(2.0 != b);
			assert(a != 3.0);
		}
		{
			var a:double = 2.0;
			var b:double = 3.0;
			assert(a != b);
			assert(2.0 != 3.0);
			assert(2.0 != b);
			assert(a != 3.0);
		}
		{
			var a:* = 2;
			var b:* = 3;
			assert(a != b);
			assert(cast<*>(2) != cast<*>(3));
			assert(cast<*>(2) != b);
			assert(a != cast<*>(3));
		}
	}
}

function __test_func_a():int {
}
function __test_func_b(a:int):int {
}
function __test_func_c(a:int, b:int) {
}
function __test_func_d(a:int, b:int, ...) {
}

function test_instanceof() {
	assert(cast<char>(0) instanceof char);

	assert(cast<int8>(0) instanceof int8);
	assert(cast<int16>(0) instanceof int16);
	assert(cast<int32>(0) instanceof int32);
	assert(cast<int64>(0) instanceof int64);

	assert(cast<uint8>(0) instanceof uint8);
	assert(cast<uint16>(0) instanceof uint16);
	assert(cast<uint32>(0) instanceof uint32);
	assert(cast<uint64>(0) instanceof uint64);

	assert(cast<float>(0) instanceof float);
	assert(cast<double>(0) instanceof double);

	{
		var a:__va_list;
		assert(a instanceof __va_list);
	}

	{
		var a:struct A;
		assert(a instanceof struct A);
	}

	{
		var a:[3]int;
		assert(a instanceof [3]int);

		var b:[3][4]int;
		assert(b instanceof [3][4]int);

		var c:[3][4][5]int;
		assert(c instanceof [3][4][5]int);

		var d:[3][4][5]*;
		assert(d instanceof [3][4][5]*);

		var e:[3][4][5]*int;
		assert(e instanceof [3][4][5]*int);

		var f:[3][4][5]*struct A;
		assert(f instanceof [3][4][5]*struct A);
	}

	{
		assert(__test_func_a instanceof *function():int);
		assert(__test_func_b instanceof *function(a:int):int);
		assert(__test_func_c instanceof *function(a:int, b:int));
		assert(__test_func_d instanceof *function(a:int, b:int, ...));
	}

	assert(cast<*>(0) instanceof *);
	assert(cast<**>(0) instanceof **);
	assert(cast<*int>(0) instanceof *int);
	assert(cast<**int>(0) instanceof **int);
}

function test_bitwise() {
	{
		var a:int = 0b0111;
		var b:int = 0b0100;
		assert((a & b) == (0b0111 & 0b0100));
		assert((0b0111 & b) == (0b0111 & 0b0100));
		assert((a & 0b0100) == (0b0111 & 0b0100));
	}
	{
		var a:int = 0b0101;
		var b:int = 0b0110;
		assert((a ^ b) == (0b0101 ^ 0b0110));
		assert((0b0101 ^ b) == (0b0101 ^ 0b0110));
		assert((a ^ 0b0110) == (0b0101 ^ 0b0110));
	}
	{
		var a:int = 0b0100;
		var b:int = 0b0011;
		assert((a | b) == (0b0100 | 0b0011));
		assert((0b0100 | b) == (0b0100 | 0b0011));
		assert((a | 0b0011) == (0b0100 | 0b0011));
	}
}

function test_logic() {
	{
		var a:int = 1;
		var b:int = 0;
		assert(a && a);
		assert(1 && a);
		assert(a && 1);
		assert(1 && 1);
		assert(!(a && b));
		assert(!(1 && b));
		assert(!(a && 0));
		assert(!(1 && 0));
	}
	{
		var a:int = 1;
		var b:int = 0;
		assert(a || b);
		assert(1 || b);
		assert(a || 0);
		assert(1 || 0);
	}
}

function test_cond() {
	{
		var a:int = 1 ? 123 : 456;
		assert(a == 123);

		var b:int = 0 ? 123 : 456;
		assert(b == 456);

		var c:int = 1 ? (0 ? 111 : 222) : (0 ? 333 : 444);
		assert(c == 222);

		var d:int = 1 ? 0 ? 111 : 222 : 0 ? 333 : 444;
		assert(d == 222);

		var e:*int = 1 ? cast<*int>(123) : cast<*int>(456);
		assert(e == cast<*int>(123));

		var f:*int = 0 ? cast<*int>(123) : cast<*int>(456);
		assert(f == cast<*int>(456));

		var g:*int = cast<*int>(1) ? cast<*int>(123) : cast<*int>(456);
		assert(g == cast<*int>(123));

		var h:*int = cast<*int>(0) ? cast<*int>(123) : cast<*int>(456);
		assert(h == cast<*int>(456));
	}
	{
		var x0:int = 0, x1:int = 1;
		var x123:int = 123, x456:int = 456;
		var x111:int = 111, x222:int = 222, x333:int = 333, x444:int = 444;

		var a:int = x1 ? x123 : x456;
		assert(a == 123);

		var b:int = x0 ? x123 : x456;
		assert(b == 456);

		var c:int = x1 ? (x0 ? x111 : x222) : (x0 ? x333 : x444);
		assert(c == 222);

		var d:int = x1 ? x0 ? x111 : x222 : x0 ? x333 : x444;
		assert(d == 222);

		var e:*int = x1 ? cast<*int>(x123) : cast<*int>(x456);
		assert(e == cast<*int>(123));

		var f:*int = x0 ? cast<*int>(x123) : cast<*int>(x456);
		assert(f == cast<*int>(456));

		var g:*int = cast<*int>(x1) ? cast<*int>(x123) : cast<*int>(x456);
		assert(g == cast<*int>(123));

		var h:*int = cast<*int>(x0) ? cast<*int>(x123) : cast<*int>(x456);
		assert(h == cast<*int>(456));
	}
}

function test_assign() {
	{
		var a:int = 0, b:int = 0;
		a = b = 123;
		assert(a == 123 && b == 123);
	}
	{
		var a:int = 0, b:int = 0;
		a = (b = 123);
		assert(a == 123 && b == 123);
	}
	{
		var a:int;
		a = 2;
		assert(a == 2);
		a += 2;
		assert(a == 4);
		a -= 1;
		assert(a == 3);
		a *= 2;
		assert(a == 6);
		a /= 3;
		assert(a == 2);
		a %= 3;
		assert(a == 2);
		a &= 3;
		assert(a == 2);
		a |= 1;
		assert(a == 3);
		a ^= 2;
		assert(a == 1);
		a <<= 2;
		assert(a == 4);
		a >>= 1;
		assert(a == 2);
	}
	{
		var a:float;
		a = 2.0;
		assert(a == 2.0);
		a += 2.0;
		assert(a == 4.0);
		a -= 1.0;
		assert(a == 3.0);
		a *= 2.0;
		assert(a == 6.0);
		a /= 3.0;
		assert(a == 2.0);
	}
	{
		var a:double;
		a = 2.0;
		assert(a == 2.0);
		a += 2.0;
		assert(a == 4.0);
		a -= 1.0;
		assert(a == 3.0);
		a *= 2.0;
		assert(a == 6.0);
		a /= 3.0;
		assert(a == 2.0);
	}
	{
		var a:*int;
		a = cast<*int>(2);
		assert(a == cast<*int>(2));
		a += 2;
		assert(a == cast<*int>(2 + 2 * sizeof(int)));
		a -= 1;
		assert(a == cast<*int>(2 + 1 * sizeof(int)));
	}
}

function main(argc:int, argv:**char):int {
	test_atom();
	test_parenthesis();
	test_member();
	test_arridx();
	test_func_call();
	test_unary();
	test_mul_div_mod();
	test_add_sub();
	test_shift();
	test_comparison();
	test_instanceof();
	test_bitwise();
	test_logic();
	test_cond();
	test_assign();

	return 0;
}
