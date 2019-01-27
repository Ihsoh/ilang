#ifndef __STRING_H__
#define __STRING_H__

#include <stdbool.h>

#include "list.h"

#define	RSTR_CSTR(rstr)					((rstr)->buffer == NULL ? "" : (rstr)->buffer)
#define	RSTR_LEN(rstr)					((rstr)->len)
#define	RSTR_CHAR(rstr, idx)			((idx) >= RSTR_LEN((rstr)) ? -1 : RSTR_CSTR((rstr))[(idx)])

#define	RSTR_SPACE_CHAR_COMPARER		(rstr_space_char_comparer)

typedef bool (* RstrCharComparer)(char chr);

typedef struct {
	int		size;
	int		len;
	char	*buffer;
} ResizableString;

extern void rstr_init(ResizableString *rstr);
extern void rstr_init_with_cstr(
	ResizableString *rstr,
	const char *cstr
);
extern void rstr_init_with_char(
	ResizableString *rstr,
	char chr
);
extern void rstr_init_with_raw(
	ResizableString *rstr,
	const char *cstr,
	int len
);
extern void rstr_init_with_rstr(
	ResizableString *dst,
	ResizableString *src
);

extern ResizableString * rstr_new(void);
extern ResizableString * rstr_new_with_cstr(const char *cstr);
extern ResizableString * rstr_new_with_char(char chr);
extern ResizableString * rstr_new_with_raw(
	const char *cstr,
	int len
);
extern ResizableString * rstr_new_with_rstr(ResizableString *src);

extern void rstr_free(ResizableString *rstr);
extern void rstr_delete(ResizableString *rstr);

extern void rstr_append_with_cstr(ResizableString *dst, const char *src);
extern void rstr_append_with_char(ResizableString *dst, char chr);
extern void rstr_append_with_raw(ResizableString *dst, const char *src, int len);
extern void rstr_append_with_rstr(ResizableString *dst, const ResizableString *src);
extern void rstr_appendf(ResizableString *dst, const char *fmt, ...);

extern void rstr_insert_with_cstr(ResizableString *dst, const char *src, int pos);
extern void rstr_insert_with_char(ResizableString *dst, char chr, int pos);
extern void rstr_insert_with_raw(ResizableString *dst, const char *src, unsigned int len, int pos);
extern void rstr_insert_with_rstr(ResizableString *dst, const ResizableString *src, int pos);
extern void rstr_insertf(ResizableString *dst, int pos, const char *fmt, ...);

extern void rstr_prepend_with_cstr(ResizableString *dst, const char *src);
extern void rstr_prepend_with_char(ResizableString *dst, char chr);
extern void rstr_prepend_with_raw(ResizableString *dst, const char *src, unsigned int len);
extern void rstr_prepend_with_rstr(ResizableString *dst, const ResizableString *src);
extern void rstr_prependf(ResizableString *dst, const char *fmt, ...);




extern bool rstr_compare(ResizableString *a, ResizableString *b);

extern void rstr_split(
	LinkedList *llst,
	ResizableString *rstr,
	const char *seperator,
	bool ignore_empty
);
extern void rstr_split_by_char(
	LinkedList *llst,
	ResizableString *rstr,
	char seperator,
	bool ignore_empty
);

extern int rstr_index_of(
	ResizableString *rstr,
	const char *search_value
);
extern int rstr_last_index_of(
	ResizableString *rstr,
	const char *search_value
);

extern void rstr_sub_string(
	ResizableString *dst,
	ResizableString *src,
	size_t start,
	size_t len
);

extern void rstr_clear(ResizableString *rstr);

extern bool rstr_space_char_comparer(char chr);

extern void rstr_ltrim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
);
extern void rstr_rtrim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
);
extern void rstr_trim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
);

extern void rstr_ltrim(
	ResizableString *dst,
	ResizableString *src,
	char chr
);
extern void rstr_rtrim(
	ResizableString *dst,
	ResizableString *src,
	char chr
);
extern void rstr_trim(
	ResizableString *dst,
	ResizableString *src,
	char chr
);




extern void rstr_test(void);

#endif
