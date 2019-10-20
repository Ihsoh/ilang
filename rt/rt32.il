#if !defined(__ASM_GAS__) && !defined(__ASM_IASM__)
	#define	__ASM_GAS__
#endif

#if defined(__ASM_GAS__)
	#define	_SET_REG_EAX	"set#%eax"
	#define	_SET_REG_EBX	"set#%ebx"
	#define	_SET_REG_ECX	"set#%ecx"
	#define	_SET_REG_EDX	"set#%edx"
#elif defined(__ASM_IASM__)
	#error NOT SUPPORT 'IASM'
#else
	#error RT32 ERROR
#endif

struct __RtInt64 {
	var l:uint32;
	var h:uint32;
}

function __rt_add_64(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	var rl:uint32 = 0, rh:uint32 = 0;

	#if defined(__ASM_GAS__)
		asm(
			"	addl %ecx, %eax\n\t	\
				adcl %edx, %ebx\n\t	\
			",
			"set#%eax"(a->l),
			"set#%ebx"(a->h),
			"set#%ecx"(b->l),
			"set#%edx"(b->h),
			"get#%eax"(rl),
			"get#%ebx"(rh)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	r->l = rl;
	r->h = rh;
}

function __rt_sub_64(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	var rl:uint32 = 0, rh:uint32 = 0;

	#if defined(__ASM_GAS__)
		asm(
			"	subl %ecx, %eax\n\t	\
				sbbl %edx, %ebx\n\t	\
			",
			"set#%eax"(a->l),
			"set#%ebx"(a->h),
			"set#%ecx"(b->l),
			"set#%edx"(b->h),
			"get#%eax"(rl),
			"get#%ebx"(rh)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	r->l = rl;
	r->h = rh;
}



/*
	64位有符号整数相乘。
	参数：
		EAX=数字1的低32位。
		EBX=数字1的高32位。
		ECX=数字2的低32位。
		EDX=数字2的高32位。
	返回值：
		EAX=结果的低32位。
		EBX=结果的高32位。
*/
function __rt_mul_i64() {
	asm(
		"",
		_SET_REG_EAX(0),
		_SET_REG_EBX(0)
	);
}

/*
	64位无符号整数相乘。
	参数：
		EAX=数字1的低32位。
		EBX=数字1的高32位。
		ECX=数字2的低32位。
		EDX=数字2的高32位。
	返回值：
		EAX=结果的低32位。
		EBX=结果的高32位。
*/
function __rt_mul_u64() {
	asm(
		"",
		_SET_REG_EAX(0),
		_SET_REG_EBX(0)
	);
}

/*
	64位有符号整数相除。
	参数：
		EAX=数字1的低32位。
		EBX=数字1的高32位。
		ECX=数字2的低32位。
		EDX=数字2的高32位。
	返回值：
		EAX=结果商的低32位。
		EBX=结果商的高32位。
		ECX=结果余的低32位。
		EDX=结果余的高32位。
*/
function __rt_div_i64() {
	asm(
		"",
		_SET_REG_EAX(0),
		_SET_REG_EBX(0),
		_SET_REG_ECX(0),
		_SET_REG_EDX(0)
	);
}

/*
	64位无符号整数相除。
	参数：
		EAX=数字1的低32位。
		EBX=数字1的高32位。
		ECX=数字2的低32位。
		EDX=数字2的高32位。
	返回值：
		EAX=结果商的低32位。
		EBX=结果商的高32位。
		ECX=结果余的低32位。
		EDX=结果余的高32位。
*/
function __rt_div_u64() {
	asm(
		"",
		_SET_REG_EAX(0),
		_SET_REG_EBX(0),
		_SET_REG_ECX(0),
		_SET_REG_EDX(0)
	);
}
