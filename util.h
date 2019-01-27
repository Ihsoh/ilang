#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <inttypes.h>

#include "lexer.h"

#include "string.h"

#define	UTIL_IS_LITERAL_UINT_BIN_CHAR(chr)	((chr) == '0' || (chr) == '1')
#define	UTIL_IS_LITERAL_UINT_OCT_CHAR(chr)	((chr) >= '0' && (chr) <= '7')
#define	UTIL_IS_LITERAL_UINT_DEC_CHAR(chr)	((chr) >= '0' && (chr) <= '9')
#define	UTIL_IS_LITERAL_UINT_HEX_CHAR(chr)	\
	(((chr) >= '0' && (chr) <= '9') || ((chr) >= 'a' && (chr) <= 'f') || ((chr) >= 'A' && (chr) <= 'F'))

extern int32_t util_get_file_size(const char *path);
extern int32_t util_put_file_content(const char *path, const char *content, int32_t len);
extern const char * util_get_file_content(const char *path, int32_t *file_size);

extern bool util_is_identifier_item(char chr);
extern bool util_is_literal_char_item(char chr);
extern bool util_is_literal_string_item(char chr);
extern bool util_is_literal_char_esc(char chr);
extern bool util_is_literal_string_esc(char chr);
extern char util_unescape(char chr);

extern bool util_is_literal_char(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_string(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_uint_bin(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_int_bin(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_uint_oct(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_int_oct(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_uint_dec(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_int_dec(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_uint_hex(
	const char *str,
	uint32_t len
);
extern bool util_is_literal_int_hex(
	const char *str,
	uint32_t len
);

extern uint64_t util_get_uint64_char_val(const char *str, uint32_t len);
extern uint32_t util_get_uint32_char_val(const char *str, uint32_t len);
extern unsigned int util_get_uint_char_val(const char *str, uint32_t len);
extern char util_get_char_val(const char *str, uint32_t len);

extern void util_get_str_val(
	ResizableString *rstr,
	const char *str,
	uint32_t len
);

extern uint64_t util_get_uint64_bin_val(const char *str, uint32_t len);
extern uint32_t util_get_uint32_bin_val(const char *str, uint32_t len);
extern unsigned int util_get_uint_bin_val(const char *str, uint32_t len);

extern int64_t util_get_int64_bin_val(const char *str, uint32_t len);
extern int32_t util_get_int32_bin_val(const char *str, uint32_t len);
extern int util_get_int_bin_val(const char *str, uint32_t len);

extern uint64_t util_get_uint64_oct_val(const char *str, uint32_t len);
extern uint32_t util_get_uint32_oct_val(const char *str, uint32_t len);
extern unsigned int util_get_uint_oct_val(const char *str, uint32_t len);

extern int64_t uint_get_int64_oct_val(const char *str, uint32_t len);
extern int32_t uint_get_int32_oct_val(const char *str, uint32_t len);
extern int util_get_int_oct_val(const char *str, uint32_t len);

extern uint64_t util_get_uint64_dec_val(const char *str, uint32_t len);
extern uint32_t util_get_uint32_dec_val(const char *str, uint32_t len);
extern unsigned int util_get_uint_dec_val(const char *str, uint32_t len);

extern int64_t util_get_int64_dec_val(const char *str, uint32_t len);
extern int32_t util_get_int32_dec_val(const char *str, uint32_t len);
extern int util_get_int_dec_val(const char *str, uint32_t len);

extern uint64_t util_get_uint64_hex_val(const char *str, uint32_t len);
extern uint32_t util_get_uint32_hex_val(const char *str, uint32_t len);
extern unsigned int util_get_uint_hex_val(const char *str, uint32_t len);

extern int64_t util_get_int64_hex_val(const char *str, uint32_t len);
extern int32_t util_get_int32_hex_val(const char *str, uint32_t len);
extern int util_get_int_hex_val(const char *str, uint32_t len);

extern double util_get_double_val(const char *str, uint32_t len);
extern float util_get_float_val(const char *str, uint32_t len);

extern void util_unescape_string(
	ResizableString *target,
	const char *source,
	size_t source_len
);

#endif
