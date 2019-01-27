function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function printf(fmt:*char, ...) dummy

struct A {
	var a:char;
	var b:int32;
	var c:char;
}

function returnVoid() {
}

var str1:*char = cast<*char>(cast<*int32>(1 + "abcdefgh" + 1) + 1);

function main(argc:int32, argv:**char):int32 {
	var sz_int32:uint32 = sizeof(int32);
	printf("sz_int32 = %u\n", sz_int32);

	var sz_struct_A:uint32 = sizeof(struct A);
	printf("sz_struct_A = %u\n", sz_struct_A);

	var a:int32 = - - -cast<int32>(sizeof(struct A));
	printf("a = %d\n", a);

	var b:int32 = -(10 + 5 % 3 * 3);
	printf("b = %d\n", b);

	var c:int32 = 10 + (1 << 2);
	printf("c = %d\n", c);

	var d:*int32 = cast<*int32>(10) + 1;
	printf("d = %u\n", d);

	var e:*int32 = cast<*int32>(10) - 1;
	printf("e = %u\n", e);

	var f:*int32 = 2 + cast<*int32>(10);
	printf("f = %u\n", f);

	// var g:*int32 = 2 - cast<*int32>(10);
	// printf("g = %u\n", g);

	var h:int32 = 100 > 2;
	printf("h = %d\n", h);

	var i:int32 = 100 < 2;
	printf("i = %d\n", i);

	// var j:int32 = 100 < returnVoid();
	// printf("j = %u\n", j);

	var k:int32 = 4 + 2 == 2 * 3;
	printf("k = %d\n", k);

	var l:int32 = cast<*int32>(100) + 1 == cast<*int32>(104);
	printf("l = %d\n", l);

	var m:uint32 = 0b1110 & 0b0101;
	printf("m = %u\n", m);

	printf("1 + 2 * 3 = %d\n", 1 + 2 * 3);

	var n:uint32 = 1 + 2 + m + 3 * 4;
	printf("1 + 2 + m + 3 * 4 = %d\n", n);

	printf("str1 = %s\n", str1);

	printf("str1 = %s\n", cast<*char>(cast<*int32>(1 + "abcdefghij" + 1 * 2) + 1));

	return 0;
}