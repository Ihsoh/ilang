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

function print_int16(str:*char, n:int16) {
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

function print_int32(str:*char, n:int32) {
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
		"set#%ebx"(n)
	);
}

function print_int64(str:*char, n:int64) {
	var n_l:uint32 = *cast<*uint32>(&n);
	var n_h:uint32 = *(cast<*uint32>(&n) + 1);

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
			pushl %ecx\n	\
			pushl %ebx\n	\
			pushl %eax\n	\
			calll printf\n	\
			addl $12, %esp\n	\
		",
		"set#%eax"("%lld\n"),
		"set#%ebx"(n_l),
		"set#%ecx"(n_h)
	);
}
