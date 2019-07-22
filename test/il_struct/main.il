#include <testlib.ilh>

struct A {
}

struct B {
	var x:int32, y:int32;
}

struct C {
	var a:char;
	var b:int8;
	var c:int16;
	var d:int32;
	var e:int64;
	var f:uint8;
	var g:uint16;
	var h:uint32;
	var i:uint64;
	var j:float;
	var k:double;

	var l:struct B;

	var m:[3]char;
	var n:[3][4]char;

	var o:*function();
	var p:*function(a:int32, b:int32):int32;

	var q:*;
	var r:*char;
	var s:*struct B;
}

function func_1() {
}

function func_2(a:int32, b:int32):int32 {
	return 0;
}

function main(argc:int, argv:**char):int {
	var a:struct A;




	var b:struct B;
	b.x = 1;
	b.y = 2;




	var c:struct C;
	c.a = 'a';
	c.b = 1;
	c.c = 2;
	c.d = 3;
	c.e = 4;
	c.f = 5;
	c.g = 6;
	c.h = 7;
	c.i = 8;
	c.j = 1.1;
	c.k = 1.2;
	
	c.l.x = 1;
	c.l.y = 2;

	c.m[0] = 'a';
	c.m[1] = 'b';
	c.m[2] = 'c';
	c.n[0][0] = 'a';
	c.n[0][1] = 'b';
	c.n[1][0] = 'c';

	c.o = func_1;
	c.p = func_2;

	c.q = cast<*>(0);
	c.r = "123";
	c.s = cast<*struct B>(0);




	var d:*struct C = &c;
	d->a = 'a';
	d->b = 1;
	d->c = 2;
	d->d = 3;
	d->e = 4;
	d->f = 5;
	d->g = 6;
	d->h = 7;
	d->i = 8;
	d->j = 1.1;
	d->k = 1.2;
	
	d->l.x = 1;
	d->l.y = 2;

	d->m[0] = 'a';
	d->m[1] = 'b';
	d->m[2] = 'c';
	d->n[0][0] = 'a';
	d->n[0][1] = 'b';
	d->n[1][0] = 'c';

	d->o = func_1;
	d->p = func_2;

	d->q = cast<*>(0);
	d->r = "123";
	d->s = cast<*struct B>(0);




	return 0;
}
