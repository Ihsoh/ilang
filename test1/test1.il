#include "test1.ilh"

#define	NULL						(cast<*>(0))
#define	ADD(a, b)					((a) + (b))
#define	MUL(a, b)					((a) * (b))
#define	SUM(result, start, end)		for (var i:int32 = (start); i <= (end); ++i) { result = ADD(result, i); }

function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function printf(fmt:*char, ...) dummy

function printData(data:*struct Data) {
	printf("Data [a=%d, b=%s]\n", data->a, data->b);
}

function main(argc:int32, argv:**char):int32 {
	var a:int32 = 1, b:int32 = 2;
	var ptrData:*struct Data = cast<*struct Data>(malloc(sizeof(struct Data)));
	if (ptrData == NULL) {
		printf("%s\n", "cannot allocate memory!");
		return -1;
	}
	ptrData->a = (a + b) * 3;
	ptrData->b = "Wow";
	printData(ptrData);
	free(ptrData);

	printf("ADD(1, 2) = %d\n", ADD(1, 2));

	printf("ADD(1, 2) + MUL(3, 4) = %d\n", ADD(1, 2) + MUL(3, 4));

	var result:int32 = 0;
	SUM(result, 1, 100)
	printf("SUM(result, 1, 100), result = %d\n", result);

	return 0;
}
