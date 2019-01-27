#include <test.ilh>

function main(argc:int, argv:**char):int {
	do {

	} while ('\0');

	do {
		
	} while (cast<int8>(0));

	do {
		
	} while (cast<int16>(0));

	do {
		
	} while (cast<int32>(0));

	do {
		
	} while (cast<int64>(0));

	do {
		
	} while (cast<uint8>(0));

	do {
		
	} while (cast<uint16>(0));

	do {
		
	} while (cast<uint32>(0));

	do {
		
	} while (cast<uint64>(0));

	do {

	} while (cast<*>(0));

	do {

	} while (cast<*int8>(0));

	var total:long = 0;
	var i1:int = 0;
	do {
		var i2:int = 0;
		do {
			var i3:int = 0;
			do {
				var i4:int = 0;
				do {
					var i5:int = 0;
					do {
						var i6:int = 0;
						do {
							var i7:int = 0;
							do {
								var i8:int = 0;
								do {
									var i9:int = 0;
									do {
										var i10:int = 0;
										do {
											++total;
											++i10;
										} while (i10 < 10);
										++i9;
									} while (i9 < 10);
									++i8;
								} while (i8 < 10);
								++i7;
							} while (i7 < 10);
							++i6;
						} while (i6 < 10);
						++i5;
					} while (i5 < 10);
					++i4;
				} while (i4 < 10);
				++i3;
			} while (i3 < 10);
			++i2;
		} while (i2 < 10);
		++i1;
	} while (i1 < 10);
	if (total != 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10) {
		return 1;
	}

	return 0;
}
