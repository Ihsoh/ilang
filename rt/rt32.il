#ifdef __ASM_GAS__
	#define	_SET_REG_EAX	"set#%eax"
	#define	_SET_REG_EBX	"set#%ebx"
	#define	_SET_REG_ECX	"set#%ecx"
	#define	_SET_REG_EDX	"set#%edx"
#elifdef __ASM_IASM__
	#error NOT SUPPORT 'IASM'
#else
	#define	_SET_REG_EAX	"set#%eax"
	#define	_SET_REG_EBX	"set#%ebx"
	#define	_SET_REG_ECX	"set#%ecx"
	#define	_SET_REG_EDX	"set#%edx"
#endif

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


