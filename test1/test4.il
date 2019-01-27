
function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function printf(fmt:*char, ...) dummy

#define	STR1	     "ABC123x"      
#define	STR2	ABC123x

function main(argc:int32, argv:**char):int32 {

	#info wow~~~
	#warn yahoo!!!

	printf("__ARCH__ = %s, __TARGET__ = %s\n", __ARCH__, __TARGET__);

	printf("A: __FILE__ = %s, __LINE__ = %d\n", __FILE__, __LINE__);

	#if "1"
		printf("A1\n");
	#endif

	#if "0"
		printf("A2\n");
	#endif

	#if "1"
		printf("A3\n");
	#endif

	#if streq("ABC123x", "ABC123")
		printf("A4\n");
	#endif

	#if streq("ABC123", "ABC123")
		printf("A5\n");
	#endif

	#if streq("ABC123", STR1)
		printf("A6\n");
	#endif

	#if streq(STR1, "ABC123")
		printf("A7\n");
	#endif

	#if streq("ABC123", STR2)
		printf("A8\n");
	#endif

	#if streq(STR2, "ABC123")
		printf("A9\n");
	#endif

	#if streq(STR1, STR2)
		printf("A10\n");
	#endif

	#if streq(STR2, STR1)
		printf("A11\n");
	#endif

	printf("A: __FILE__ = %s, __LINE__ = %d\n", __FILE__, __LINE__);

	return 0;
}
