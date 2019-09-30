#include <testlib.ilh>

function main(argc:int, argv:**char):int {
	while ('\0') {
		return 1;
	}

	while (cast<int8>(0)) {
		return 1;
	}

	while (cast<int16>(0)) {
		return 1;
	}

	while (cast<int32>(0)) {
		return 1;
	}

	while (cast<int64>(0)) {
		return 1;
	}

	while (cast<uint8>(0)) {
		return 1;
	}

	while (cast<uint16>(0)) {
		return 1;
	}

	while (cast<uint32>(0)) {
		return 1;
	}

	while (cast<uint64>(0)) {
		return 1;
	}

	while (cast<*>(0)) {
		return 1;
	}

	while (cast<*int8>(0)) {
		return 1;
	}

	var total:int64 = 0;
	var i1:int = 0;
	while (i1 < 10) {
		var i2:int = 0;
		while (i2 < 10) {
			var i3:int = 0;
			while (i3 < 10) {
				var i4:int = 0;
				while (i4 < 10) {
					var i5:int = 0;
					while (i5 < 10) {
						var i6:int = 0;
						while (i6 < 10) {
							var i7:int = 0;
							while (i7 < 10) {
								var i8:int = 0;
								while (i8 < 10) {
									var i9:int = 0;
									while (i9 < 10) {
										var i10:int = 0;
										while (i10 < 10) {
											++total;
											++i10;
										}
										++i9;
									}
									++i8;
								}
								++i7;
							}
							++i6;
						}
						++i5;
					}
					++i4;
				}
				++i3;
			}
			++i2;
		}
		++i1;
	}
	if (total != cast<int64>(10) * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10) {
		return 1;
	}

	return 0;
}
