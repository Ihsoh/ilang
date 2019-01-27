#include <test.ilh>

function main(argc:int, argv:**char):int {
	for (;;) {
		break;
	}

	for (var i:int;;) {
		break;
	}

	for (var i:int = 0, i1:int = 0;;) {
		break;
	}

	for (; 1 + 1 == 2;) {
		break;
	}

	var tmp:int = 0;
	for (;; ++tmp) {
		break;
	}

	var total1:int = 0;
	for (var i:int = 1; i <= 100; ++i) {
		total1 += i;
	}
	if (total1 != 5050) {
		return 1;
	}

	var total2:long = 0;
	for (var i1:int = 0; i1 < 10; ++i1) {
		for (var i2:int = 0; i2 < 10; ++i2) {
			for (var i3:int = 0; i3 < 10; ++i3) {
				for (var i4:int = 0; i4 < 10; ++i4) {
					for (var i5:int = 0; i5 < 10; ++i5) {
						for (var i6:int = 0; i6 < 10; ++i6) {
							for (var i7:int = 0; i7 < 10; ++i7) {
								for (var i8:int = 0; i8 < 10; ++i8) {
									for (var i9:int = 0; i9 < 10; ++i9) {
										for (var i10:int = 0; i10 < 10; ++i10) {
											++total2;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (total2 != 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10) {
		return 1;
	}

	return 0;
}
