#include "array.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myassert.h"

#define	_BYTE_COUNT(rarr_ptr, size)	((size) * (rarr_ptr)->item_size)

void rarr_init(
	ResizableArray *rarr,
	size_t item_size
) {
	assert(rarr);
	assert(item_size > 0);

	rarr->size = 0;
	rarr->len = 0;
	rarr->item_size = item_size;
	rarr->buffer = NULL;
}

void rarr_free(ResizableArray *rarr) {
	assert(rarr);

	if (rarr->buffer != NULL) {
		free(rarr->buffer);	
	}

	rarr->size = 0;
	rarr->len = 0;
	rarr->item_size = 0;
	rarr->buffer = NULL;
}

static void _inc_buffer_size(
	ResizableArray *rarr,
	size_t inc_size
) {
	assert(rarr);
	assert(inc_size > 0);

	rarr->size = rarr->size + 2 * inc_size;
	if (rarr->buffer == NULL) {
		rarr->buffer = (char *)malloc(_BYTE_COUNT(rarr, rarr->size));
	} else {
		rarr->buffer = (char *)realloc(rarr->buffer, _BYTE_COUNT(rarr, rarr->size));
	}
}

void rarr_append_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len
) {
	assert(dst);
	assert(src);
	
	if (len == 0) {
		return;
	}

	if (dst->len + len > dst->size) {
		_inc_buffer_size(dst, len);
	}

	memcpy((uint8_t *)(dst->buffer) + _BYTE_COUNT(dst, dst->len), src, _BYTE_COUNT(dst, len));
	dst->len += len;
}

void rarr_append_with_rarr(
	ResizableArray *dst,
	ResizableArray *src
) {
	assert(dst);
	assert(src);
	assert(dst->item_size == src->item_size);

	rarr_append_with_raw(dst, src->buffer, src->len);
}

void rarr_insert_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len,
	size_t pos
) {
	assert(dst);
	assert(src);

	if (len == 0) {
		return;
	}

	if (dst->len + len > dst->size) {
		_inc_buffer_size(dst, len);
	}

	memcpy(dst->buffer + _BYTE_COUNT(dst, pos + len), dst->buffer + _BYTE_COUNT(dst, pos), _BYTE_COUNT(dst, dst->len - pos));
	memcpy(dst->buffer + _BYTE_COUNT(dst, pos), src, _BYTE_COUNT(dst, len));
	dst->len += len;
}

void rarr_insert_with_rarr(
	ResizableArray *dst,
	ResizableArray *src,
	size_t pos
) {
	assert(dst);
	assert(src);

	rarr_insert_with_raw(dst, src->buffer, src->len, pos);
}

void rarr_prepend_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len
) {
	assert(dst);
	assert(src);

	rarr_insert_with_raw(dst, src, len, 0);
}

void rarr_prepend_with_rarr(
	ResizableArray *dst,
	ResizableArray *src
) {
	assert(dst);
	assert(src);

	rarr_insert_with_rarr(dst, src, 0);
}




void rarr_test(void) {
	ResizableArray rarr1;

	rarr_init(&rarr1, sizeof(int));
	MYASSERT(
		rarr1.size == 0 && rarr1.len == 0 && rarr1.item_size == sizeof(int) && rarr1.buffer == NULL,
		"TEST rarr_init(...)"
	);

	#define	__CMP_RARR_ARR(rarr, arr, msg)	{	\
		int __temp[] = arr;	\
		if ((rarr)->len != sizeof(__temp) / sizeof(int)) {	\
			MYASSERT(0, (msg "xxxxxx"));	\
		}	\
		for (int __i = 0; __i < sizeof(__temp) / sizeof(int); __i++) {	\
			if (__temp[__i] != RARR_ELEMENT((rarr), __i, int)) {	\
				MYASSERT(0, (msg));	\
			}	\
		}	\
	}
	#define	__C	,

	// [] + [1, 2] => [1, 2]
	int arr1[] = {1, 2};
	rarr_append_with_raw(&rarr1, arr1, sizeof(arr1) / sizeof(int));
	__CMP_RARR_ARR(&rarr1, {1 __C 2}, "TEST rarr_append_with_raw(...)");

	// [1, 2] + [3, 4] => [1, 2, 3, 4]
	int arr2[] = {3, 4};
	ResizableArray rarr2;
	rarr_init(&rarr2, sizeof(int));
	rarr_append_with_raw(&rarr2, arr2, sizeof(arr2) / sizeof(int));
	rarr_append_with_rarr(&rarr1, &rarr2);
	__CMP_RARR_ARR(&rarr1, {1 __C 2 __C 3 __C 4}, "TEST rarr_append_with_rarr(...)");
	rarr_free(&rarr2);

	// INSERT([1, 2, 3, 4], 1, [10, 20]) => [1, 10, 20, 2, 3, 4]
	int arr3[] = {10, 20};
	rarr_insert_with_raw(&rarr1, arr3, sizeof(arr3) / sizeof(int), 1);
	__CMP_RARR_ARR(&rarr1, {1 __C 10 __C 20 __C 2 __C 3 __C 4}, "TEST rarr_insert_with_raw(...)");

	// INSERT([1, 10, 20, 2, 3, 4], 4, [20, 30]) => [1, 10, 20, 2, 20, 30, 3, 4]
	int arr4[] = {20, 30};
	ResizableArray rarr3;
	rarr_init(&rarr3, sizeof(int));
	rarr_append_with_raw(&rarr3, arr4, sizeof(arr4) / sizeof(int));
	rarr_insert_with_rarr(&rarr1, &rarr3, 4);
	__CMP_RARR_ARR(&rarr1, {1 __C 10 __C 20 __C 2 __C 20 __C 30 __C 3 __C 4}, "TEST rarr_insert_with_rarr(...)");
	rarr_free(&rarr3);

	// [100, 200] + [1, 10, 20, 2, 20, 30, 3, 4] => [100, 200, 1, 10, 20, 2, 20, 30, 3, 4]
	int arr5[] = {100, 200};
	rarr_prepend_with_raw(&rarr1, arr5, sizeof(arr5) / sizeof(int));
	__CMP_RARR_ARR(&rarr1, {100 __C 200 __C 1 __C 10 __C 20 __C 2 __C 20 __C 30 __C 3 __C 4}, "TEST rarr_prepend_with_raw(...)");

	// [300, 400] + [100, 200, 1, 10, 20, 2, 20, 30, 3, 4] => [300, 400, 100, 200, 1, 10, 20, 2, 20, 30, 3, 4]
	int arr6[] = {300, 400};
	ResizableArray rarr4;
	rarr_init(&rarr4, sizeof(int));
	rarr_append_with_raw(&rarr4, arr6, sizeof(arr6) / sizeof(int));
	rarr_prepend_with_rarr(&rarr1, &rarr4);
	__CMP_RARR_ARR(&rarr1, {300 __C 400 __C 100 __C 200 __C 1 __C 10 __C 20 __C 2 __C 20 __C 30 __C 3 __C 4}, "TEST rarr_prepend_with_rarr(...)");
	rarr_free(&rarr4);

	rarr_free(&rarr1);

	#undef	__CMP_RARR_ARR
	#undef	__COMMA
}
