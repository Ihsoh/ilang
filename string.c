#include "string.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "myassert.h"
#include "list.h"

void rstr_init(ResizableString *rstr) {
	assert(rstr);

	rstr->size = 0;
	rstr->len = 0;
	rstr->buffer = NULL;
}

void rstr_init_with_cstr(
	ResizableString *rstr,
	const char *cstr
) {
	assert(rstr);
	assert(cstr);

	rstr_init(rstr);
	rstr_append_with_cstr(rstr, cstr);
}

void rstr_init_with_char(
	ResizableString *rstr,
	char chr
) {
	assert(rstr);

	rstr_init(rstr);
	rstr_append_with_char(rstr, chr);
}

void rstr_init_with_raw(
	ResizableString *rstr,
	const char *cstr,
	int len
) {
	assert(rstr);
	assert(cstr);
	assert(len >= 0);

	rstr_init(rstr);
	rstr_append_with_raw(rstr, cstr, len);
}

void rstr_init_with_rstr(
	ResizableString *dst,
	ResizableString *src
) {
	assert(dst);
	assert(src);

	rstr_init(dst);
	rstr_append_with_rstr(dst, src);
}




ResizableString * rstr_new(void) {
	ResizableString *rstr = (ResizableString *)malloc(sizeof(ResizableString));
	rstr_init(rstr);
	return rstr;
}

ResizableString * rstr_new_with_cstr(const char *cstr) {
	assert(cstr);

	ResizableString *rstr = rstr_new();
	rstr_append_with_cstr(rstr, cstr);
	return rstr;
}

ResizableString * rstr_new_with_char(char chr) {
	ResizableString *rstr = rstr_new();
	rstr_append_with_char(rstr, chr);
	return rstr;
}

ResizableString * rstr_new_with_raw(
	const char *cstr,
	int len
) {
	ResizableString *rstr = rstr_new();
	rstr_append_with_raw(rstr, cstr, len);
	return rstr;
}

ResizableString * rstr_new_with_rstr(ResizableString *src) {
	ResizableString *rstr = rstr_new();
	rstr_append_with_rstr(rstr, src);
	return rstr;
}




void rstr_free(ResizableString *rstr) {
	assert(rstr);

	if (rstr->buffer != NULL) {
		free(rstr->buffer);
	}
	rstr->size = 0;
	rstr->len = 0;
	rstr->buffer = NULL;
}

void rstr_delete(ResizableString *rstr) {
	assert(rstr);

	rstr_free(rstr);

	free(rstr);
}

static void _inc_buffer_size(ResizableString *rstr, int inc_size) {
	assert(rstr);
	assert(inc_size > 0);

	rstr->size = rstr->size + 2 * inc_size;
	if (rstr->buffer == NULL) {
		rstr->buffer = (char *)malloc(rstr->size + 1);
	} else {
		rstr->buffer = (char *)realloc(rstr->buffer, rstr->size + 1);
	}
}

static void _append_null_terminated_char(ResizableString *dst) {
	dst->buffer[dst->len] = '\0';
}

void rstr_append_with_cstr(ResizableString *dst, const char *src) {
	assert(dst);
	assert(src);

	int len_src = strlen(src);
	if (len_src == 0) {
		return;
	}

	if (dst->len + len_src > dst->size) {
		_inc_buffer_size(dst, len_src);
	}

	memcpy(dst->buffer + dst->len, src, len_src);
	dst->len += len_src;

	_append_null_terminated_char(dst);
}

void rstr_append_with_char(ResizableString *dst, char chr) {
	char temp[2] = {'\0', '\0'};
	temp[0] = chr;

	rstr_append_with_cstr(dst, temp);
}

void rstr_append_with_raw(ResizableString *dst, const char *src, int len) {
	assert(dst);
	assert(src);
	assert(len >= 0);

	if (len == 0) {
		return;
	}

	if (dst->len + len > dst->size) {
		_inc_buffer_size(dst, len);
	}

	memcpy(dst->buffer + dst->len, src, len);
	dst->len += len;

	_append_null_terminated_char(dst);
}

void rstr_append_with_rstr(ResizableString *dst, const ResizableString *src) {
	assert(dst);
	assert(src);

	if (src->len == 0) {
		return;
	}

	if (dst->len + src->len > dst->size) {
		_inc_buffer_size(dst, src->len);
	}

	memcpy(dst->buffer + dst->len, src->buffer, src->len);
	dst->len += src->len;

	_append_null_terminated_char(dst);
}

void rstr_appendf(ResizableString *dst, const char *fmt, ...) {
	char buffer[64 * 1024];

	va_list argptr;
    va_start(argptr, fmt);

	vsnprintf(buffer, sizeof(buffer), fmt, argptr);
	rstr_append_with_cstr(dst, buffer);

	va_end(argptr);
}

void rstr_insert_with_cstr(ResizableString *dst, const char *src, int pos) {
	assert(dst);
	assert(src);
	assert(pos >= 0);

	int len_src = strlen(src);
	if (len_src == 0) {
		return;
	}

	if (dst->len + len_src > dst->size) {
		_inc_buffer_size(dst, len_src);
	}

	memcpy(dst->buffer + pos + len_src, dst->buffer + pos, dst->len - pos);
	memcpy(dst->buffer + pos, src, len_src);
	dst->len += len_src;

	_append_null_terminated_char(dst);
}

void rstr_insert_with_char(ResizableString *dst, char chr, int pos) {
	char temp[2] = {'\0', '\0'};
	temp[0] = chr;

	rstr_insert_with_cstr(dst, temp, pos);
}

void rstr_insert_with_raw(ResizableString *dst, const char *src, unsigned int len, int pos) {
	assert(dst);
	assert(src);
	assert(len >= 0);
	assert(pos >= 0);

	if (len == 0) {
		return;
	}

	if (dst->len + len > dst->size) {
		_inc_buffer_size(dst, len);
	}

	memcpy(dst->buffer + pos + len, dst->buffer + pos, dst->len - pos);
	memcpy(dst->buffer + pos, src, len);
	dst->len += len;

	_append_null_terminated_char(dst);
}

void rstr_insert_with_rstr(ResizableString *dst, const ResizableString *src, int pos) {
	assert(dst);
	assert(src);
	assert(pos >= 0);

	if (src->len == 0) {
		return;
	}

	if (dst->len + src->len > dst->size) {
		_inc_buffer_size(dst, src->len);
	}

	memcpy(dst->buffer + pos + src->len, dst->buffer + pos, dst->len - pos);
	memcpy(dst->buffer + pos, src->buffer, src->len);
	dst->len += src->len;

	_append_null_terminated_char(dst);
}

void rstr_insertf(ResizableString *dst, int pos, const char *fmt, ...) {
	char buffer[64 * 1024];

	va_list argptr;
    va_start(argptr, fmt);

	vsnprintf(buffer, sizeof(buffer), fmt, argptr);
	rstr_insert_with_cstr(dst, buffer, pos);

	va_end(argptr);
}

void rstr_prepend_with_cstr(ResizableString *dst, const char *src) {
	rstr_insert_with_cstr(dst, src, 0);
}

void rstr_prepend_with_char(ResizableString *dst, char chr) {
	char temp[2] = {'\0', '\0'};
	temp[0] = chr;

	rstr_prepend_with_cstr(dst, temp);
}

void rstr_prepend_with_raw(ResizableString *dst, const char *src, unsigned int len) {
	rstr_insert_with_raw(dst, src, len, 0);
}

void rstr_prepend_with_rstr(ResizableString *dst, const ResizableString *src) {
	rstr_insert_with_rstr(dst, src, 0);
}

void rstr_prependf(ResizableString *dst, const char *fmt, ...) {
	char buffer[64 * 1024];

	va_list argptr;
    va_start(argptr, fmt);

	vsnprintf(buffer, sizeof(buffer), fmt, argptr);
	rstr_prepend_with_cstr(dst, buffer);

	va_end(argptr);
}




bool rstr_compare(ResizableString *a, ResizableString *b) {
	return a != NULL
		&& b != NULL
		&& a->len == b->len
		&& (a->buffer == NULL || strcmp(a->buffer, b->buffer) == 0);
}

void rstr_split(
	LinkedList *llst,
	ResizableString *rstr,
	const char *seperator,
	bool ignore_empty
) {
	assert(llst);
	assert(rstr);
	assert(seperator);

	size_t sep_len = strlen(seperator);

	#define	_GET_STR(i) ((i) >= rstr->len ? NULL : &(rstr->buffer[(i)]))

	int idx = 0;
	while (_GET_STR(idx) != NULL) {
		ResizableString *item = rstr_new();

		while (_GET_STR(idx) != NULL && strncmp(_GET_STR(idx), seperator, sep_len) != 0) {
			rstr_append_with_char(item, *_GET_STR(idx));
			idx++;
		}

		if (_GET_STR(idx) != NULL && strncmp(_GET_STR(idx), seperator, sep_len) == 0) {
			idx += sep_len;
		}

		if (ignore_empty
				&& item->len == 0) {
			rstr_delete(item);
		} else {
			llst_append(llst, item);
		}
	}

	#undef	_GET_STR
}

void rstr_split_by_char(
	LinkedList *llst,
	ResizableString *rstr,
	char seperator,
	bool ignore_empty
) {
	assert(llst);
	assert(rstr);

	char temp[2] = {'\0', '\0'};
	temp[0] = seperator;

	rstr_split(llst, rstr, temp, ignore_empty);
}

int rstr_index_of(
	ResizableString *rstr,
	const char *search_value
) {
	assert(rstr);
	assert(search_value);

	size_t search_value_len = strlen(search_value);
	if (search_value_len == 0) {
		return -1;
	}

	for (int i = 0; RSTR_CSTR(rstr)[i] != '\0'; i++) {
		if (strncmp(&(RSTR_CSTR(rstr)[i]), search_value, search_value_len) == 0) {
			return i;
		}
	}

	return -1;
}

int rstr_last_index_of(
	ResizableString *rstr,
	const char *search_value
) {
	assert(rstr);
	assert(search_value);

	size_t search_value_len = strlen(search_value);
	if (search_value_len == 0) {
		return -1;
	}

	size_t len = RSTR_LEN(rstr);

	for (int i = len; i >= 0; i--) {
		if (strncmp(&(RSTR_CSTR(rstr)[i]), search_value, search_value_len) == 0) {
			return i;
		}
	}

	return -1;
}

void rstr_sub_string(
	ResizableString *dst,
	ResizableString *src,
	size_t start,
	size_t len
) {
	assert(dst);
	assert(src);
	assert(dst != src);

	for (int i = start; i < start + len && RSTR_CHAR(src, i) != -1; i++) {
		rstr_append_with_char(dst, RSTR_CHAR(src, i));
	}
}

void rstr_clear(ResizableString *rstr) {
	assert(rstr);

	rstr->len = 0;

	_append_null_terminated_char(rstr);
}

bool rstr_space_char_comparer(char chr) {
	if (isspace(chr)) {
		return true;
	} else {
		return false;
	}
}

void rstr_ltrim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
) {
	assert(dst);
	assert(src);
	assert(dst != src);
	assert(comparer);

	int start = 0;
	for (; start < RSTR_LEN(src) && comparer(RSTR_CHAR(src, start)); start++);

	rstr_sub_string(dst, src, start, RSTR_LEN(src) - start);
}

void rstr_rtrim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
) {
	assert(dst);
	assert(src);
	assert(dst != src);
	assert(comparer);

	int len = RSTR_LEN(src);
	for (; len > 0 && comparer(RSTR_CHAR(src, len - 1)); len--);

	rstr_sub_string(dst, src, 0, len);
}

void rstr_trim_ex(
	ResizableString *dst,
	ResizableString *src,
	RstrCharComparer comparer
) {
	assert(dst);
	assert(src);
	assert(dst != src);
	assert(comparer);

	int start = 0;
	for (; start < RSTR_LEN(src) && comparer(RSTR_CHAR(src, start)); start++);

	int len = RSTR_LEN(src);
	for (; len > 0 && comparer(RSTR_CHAR(src, len - 1)); len--);

	rstr_sub_string(dst, src, start, len - start);
}

void rstr_ltrim(
	ResizableString *dst,
	ResizableString *src,
	char chr
) {
	assert(dst);
	assert(src);
	assert(dst != src);

	int start = 0;
	for (; start < RSTR_LEN(src) && RSTR_CHAR(src, start) == chr; start++);

	rstr_sub_string(dst, src, start, RSTR_LEN(src) - start);
}

void rstr_rtrim(
	ResizableString *dst,
	ResizableString *src,
	char chr
) {
	assert(dst);
	assert(src);
	assert(dst != src);

	int len = RSTR_LEN(src);
	for (; len > 0 && RSTR_CHAR(src, len - 1) == chr; len--);

	rstr_sub_string(dst, src, 0, len);
}

void rstr_trim(
	ResizableString *dst,
	ResizableString *src,
	char chr
) {
	assert(dst);
	assert(src);
	assert(dst != src);

	int start = 0;
	for (; start < RSTR_LEN(src) && RSTR_CHAR(src, start) == chr; start++);

	int len = RSTR_LEN(src);
	for (; len > 0 && RSTR_CHAR(src, len - 1) == chr; len--);

	rstr_sub_string(dst, src, start, len - start);
}



void rstr_test(void) {
	ResizableString rstr1;

	rstr_init(&rstr1);
	MYASSERT(
		rstr1.size == 0 && rstr1.len == 0 && rstr1.buffer == NULL,
		"TEST rstr_init(...)"
	);

	#define	__CMP_RSTR_CSTR(rstr, cstr, msg)	{	\
		MYASSERT(	\
			sizeof((cstr)) - 1 == (rstr).len && strncmp((rstr).buffer, (cstr), sizeof((cstr)) - 1) == 0,	\
			(msg)	\
		);	\
	}

	// "" + "AB" => "AB"
	rstr_append_with_cstr(&rstr1, "AB");
	__CMP_RSTR_CSTR(rstr1, "AB", "TEST rstr_append_with_cstr(...)");

	// "AB" + "CD" => "ABCD"
	rstr_append_with_raw(&rstr1, "CD", sizeof("CD") - 1);
	__CMP_RSTR_CSTR(rstr1, "ABCD", "TEST rstr_append_with_raw(...)");

	// "ABCD" + "EF" => "ABCDEF"
	ResizableString rstr2;
	rstr_init(&rstr2);
	rstr_append_with_cstr(&rstr2, "EF");
	rstr_append_with_rstr(&rstr1, &rstr2);
	__CMP_RSTR_CSTR(rstr1, "ABCDEF", "TEST rstr_append_with_rstr(...)");
	rstr_free(&rstr2);
	MYASSERT(
		rstr2.size == 0 && rstr2.len == 0 && rstr2.buffer == NULL,
		"TEST rstr_free(...)"
	);

	// INSERT("ABCDEF", 1, "ab") => "AabBCDEF"
	rstr_insert_with_cstr(&rstr1, "ab", 1);
	__CMP_RSTR_CSTR(rstr1, "AabBCDEF", "TEST rstr_insert_with_cstr(...)");

	// INSERT("AabBCDEF", 4, "bc") => "AabBbcCDEF"
	rstr_insert_with_raw(&rstr1, "bc", sizeof("bc") - 1, 4);
	__CMP_RSTR_CSTR(rstr1, "AabBbcCDEF", "TEST rstr_insert_with_raw(...)");

	// INSERT("AabBbcCDEF", 7, "cd") => "AabBbcCcdDEF"
	ResizableString rstr3;
	rstr_init(&rstr3);
	rstr_append_with_cstr(&rstr3, "cd");
	rstr_insert_with_rstr(&rstr1, &rstr3, 7);
	__CMP_RSTR_CSTR(rstr1, "AabBbcCcdDEF", "TEST rstr_insert_with_rstr(...)");
	rstr_free(&rstr3);

	// "12" + "AabBbcCcdDEF" => "12AabBbcCcdDEF"
	rstr_prepend_with_cstr(&rstr1, "12");
	__CMP_RSTR_CSTR(rstr1, "12AabBbcCcdDEF", "TEST rstr_prepend_with_cstr(...)");

	// "34" + "12AabBbcCcdDEF" => "3412AabBbcCcdDEF"
	rstr_prepend_with_raw(&rstr1, "34", sizeof("34") - 1);
	__CMP_RSTR_CSTR(rstr1, "3412AabBbcCcdDEF", "TEST rstr_prepend_with_raw(...)");

	// "56" + "3412AabBbcCcdDEF" => "563412AabBbcCcdDEF"
	ResizableString rstr4;
	rstr_init(&rstr4);
	rstr_append_with_cstr(&rstr4, "56");
	rstr_prepend_with_rstr(&rstr1, &rstr4);
	rstr_free(&rstr4);
	__CMP_RSTR_CSTR(rstr1, "563412AabBbcCcdDEF", "TEST rstr_prepend_with_rstr(...)");

	rstr_free(&rstr1);

	rstr_init(&rstr1);

	// "" + 'A' + 'B' => "AB"
	rstr_append_with_char(&rstr1, 'A');
	rstr_append_with_char(&rstr1, 'B');
	__CMP_RSTR_CSTR(rstr1, "AB", "TEST rstr_append_with_char(...)");

	// INSERT("AB", 1, 'C') => "ACB"
	rstr_insert_with_char(&rstr1, 'C', 1);
	__CMP_RSTR_CSTR(rstr1, "ACB", "TEST rstr_insert_with_char(...)");

	// 'D' + "ACB" => "DACB"
	rstr_prepend_with_char(&rstr1, 'D');
	__CMP_RSTR_CSTR(rstr1, "DACB", "TEST rstr_prepend_with_char(...)");

	MYASSERT(
		rstr1.buffer[rstr1.len] == '\0',
		"TEST null-terminated character"
	);

	rstr_free(&rstr1);


	LinkedList llst1;

	// SPLIT(str = '', sep = ';', ignore_empty = true)
	rstr_init(&rstr1);
	llst_init(&llst1, NULL, (LinkedListReleaser) rstr_delete);
	rstr_append_with_cstr(&rstr1, "");
	rstr_split(&llst1, &rstr1, ";", true);
	MYASSERT(
		llst1.count == 0,
		"TEST rstr_split(..., '', ';', true)"
	);
	llst_free(&llst1, true);
	rstr_free(&rstr1);

	// SPLIT(str = '', sep = ';', ignore_empty = false)
	rstr_init(&rstr1);
	llst_init(&llst1, NULL, (LinkedListReleaser) rstr_delete);
	rstr_append_with_cstr(&rstr1, "");
	rstr_split(&llst1, &rstr1, ";", false);
	MYASSERT(
		llst1.count == 0,
		"TEST rstr_split(..., '', ';', false)"
	);
	llst_free(&llst1, true);
	rstr_free(&rstr1);

	// SPLIT(str = 'AB;CD;EF', sep = ';', ignore_empty = false)
	rstr_init(&rstr1);
	llst_init(&llst1, NULL, (LinkedListReleaser) rstr_delete);
	rstr_append_with_cstr(&rstr1, "AB;CD;EF");
	rstr_split(&llst1, &rstr1, ";", false);
	MYASSERT(
		llst1.count == 3,
		"TEST rstr_split(..., ..., ..., false)"
	);
	llst_free(&llst1, true);
	rstr_free(&rstr1);


	// INDEX_OF('a b c', ' ')
	rstr_init_with_cstr(&rstr1, "a b c");
	MYASSERT(
		rstr_index_of(&rstr1, " ") == 1,
		"TEST rstr_index_of(...)"
	);
	rstr_free(&rstr1);

	// LAST_INDEX_OF('a b c', ' ')
	rstr_init_with_cstr(&rstr1, "a b c");
	MYASSERT(
		rstr_last_index_of(&rstr1, " ") == 3,
		"TEST rstr_last_index_of(...)"
	);
	rstr_free(&rstr1);

	// SUB_STRING('abcdef', 1, 4) => 'bcde'
	rstr_init_with_cstr(&rstr1, "abcdef");
	ResizableString rstr5;
	rstr_init(&rstr5);
	rstr_sub_string(&rstr5, &rstr1, 1, 4);
	__CMP_RSTR_CSTR(rstr5, "bcde", "TEST rstr_sub_string(...)");
	rstr_free(&rstr5);
	rstr_free(&rstr1);

	// LTRAM('   abc  ', ' ')
	rstr_init_with_cstr(&rstr1, "   abc  ");
	ResizableString rstr7;
	rstr_init(&rstr7);
	rstr_ltrim(&rstr7, &rstr1, ' ');
	__CMP_RSTR_CSTR(rstr7, "abc  ", "TEST rstr_ltrim(...)");
	rstr_free(&rstr7);
	rstr_free(&rstr1);

	// RTRAM('   abc  ', ' ')
	rstr_init_with_cstr(&rstr1, "   abc  ");
	ResizableString rstr8;
	rstr_init(&rstr8);
	rstr_rtrim(&rstr8, &rstr1, ' ');
	__CMP_RSTR_CSTR(rstr8, "   abc", "TEST rstr_rtrim(...)");
	rstr_free(&rstr8);
	rstr_free(&rstr1);

	// TRAM('   abc  ', ' ')
	rstr_init_with_cstr(&rstr1, "   abc  ");
	ResizableString rstr9;
	rstr_init(&rstr9);
	rstr_trim(&rstr9, &rstr1, ' ');
	__CMP_RSTR_CSTR(rstr9, "abc", "TEST rstr_rtrim(...)");
	rstr_free(&rstr9);
	rstr_free(&rstr1);

	// "X" + STR(123456) => "X123456"
	rstr_init_with_cstr(&rstr1, "X");
	rstr_appendf(&rstr1, "%d", 123456);
	__CMP_RSTR_CSTR(rstr1, "X123456", "TEST rstr_appendf(...)");
	rstr_free(&rstr1);

	// INSERT("AB", 1, STR(123456)) => "A123456B"
	rstr_init_with_cstr(&rstr1, "AB");
	rstr_insertf(&rstr1, 1, "%d", 123456);
	__CMP_RSTR_CSTR(rstr1, "A123456B", "TEST rstr_insertf(...)");
	rstr_free(&rstr1);

	// STR(123456) + "X" => "123456X"
	rstr_init_with_cstr(&rstr1, "X");
	rstr_prependf(&rstr1, "%d", 123456);
	__CMP_RSTR_CSTR(rstr1, "123456X", "TEST rstr_prependf(...)");
	rstr_free(&rstr1);

	#undef	__CMP_RSTR_CSTR
}
