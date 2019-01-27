#include <test.ilh>

#include "A1.ilh"
#include "A2.ilh"
#include <A3.ilh>
#include <A4.ilh>

function main(argc:int, argv:**char):int {
	if (strcmp(A1, "a1") == 0
			&& strcmp(A2, "a2") == 0
			&& strcmp(A3, "a3") == 0
			&& strcmp(A4, "a4") == 0) {
		return 0;
	} else {
		return 1;
	}
	return 0;
}
