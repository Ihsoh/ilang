#include <testlib.ilh>

struct A {
	var a:int8;
}

struct B {
	var a:int16;
}

struct C {
	var a:int8;
	var b:int16;
}

struct D {
	var a:int16;
	var b:int8;
}

struct E {
	var a:int8;
	var b:struct D;
}

struct F {
	var a:int8;
	var b:struct D;
	var c:[10]int32;
}

struct G {
	var a:int8;
	var b:struct D;
	var c:[10]int32;
	var d:int64;
}

struct H {
}

packed struct I {
}

packed struct J {
	var x:int;
	var y:int;
}

align(1) var ga:char;
align(1) var gb:int;
align(16) var gc:char = 1;
align(16) var gd:int = 2;

function main(argc:int, argv:**char):int {
	{
		align(1) var la:char;
		align(1) var lb:int;
		align(16) var lc:char = 1;
		align(16) var ld:int = 2;

		if (alignof(ga) != 1) {
			assert(0);
		}
		if (alignof(gb) != 1) {
			assert(0);
		}
		if (alignof(gc) != 16) {
			assert(0);
		}
		if (alignof(gd) != 16) {
			assert(0);
		}

		if (alignof(la) != 1) {
			assert(0);
		}
		if (alignof(lb) != 1) {
			assert(0);
		}
		if (alignof(lc) != 16) {
			assert(0);
		}
		if (alignof(ld) != 16) {
			assert(0);
		}
	}

	{
		var la:struct A;
		var lb:struct B;
		var lc:struct C;
		var ld:struct D;
		var le:struct E;
		var lf:struct F;
		var lg:struct G;

		var lh:[10]char;
		var li:[10]int32;
		var lj:[10]struct G;

		if (alignof(cast<char>(1)) != 1) {
			assert(0);
		}
		if (alignof(cast<int8>(1)) != 1) {
			assert(0);
		}
		if (alignof(cast<int16>(1)) != 2) {
			assert(0);
		}
		if (alignof(cast<int32>(1)) != 4) {
			assert(0);
		}
		if (alignof(cast<int64>(1)) != 8) {
			assert(0);
		}
		if (alignof(cast<uint8>(1)) != 1) {
			assert(0);
		}
		if (alignof(cast<uint16>(1)) != 2) {
			assert(0);
		}
		if (alignof(cast<uint32>(1)) != 4) {
			assert(0);
		}
		if (alignof(cast<uint64>(1)) != 8) {
			assert(0);
		}
		if (alignof(cast<float>(1)) != 4) {
			assert(0);
		}
		if (alignof(cast<double>(1)) != 8) {
			assert(0);
		}
		if (alignof(la) != 1) {
			assert(0);
		}
		if (alignof(lb) != 2) {
			assert(0);
		}
		if (alignof(lc) != 2) {
			assert(0);
		}
		if (alignof(ld) != 2) {
			assert(0);
		}
		if (alignof(le) != 2) {
			assert(0);
		}
		if (alignof(lf) != 4) {
			assert(0);
		}
		if (alignof(lg) != 8) {
			assert(0);
		}
		if (alignof(lh) != 1) {
			assert(0);
		}
		if (alignof(li) != 4) {
			assert(0);
		}
		if (alignof(lj) != 8) {
			assert(0);
		}
		if (alignof(cast<*>(0)) != ARCH_WORD_SIZE) {
			assert(0);
		}
		if (alignof(cast<*char>(0)) != ARCH_WORD_SIZE) {
			assert(0);
		}
		if (alignof(cast<*int>(0)) != ARCH_WORD_SIZE) {
			assert(0);
		}
	}

	if (alignof(char) != 1) {
		assert(0);
	}
	if (alignof(int8) != 1) {
		assert(0);
	}
	if (alignof(int16) != 2) {
		assert(0);
	}
	if (alignof(int32) != 4) {
		assert(0);
	}
	if (alignof(int64) != 8) {
		assert(0);
	}
	if (alignof(uint8) != 1) {
		assert(0);
	}
	if (alignof(uint16) != 2) {
		assert(0);
	}
	if (alignof(uint32) != 4) {
		assert(0);
	}
	if (alignof(uint64) != 8) {
		assert(0);
	}
	if (alignof(float) != 4) {
		assert(0);
	}
	if (alignof(double) != 8) {
		assert(0);
	}

	if (alignof(struct A) != 1) {
		assert(0);
	}
	if (alignof(struct B) != 2) {
		assert(0);
	}
	if (alignof(struct C) != 2) {
		assert(0);
	}
	if (alignof(struct D) != 2) {
		assert(0);
	}
	if (alignof(struct E) != 2) {
		assert(0);
	}
	if (alignof(struct F) != 4) {
		assert(0);
	}
	if (alignof(struct G) != 8) {
		assert(0);
	}
	if (alignof(struct H) != 1) {
		assert(0);
	}
	if (alignof(struct I) != 1) {
		assert(0);
	}
	if (alignof(struct J) != 1) {
		assert(0);
	}

	if (alignof([10]char) != 1) {
		assert(0);
	}
	if (alignof([10]int8) != 1) {
		assert(0);
	}
	if (alignof([10]int16) != 2) {
		assert(0);
	}
	if (alignof([10]int32) != 4) {
		assert(0);
	}
	if (alignof([10]int64) != 8) {
		assert(0);
	}
	if (alignof([10]uint8) != 1) {
		assert(0);
	}
	if (alignof([10]uint16) != 2) {
		assert(0);
	}
	if (alignof([10]uint32) != 4) {
		assert(0);
	}
	if (alignof([10]uint64) != 8) {
		assert(0);
	}
	if (alignof([10]float) != 4) {
		assert(0);
	}
	if (alignof([10]double) != 8) {
		assert(0);
	}
	if (alignof([10]struct A) != 1) {
		assert(0);
	}
	if (alignof([10]struct B) != 2) {
		assert(0);
	}
	if (alignof([10]struct C) != 2) {
		assert(0);
	}
	if (alignof([10]struct D) != 2) {
		assert(0);
	}
	if (alignof([10]struct E) != 2) {
		assert(0);
	}
	if (alignof([10]struct F) != 4) {
		assert(0);
	}
	if (alignof([10]struct G) != 8) {
		assert(0);
	}

	if (alignof(*) != ARCH_WORD_SIZE) {
		assert(0);
	}
	if (alignof(*char) != ARCH_WORD_SIZE) {
		assert(0);
	}
	if (alignof(*int) != ARCH_WORD_SIZE) {
		assert(0);
	}

	return 0;
}
