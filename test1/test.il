#include "a.ilh" // asd
#include "b.ilh" /* zxc */
#include "c/c.ilh"

// asd
/* zxc */

	// asd
	/* zxc */

// ""
// ''
//

function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function printf(fmt:*char, ...) dummy

#define	MACRO_A		1234
#define	MACRO_B		MACRO_A
#define	MACRO_C		MACRO_B

var xxA:int32 = MACRO_C;

#undef	MACRO_A
#define	MACRO_A		4321

var xxB:int32 = MACRO_C;

#ifdef	MACRO_A
var xA:int32 = 1;
#else
var xA:int32 = 2;
#endif

#ifdef	MACRO_B
var xB1:int32 = 1;
#else
var xB1:int32 = 2;
#endif

#ifndef	MACRO_B
var xB2:int32 = 1;
#else
var xB2:int32 = 2;
#endif

#ifdef	MACRO_C
var xC:int32 = 1;
#else
var xC:int32 = 2;
#endif



function getFunc1():*function(a1:int32, b1:int32) dummy
function getFunc():*function(a:int32, b:int32) dummy

struct Person dummy

struct A {
	var a:*struct Person;
}

struct Person {
	var name:*char;
	var age:uint32;
	var a:int32;
	var b:int32;
}

function funcA(a:int32, b:int32) dummy

/*
function funcB() {
	// abc def // 233
	//
	//
	funcA(1, 2);//x // 2333
}

function funcA(a:int32, b:int32) {
}*/

function printPerson(person:*struct Person) {
	printf("Name: %s, Age: %u\n", person->name, person->age);
}

#ifdef	MACRO_A
	#define	XA	"XA"
	#ifdef	__NOT_EXISTS__
		#define	XB	"XB"
	#else
		#define	XB	"wow~~"
	#endif
	#define	XC	"XC"
#endif

#define	VAL_A	30
#define	VAL_B	40
#define	VAL		VAL_A##VAL_B
#define	STR_VAL	#VAL

#define	ADD(a, b)			((a) + (b))
#define	SUM(result, a, b)	for (var i:int32 = (a); i <= (b); ++i) { result += i; }

var a1:int32 = 123;
var a2:int32 = 123;
var a3:int32 = -~0xfffffffd;

function main(argc:int32, argv:**char):int32 {
	var b1:int32 = 123;
	var b2:int32 = b1;

	printf("a3 = %d\n", a3);

	var dval:double = 1_2_3.4_5_6_7_8;
	printf("dval = %lf\n", dval);

	#if 0
		printf("#if %s\n", "a");
	#elif defined(VAL_A) && defined(VAL_B)
		printf("#if %s\n", "b");
	#elif 1
		printf("#if %s\n", "c");
	#else
		printf("#if %s\n", "d");
	#endif

	printf("VAL = %d\n", VAL);
	printf("STR_VAL = %s\n", STR_VAL);
	printf("STR_VAL1 = %s\n", #VAL);

	printf("ADD(1, 2) = %d\n", ADD(1, 2));

	var result:int32 = 0;
	SUM(result, 1, 100)
	printf("SUM(1, 100) = %d\n", result);

	printf("Test: %s, %s, %s\n", XA, XB, XC);



	return 0;
	
	var arr_01:[1][1]char;
	var arr_02:[1][1]char;

	arr_01[0][0] = arr_02[0][0];
	// arr_01[0] = arr_02[0];

	var p1:struct Person, p2:struct Person;
	p1 = p2;

	var person:*struct Person = cast<*struct Person>(malloc(sizeof(struct Person)));
	person->name = "Test";
	person->age = 2333;
	printPerson(person);
	free(person);
	
	var matrix:[2][2]int32;
	matrix[0][0] = 1;
#line 100 "hello.il"
#line 1000 "hello2333.il"
	var i:int32 = 123, i1:int32 = 0;
	var iptr:*int32;

	iptr = &i;
	i1 = *cast<*int32>(cast<*>(iptr));

	printf("%d\n", ++i1);

	printf("%f\n", 2.0 * 2);

	var person1:*struct Person = cast<*struct Person>(0);
	var abc:int32;
	person1 = person1 + -1;
	abc += 2;

	if (1) {

	} elif (1) {

	} elif (2) {

	} else {

	}

	while (1) {
		break;
		continue;
	}

	do {
		break;
		continue;
	} while (1);

	for (var x:int32 = 0; x < 100; ++x) {
		x = 123;
		break;
		continue;
	}

	var str:*char = "wahahaha";

	return 0;
}