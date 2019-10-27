function print_int8(str:*char, n:int8) {
	asm(
		"	\
			pushl %eax\n	\
			calll printf\n	\
			addl $4, %esp\n	\
		",
		"set#%eax"(str)
	);

	asm(
		"	\
			pushl %ebx\n	\
			pushl %eax\n	\
			calll printf\n	\
			addl $8, %esp\n	\
		",
		"set#%eax"("%d\n"),
		"set#%ebx"(cast<int32>(n))
	);
}
