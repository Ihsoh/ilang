function print_int8(str:*char, n:int8) dummy
function print_int16(str:*char, n:int16) dummy
function print_int32(str:*char, n:int32) dummy
function print_int64(str:*char, n:int64) dummy
function print_uint8(str:*char, n:uint8) dummy
function print_uint16(str:*char, n:uint16) dummy
function print_uint32(str:*char, n:uint32) dummy
function print_uint64(str:*char, n:uint64) dummy
function print_float(str:*char, n:float) dummy
function print_double(str:*char, n:double) dummy
function print_pointer(str:*char, ptr:*) dummy
function print_string(str:*char) dummy

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

#define __rt_assert(v)	;

function __rt_64_add(
	r_l:*uint32, r_h:*uint32,
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	__rt_assert(r_l);
	__rt_assert(r_h);

	var tmp_r_l:uint32 = 0, tmp_r_h:uint32 = 0;

	#if defined(__ASM_GAS__)
		asm(
			"	addl %ecx, %eax\n\t	\
				adcl %edx, %ebx\n\t	\
			",
			"set#%eax"(a_l),
			"set#%ebx"(a_h),
			"set#%ecx"(b_l),
			"set#%edx"(b_h),
			"get#%eax"(tmp_r_l),
			"get#%ebx"(tmp_r_h)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	*r_l = tmp_r_l;
	*r_h = tmp_r_h;
}

function __rt_64_sub(
	r_l:*uint32, r_h:*uint32,
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	__rt_assert(r_l);
	__rt_assert(r_h);

	var tmp_r_l:uint32 = 0, tmp_r_h:uint32 = 0;

	#if defined(__ASM_GAS__)
		asm(
			"	subl %ecx, %eax\n\t	\
				sbbl %edx, %ebx\n\t	\
			",
			"set#%eax"(a_l),
			"set#%ebx"(a_h),
			"set#%ecx"(b_l),
			"set#%edx"(b_h),
			"get#%eax"(tmp_r_l),
			"get#%ebx"(tmp_r_h)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	*r_l = tmp_r_l;
	*r_h = tmp_r_h;
}

struct __RtInt64 {
	var l:uint32;
	var h:uint32;
}

#define	__RT_RI64_LOW(v)	((v).l)
#define	__RT_RI64_HIGH(v)	((v).h)

function __rt_ri64_set_by_2u32(
	v:*struct __RtInt64,
	l:uint32,
	h:uint32
) {
	__rt_assert(v);

	v->l = l;
	v->h = h;
}

function __rt_ri64_set_by_u32(
	v:*struct __RtInt64,
	l:uint32
) {
	__rt_assert(v);

	v->l = l;
	v->h = 0;
}

function __rt_ri64_copy(
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(a);
	__rt_assert(b);

	a->l = b->l;
	a->h = b->h;
}

function __rt_ri64_add(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(r);
	__rt_assert(a);
	__rt_assert(b);

	__rt_64_add(
		&(r->l), &(r->h),
		a->l, a->h,
		b->l, b->h
	);
}

function __rt_ri64_add_ri_u32(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:uint32
) {
	__rt_assert(r);
	__rt_assert(a);

	__rt_64_add(
		&(r->l), &(r->h),
		a->l, a->h,
		b, 0
	);
}

function __rt_ri64_add_u32_ri(
	r:*struct __RtInt64,
	a:uint32,
	b:*struct __RtInt64
) {
	__rt_assert(r);
	__rt_assert(b);

	__rt_64_add(
		&(r->l), &(r->h),
		a, 0,
		b->l, b->h
	);
}

function __rt_ri64_inc(
	v:*struct __RtInt64
) {
	__rt_assert(v);

	__rt_ri64_add_ri_u32(v, v, 1);
}

function __rt_ri64_sub(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(r);
	__rt_assert(a);
	__rt_assert(b);

	__rt_64_sub(
		&(r->l), &(r->h),
		a->l, a->h,
		b->l, b->h
	);
}

function __rt_ri64_sub_ri_u32(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:uint32
) {
	__rt_assert(r);
	__rt_assert(a);

	__rt_64_sub(
		&(r->l), &(r->h),
		a->l, a->h,
		b, 0
	);
}

function __rt_ri64_sub_u32_ri(
	r:*struct __RtInt64,
	a:uint32,
	b:*struct __RtInt64
) {
	__rt_assert(r);
	__rt_assert(b);

	__rt_64_sub(
		&(r->l), &(r->h),
		a, 0,
		b->l, b->h
	);
}

function __rt_ri64_dec(
	v:*struct __RtInt64
) {
	__rt_assert(v);

	__rt_ri64_sub_ri_u32(v, v, 1);
}

function __rt_ri64_shl(
	r:*struct __RtInt64,
	v:*struct __RtInt64,
	c:uint8
) {
	__rt_assert(r);
	__rt_assert(v);

	var tmp_l:uint32 = 0, tmp_h:uint32 = 0;
	
	#if defined(__ASM_GAS__)
		asm(
			"	movl %eax, %esi\n\t	\
				movl %ebx, %edi\n\t	\
				shll %cl, %esi\n\t	\
				shldl %cl, %eax, %edi\n\t	\
				movl %esi, %eax\n\t	\
				movl %edi, %ebx\n\t	\
				xorl %edx, %edx\n\t	\
				testb $32, %cl\n\t	\
				cmovnel %edx, %eax\n\t	\
				cmovnel %esi, %ebx\n\t	\
			",
			"set#%eax"(v->l),
			"set#%ebx"(v->h),
			"set#%cl"(c),
			"get#%eax"(tmp_l),
			"get#%ebx"(tmp_h)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	r->l = tmp_l;
	r->h = tmp_h;
}

function __rt_ri64_sar(
	r:*struct __RtInt64,
	v:*struct __RtInt64,
	c:uint8
) {
	__rt_assert(r);
	__rt_assert(v);

	var tmp_l:uint32 = 0, tmp_h:uint32 = 0;
	
	#if defined(__ASM_GAS__)
		asm(
			"	movl %eax, %esi\n\t	\
				movl %ebx, %edi\n\t	\
				sarl %cl, %edi\n\t	\
				shrdl %cl, %ebx, %esi\n\t	\
				movl %ebx, %edx\n\t	\
				sarl $31, %edx\n\t	\
				movl %esi, %eax\n\t	\
				movl %edi, %ebx\n\t	\
				testb $32, %cl\n\t	\
				cmovnel %edi, %eax\n\t	\
				cmovnel %edx, %ebx\n\t	\
			",
			"set#%eax"(v->l),
			"set#%ebx"(v->h),
			"set#%cl"(c),
			"get#%eax"(tmp_l),
			"get#%ebx"(tmp_h)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	r->l = tmp_l;
	r->h = tmp_h;
}

function __rt_ri64_shr(
	r:*struct __RtInt64,
	v:*struct __RtInt64,
	c:uint8
) {
	__rt_assert(r);
	__rt_assert(v);

	var tmp_l:uint32 = 0, tmp_h:uint32 = 0;
	
	#if defined(__ASM_GAS__)
		asm(
			"	movl %eax, %esi\n\t	\
				movl %ebx, %edi\n\t	\
				shrl %cl, %edi\n\t	\
				shrdl %cl, %ebx, %esi\n\t	\
				movl %esi, %eax\n\t	\
				movl %edi, %ebx\n\t	\
				xorl %edx, %edx\n\t	\
				testb $32, %cl\n\t	\
				cmovnel %edi, %eax\n\t	\
				cmovnel %edx, %ebx\n\t	\
			",
			"set#%eax"(v->l),
			"set#%ebx"(v->h),
			"set#%cl"(c),
			"get#%eax"(tmp_l),
			"get#%ebx"(tmp_h)
		);
	#elif defined(__ASM_IASM__)
		#error NOT SUPPORT 'IASM'
	#else
		#error RT32 ERROR
	#endif

	r->l = tmp_l;
	r->h = tmp_h;
}

function __rt_ri64_is_bit_set(
	v:*struct __RtInt64,
	c:uint8
):uint32 {
	__rt_assert(v);

	if (c <= 31) {
		return v->l & (1 << c);
	} else {
		return v->h & (1 << (c - 32));
	}
}

#define	__RT_IS_NEGATIVE_RI64(ri64v)	((ri64v).h & 0x80000000)
#define	__RT_IS_POSITIVE_RI64(ri64v)	(!(__RT_IS_NEGATIVE_RI64((ri64v))))

function __rt_ri64_cmp_u(
	a:*struct __RtInt64,
	b:*struct __RtInt64
):int32 {
	__rt_assert(a);
	__rt_assert(b);

	if (a->l == b->l && a->h == b->h) {
		return 0;
	} else {
		if (a->h > b->h) {
			return 1;
		}

		if (a->h == b->h && a->l > b->l) {
			return 1;
		}

		return -1;
	}
}

#define	__RT_RI64_CMP_EQ_U(a, b)	(__rt_ri64_cmp_u((a), (b)) == 0)
#define	__RT_RI64_CMP_NE_U(a, b)	(__rt_ri64_cmp_u((a), (b)) != 0)
#define	__RT_RI64_CMP_GT_U(a, b)	(__rt_ri64_cmp_u((a), (b)) > 0)
#define	__RT_RI64_CMP_GE_U(a, b)	(__rt_ri64_cmp_u((a), (b)) >= 0)
#define	__RT_RI64_CMP_LT_U(a, b)	(__rt_ri64_cmp_u((a), (b)) < 0)
#define	__RT_RI64_CMP_LE_U(a, b)	(__rt_ri64_cmp_u((a), (b)) <= 0)

function __rt_ri64_cmp_i(
	a:*struct __RtInt64,
	b:*struct __RtInt64
):int32 {
	__rt_assert(a);
	__rt_assert(b);

	if (__RT_IS_POSITIVE_RI64(*a) && __RT_IS_NEGATIVE_RI64(*b)) {
		return 1;
	} elif (__RT_IS_NEGATIVE_RI64(*a) && __RT_IS_POSITIVE_RI64(*b)) {
		return -1;
	} else {
		return __rt_ri64_cmp_u(a, b);
	}
}

#define	__RT_RI64_CMP_EQ_I(a, b)	(__rt_ri64_cmp_i((a), (b)) == 0)
#define	__RT_RI64_CMP_NE_I(a, b)	(__rt_ri64_cmp_i((a), (b)) != 0)
#define	__RT_RI64_CMP_GT_I(a, b)	(__rt_ri64_cmp_i((a), (b)) > 0)
#define	__RT_RI64_CMP_GE_I(a, b)	(__rt_ri64_cmp_i((a), (b)) >= 0)
#define	__RT_RI64_CMP_LT_I(a, b)	(__rt_ri64_cmp_i((a), (b)) < 0)
#define	__RT_RI64_CMP_LE_I(a, b)	(__rt_ri64_cmp_i((a), (b)) <= 0)

function __rt_ri64_mul_u(
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(r);
	__rt_assert(a);
	__rt_assert(b);

	var const_0:struct __RtInt64;
	var const_1:struct __RtInt64;

	__rt_ri64_set_by_u32(&const_0, 0);
	__rt_ri64_set_by_u32(&const_1, 1);

	print_string("#mul 1\n");

	if (__RT_RI64_CMP_EQ_U(b, &const_0)) {
		__rt_ri64_set_by_u32(r, 0);
		return;
	}

	print_string("#mul 2\n");

	var tmp_a:uint64 = (cast<uint64>(a->h) << 32) + a->l;
	print_uint64("#a = ", tmp_a);
	var tmp_b:uint64 = (cast<uint64>(b->h) << 32) + b->l;
	print_uint64("#b = ", tmp_b);
	var tmp_const_1:uint64 = (cast<uint64>(const_1.h) << 32) + const_1.l;
	print_uint64("#const1 = ", tmp_const_1);

	print_int32("#cmp1 = ", __RT_RI64_CMP_EQ_U(b, &const_1));
	print_int32("#cmp2 = ", __rt_ri64_cmp_u(b, &const_1));
	print_int32("#cmp3 = ", b->l == const_1.l && b->h == const_1.h);

	if (__RT_RI64_CMP_EQ_U(b, &const_1)) {
		__rt_ri64_copy(r, a);
		return;
	}

	print_string("#mul 3\n");

	__rt_ri64_set_by_u32(r, 0);

	for (var i:uint8 = 0; i < 64; i++) {
		if (__rt_ri64_is_bit_set(b, i)) {
			var temp:struct __RtInt64;
			__rt_ri64_copy(&temp, a);

			__rt_ri64_shl(&temp, &temp, i);
			__rt_ri64_add(r, r, &temp);

			print_string("#mul 4\n");
		}
	}
}

function __rt_ri64_div_u(
	q:*struct __RtInt64,
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(q);
	__rt_assert(r);
	__rt_assert(a);
	__rt_assert(b);

	var const_0:struct __RtInt64;
	var const_1:struct __RtInt64;

	__rt_ri64_set_by_u32(&const_0, 0);
	__rt_ri64_set_by_u32(&const_1, 1);

	if (__RT_RI64_CMP_EQ_U(b, &const_0)) {
		__rt_ri64_set_by_u32(q, 0);
		__rt_ri64_set_by_u32(r, 0);
		return;
	}

	if (__RT_RI64_CMP_EQ_U(b, &const_1)) {
		__rt_ri64_copy(q, a);
		__rt_ri64_set_by_u32(r, 0);
		return;
	}

	if (__RT_RI64_CMP_EQ_U(a, &const_0)) {
		__rt_ri64_set_by_u32(q, 0);
		__rt_ri64_set_by_u32(r, 0);
		return;
	}

	if (__RT_RI64_CMP_EQ_U(a, b)) {
		__rt_ri64_set_by_u32(q, 1);
		__rt_ri64_set_by_u32(r, 0);
		return;
	}

	if (__RT_RI64_CMP_LT_U(a, b)) {
		__rt_ri64_set_by_u32(q, 0);
		__rt_ri64_copy(r, a);
		return;
	}

	__rt_ri64_set_by_u32(q, 0);

	var temp_a:struct __RtInt64;
	__rt_ri64_copy(&temp_a, a);

	while (__RT_RI64_CMP_GE_U(&temp_a, b)) {
		var count:uint8 = 0;
		var temp_b:struct __RtInt64;
		__rt_ri64_copy(&temp_b, b);

		while (__RT_RI64_CMP_LT_U(&temp_b, &temp_a) && !__rt_ri64_is_bit_set(&temp_b, 63)) {
			__rt_ri64_shl(&temp_b, &temp_b, 1);
			count++;
		}
		if (__RT_RI64_CMP_GT_U(&temp_b, &temp_a)) {
			__rt_ri64_shr(&temp_b, &temp_b, 1);
			count--;
		}
		if (count > 0) {
			var temp_inc:struct __RtInt64;
			__rt_ri64_set_by_u32(&temp_inc, 1);
			__rt_ri64_shl(&temp_inc, &temp_inc, count);
			__rt_ri64_add(q, q, &temp_inc);

			__rt_ri64_sub(&temp_a, &temp_a, &temp_b);
		} else {
			break;
		}
	}

	while (__RT_RI64_CMP_GE_U(&temp_a, b)) {
		__rt_ri64_sub(&temp_a, &temp_a, b);
		__rt_ri64_inc(q);
	}

	__rt_ri64_copy(r, &temp_a);
}

function __rt_ri64_encode_comp(
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(a);
	__rt_assert(b);

	__rt_ri64_set_by_2u32(
		a,
		~__RT_RI64_LOW(*b),
		~__RT_RI64_HIGH(*b)
	);

	__rt_ri64_inc(a);
}

function __rt_ri64_decode_comp(
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(a);
	__rt_assert(b);

	__rt_ri64_copy(a, b);

	__rt_ri64_dec(a);

	__rt_ri64_set_by_2u32(
		a,
		~__RT_RI64_LOW(*a),
		~__RT_RI64_HIGH(*a)
	);
}

function __rt_ri64_div_i(
	q:*struct __RtInt64,
	r:*struct __RtInt64,
	a:*struct __RtInt64,
	b:*struct __RtInt64
) {
	__rt_assert(q);
	__rt_assert(r);
	__rt_assert(a);
	__rt_assert(b);

	var temp_a:struct __RtInt64;
	var sign_a:uint32 = __rt_ri64_is_bit_set(a, 63);
	if (sign_a) {
		__rt_ri64_encode_comp(&temp_a, a);
	} else {
		__rt_ri64_copy(&temp_a, a);
	}

	var temp_b:struct __RtInt64;
	var sign_b:uint32 = __rt_ri64_is_bit_set(b, 63);
	if (sign_b) {
		__rt_ri64_encode_comp(&temp_b, b);
	} else {
		__rt_ri64_copy(&temp_b, b);
	}

	var temp_q:struct __RtInt64;
	var temp_r:struct __RtInt64;
	__rt_ri64_div_u(&temp_q, &temp_r, &temp_a, &temp_b);

	if ((sign_a && sign_b) || (!sign_a && !sign_b)) {
		__rt_ri64_copy(q, &temp_q);
	} else {
		__rt_ri64_encode_comp(q, &temp_q);
	}

	if (sign_a) {
		__rt_ri64_encode_comp(r, &temp_r);
	} else {
		__rt_ri64_copy(r, &temp_r);
	}
}

/*
	64位有符号整数相乘。
*/
function __rt_mul_i64(
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	var r:struct __RtInt64;
	var a:struct __RtInt64;
	var b:struct __RtInt64;

	var tmp_a:uint64 = (cast<uint64>(a_h) << 32) + a_l;
	print_uint64("A = ", tmp_a);
	var tmp_b:uint64 = (cast<uint64>(b_h) << 32) + b_l;
	print_uint64("B = ", tmp_b);

	__rt_ri64_set_by_2u32(&a, a_l, a_h);
	__rt_ri64_set_by_2u32(&b, b_l, b_h);

	__rt_ri64_mul_u(&r, &a, &b);

	var tmp_c:uint64 = (cast<uint64>(r.h) << 32) + r.l;
	print_uint64("C = ", tmp_c);

	asm(
		"",
		_SET_REG_EAX(__RT_RI64_LOW(r)),
		_SET_REG_EBX(__RT_RI64_HIGH(r))
	);
}

/*
	64位无符号整数相乘。
*/
function __rt_mul_u64(
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	var r:struct __RtInt64;
	var a:struct __RtInt64;
	var b:struct __RtInt64;

	__rt_ri64_set_by_2u32(&a, a_l, a_h);
	__rt_ri64_set_by_2u32(&b, b_l, b_h);

	__rt_ri64_mul_u(&r, &a, &b);

	asm(
		"",
		_SET_REG_EAX(__RT_RI64_LOW(r)),
		_SET_REG_EBX(__RT_RI64_HIGH(r))
	);
}

/*
	64位有符号整数相除。
	返回值：
		EAX=结果商的低32位。
		EBX=结果商的高32位。
		ECX=结果余的低32位。
		EDX=结果余的高32位。
*/
function __rt_div_i64(
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	var q:struct __RtInt64;
	var r:struct __RtInt64;
	var a:struct __RtInt64;
	var b:struct __RtInt64;

	__rt_ri64_set_by_2u32(&a, a_l, a_h);
	__rt_ri64_set_by_2u32(&b, b_l, b_h);

	__rt_ri64_div_i(&q, &r, &a, &b);

	asm(
		"",
		_SET_REG_EAX(__RT_RI64_LOW(q)),
		_SET_REG_EBX(__RT_RI64_HIGH(q)),
		_SET_REG_ECX(__RT_RI64_LOW(r)),
		_SET_REG_EDX(__RT_RI64_HIGH(r))
	);
}

/*
	64位无符号整数相除。
	返回值：
		EAX=结果商的低32位。
		EBX=结果商的高32位。
		ECX=结果余的低32位。
		EDX=结果余的高32位。
*/
function __rt_div_u64(
	a_l:uint32, a_h:uint32,
	b_l:uint32, b_h:uint32
) {
	var q:struct __RtInt64;
	var r:struct __RtInt64;
	var a:struct __RtInt64;
	var b:struct __RtInt64;

	__rt_ri64_set_by_2u32(&a, a_l, a_h);
	__rt_ri64_set_by_2u32(&b, b_l, b_h);

	__rt_ri64_div_u(&q, &r, &a, &b);

	asm(
		"",
		_SET_REG_EAX(__RT_RI64_LOW(q)),
		_SET_REG_EBX(__RT_RI64_HIGH(q)),
		_SET_REG_ECX(__RT_RI64_LOW(r)),
		_SET_REG_EDX(__RT_RI64_HIGH(r))
	);
}
