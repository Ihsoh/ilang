#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <inttypes.h>
#include <stddef.h>

typedef struct {
	size_t	size;
	size_t	len;
	size_t	item_size;
	void	*buffer;
} ResizableArray;

#define	RARR_ELEMENT(rarr, idx, type)	(*(type *)((uint8_t *)((rarr)->buffer) + idx * (rarr)->item_size))
#define	RARR_SIZE(rarr)					((rarr)->size)
#define	RARR_LEN(rarr)					((rarr)->len)
#define	RARR_ITEM_SIZE(rarr)			((rarr)->item_size)

extern void rarr_init(
	ResizableArray *rarr,
	size_t item_size
);
extern void rarr_free(ResizableArray *rarr);
extern void rarr_append_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len
);
extern void rarr_append_with_rarr(
	ResizableArray *dst,
	ResizableArray *src
);
extern void rarr_insert_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len,
	size_t pos
);
extern void rarr_insert_with_rarr(
	ResizableArray *dst,
	ResizableArray *src,
	size_t pos
);
extern void rarr_prepend_with_raw(
	ResizableArray *dst,
	void *src,
	size_t len
);
extern void rarr_prepend_with_rarr(
	ResizableArray *dst,
	ResizableArray *src
);




extern void rarr_test(void);

#endif
