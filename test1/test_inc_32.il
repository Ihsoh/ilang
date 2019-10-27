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

function print_int8(str:*char, n:int8) {
	print_int32(str, n);
}

function print_int16(str:*char, n:int16) {
	print_int32(str, n);
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

function print_uint32(str:*char, n:uint32) {
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
		"set#%eax"("%u\n"),
		"set#%ebx"(n)
	);
}

function print_uint8(str:*char, n:uint8) {
	print_uint32(str, n);
}

function print_uint16(str:*char, n:uint16) {
	print_uint32(str, n);
}

function print_uint64(str:*char, n:uint64) {
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
		"set#%eax"("%llu\n"),
		"set#%ebx"(n_l),
		"set#%ecx"(n_h)
	);
}

function print_float(str:*char, n:float) {
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
		"set#%eax"("%f\n"),
		"set#%ebx"(n)
	);
}

function print_double(str:*char, n:double) {
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
		"set#%eax"("%lf\n"),
		"set#%ebx"(n_l),
		"set#%ecx"(n_h)
	);
}

function print_string(str:*char) {
	asm(
		"	\
			pushl %eax\n	\
			calll printf\n	\
			addl $4, %esp\n	\
		",
		"set#%eax"(str)
	);
}

function print_pointer(str:*char, ptr:*) {
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
		"set#%eax"("%x\n"),
		"set#%ebx"(ptr)
	);
}
