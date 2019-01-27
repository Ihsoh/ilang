#include "util.h"

#include <sys/stat.h>

#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"

#include "string.h"

int32_t util_get_file_size(const char *path) {
	assert(path);

	struct stat statbuff;
	if (stat(path, &statbuff) < 0) {
		return -1;
	}
	
	if (statbuff.st_size > INT_MAX) {
		return -1;
	}

	return (int32_t)statbuff.st_size;
}

int32_t util_put_file_content(const char *path, const char *content, int32_t len) {
	assert(path);
	assert(content);
	assert(len >= 0);

	FILE *file = NULL;

	file = fopen(path, "w+");
	if (file == NULL) {
		goto err;
	}

	if (len > 0) {
		int32_t c = len;
		while ((c -= fwrite(content, sizeof(char), c, file)) > 0);
	}

	fclose(file);

err:
	if (file != NULL) {
		fclose(file);
	}
	return -1;
}

const char * util_get_file_content(const char *path, int32_t *file_size) {
	assert(path);
	assert(file_size);

	char *content = NULL;
	FILE *file = NULL;

	int32_t fsize = util_get_file_size(path);
	if (fsize < 0) {
		*file_size = -1;
		goto err;
	} else {
		*file_size = fsize;
		if (fsize == 0) {
			return NULL;
		}
	}

	content = (char *)malloc(fsize);
	if (content == NULL) {
		goto err;
	}

	file = fopen(path, "r");
	if (file == NULL) {
		goto err;
	}

	int32_t c = fsize;
	while ((c -= fread(content, sizeof(char), c, file)) > 0);

	fclose(file);

	return content;

err:
	if (content != NULL) {
		free(content);
	}
	if (file != NULL) {
		fclose(file);
	}
	return NULL;
}




bool util_is_identifier_item(char chr) {
	return isalnum(chr) || chr == '_';
}

bool util_is_literal_char_item(char chr) {
	return chr != '\'' && (isblank(chr) || isgraph(chr));
}

bool util_is_literal_string_item(char chr) {
	return chr != '"' && (isblank(chr) || isgraph(chr));
}

bool util_is_literal_char_esc(char chr) {
	static const char esc_list[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\'', '0', 0};

	for (int i = 0; esc_list[i] != 0; i++) {
		if (esc_list[i] == chr) {
			return true;
		}
	}

	return false;
}

bool util_is_literal_string_esc(char chr) {
	static const char esc_list[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '"', '0', 0};

	for (int i = 0; esc_list[i] != 0; i++) {
		if (esc_list[i] == chr) {
			return true;
		}
	}

	return false;
}

char util_unescape(char chr) {
	static const char esc_map[][2] = {
		{'a', '\a'},
		{'b', '\b'},
		{'f', '\f'},
		{'n', '\n'},
		{'r', '\r'},
		{'t', '\t'},
		{'v', '\v'},
		{'\\', '\\'},
		{'\'', '\''},
		{'"', '"'},
		{'0', '\0'},
		{-1, -1}
	};

	for (int i = 0; esc_map[i][0] != -1; i++) {
		if (esc_map[i][0] == chr) {
			return esc_map[i][1];
		}
	}

	return -1;
}

bool util_is_literal_char(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len == 3) {
		return str[0] == '\''
			&& util_is_literal_char_item(str[1])
			&& str[2] == '\'';
	} else if (len == 4) {
		return str[0] == '\''
			&& str[1] == '\\'
			&& util_is_literal_char_esc(str[2])
			&& str[3] == '\'';
	} else {
		return false;
	}
}

bool util_is_literal_string(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len >= 2) {
		if (str[0] != '"'
				|| str[len - 1] != '"') {
			return false;
		}

		for (size_t i = 1; i < len - 2; i++) {
			if (!util_is_literal_string_item(str[i])
					&& str[i] != '\\'
					&& i + 1 >= len - 2
					&& !util_is_literal_string_esc(str[i + 1])) {
				return false;
			}
		}

		return true;
	} else {
		return false;
	}
}

bool util_is_literal_uint_bin(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len >= 3) {
		if (str[0] == '0'
				&& str[1] == 'b'
				&& UTIL_IS_LITERAL_UINT_BIN_CHAR(str[2])) {
			for (int i = 3; i < len; i++) {
				if (!UTIL_IS_LITERAL_UINT_BIN_CHAR(str[i])
						&& str[i] != '_') {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool util_is_literal_int_bin(
	const char *str,
	uint32_t len
) {
	assert(str && len > 0);

	if (*str == '+' || *str == '-') {
		return util_is_literal_uint_bin(str + 1, len - 1);
	} else {
		return util_is_literal_uint_bin(str, len);
	}
}

bool util_is_literal_uint_oct(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len >= 3) {
		if (str[0] == '0'
				&& str[1] == 'o'
				&& UTIL_IS_LITERAL_UINT_OCT_CHAR(str[2])) {
			for (int i = 3; i < len; i++) {
				if (!UTIL_IS_LITERAL_UINT_OCT_CHAR(str[i])
						&& str[i] != '_') {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool util_is_literal_int_oct(
	const char *str,
	uint32_t len
) {
	assert(str && len > 0);

	if (*str == '+' || *str == '-') {
		return util_is_literal_uint_oct(str + 1, len - 1);
	} else {
		return util_is_literal_uint_oct(str, len);
	}
}

bool util_is_literal_uint_dec(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len >= 1) {
		if (UTIL_IS_LITERAL_UINT_DEC_CHAR(str[0])) {
			for (int i = 1; i < len; i++) {
				if (!UTIL_IS_LITERAL_UINT_DEC_CHAR(str[i])
						&& str[i] != '_') {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool util_is_literal_int_dec(
	const char *str,
	uint32_t len
) {
	assert(str && len > 0);

	if (*str == '+' || *str == '-') {
		return util_is_literal_uint_dec(str + 1, len - 1);
	} else {
		return util_is_literal_uint_dec(str, len);
	}
}

bool util_is_literal_uint_hex(
	const char *str,
	uint32_t len
) {
	assert(str);

	if (len >= 3) {
		if (str[0] == '0'
				&& str[1] == 'x'
				&& UTIL_IS_LITERAL_UINT_HEX_CHAR(str[2])) {
			for (int i = 3; i < len; i++) {
				if (!UTIL_IS_LITERAL_UINT_HEX_CHAR(str[i])
						&& str[i] != '_') {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool util_is_literal_int_hex(
	const char *str,
	uint32_t len
) {
	assert(str && len > 0);

	if (*str == '+' || *str == '-') {
		return util_is_literal_uint_hex(str + 1, len - 1);
	} else {
		return util_is_literal_uint_hex(str, len);
	}
}




uint64_t util_get_uint64_char_val(const char *str, uint32_t len) {
	assert(str);
	
	if (!util_is_literal_char(str, len)) {
		assert(0);
		return UINT64_MAX;
	}

	if (len == 3) {
		return str[1];
	} else if (len == 4) {
		char chr = util_unescape(str[2]);
		if (chr == -1) {
			assert(0);
			return UINT64_MAX;
		}
		return chr;
	}

	assert(0);
	return UINT64_MAX;
}

uint32_t util_get_uint32_char_val(const char *str, uint32_t len) {
	return (uint32_t) (util_get_uint64_char_val(str, len) & 0xffffffff);
}

unsigned int util_get_uint_char_val(const char *str, uint32_t len) {
	return (unsigned int) util_get_uint64_char_val(str, len);
}

char util_get_char_val(const char *str, uint32_t len) {
	return (char) (util_get_uint64_char_val(str, len) & 0xff);
}




void util_get_str_val(
	ResizableString *rstr,
	const char *str,
	uint32_t len
) {
	assert(rstr);
	assert(str);
	assert(len >= 2);
	assert(str[0] == '"' && str[len - 1] == '"');

	util_unescape_string(rstr, str + 1, len - 2);
}




uint64_t util_get_uint64_bin_val(const char *str, uint32_t len) {
	#define	_IS_BIN_DIGIT(chr)	((chr) >= '0' && (chr) <= '1')

	assert(str && len > 2 && str[0] == '0' && str[1] == 'b' && _IS_BIN_DIGIT(str[2]));

	const char *s = str + len - 1;
	str += 2;

	uint64_t v = 0;
	uint32_t i = 0;
	while (s >= str) {
		const char chr = *(s--);
		if (_IS_BIN_DIGIT(chr)) {
			v |= (chr - '0') << (i++);
		} else if (chr == '_') {
			;
		} else {
			assert(0);
		}
	}

	return v;

	#undef	_IS_BIN_DIGIT
}

uint32_t util_get_uint32_bin_val(const char *str, uint32_t len) {
	return (uint32_t) (util_get_uint64_bin_val(str, len) & 0xffffffff);
}

unsigned int util_get_uint_bin_val(const char *str, uint32_t len) {
	return (unsigned int) util_get_uint64_bin_val(str, len);
}

int64_t util_get_int64_bin_val(const char *str, uint32_t len) {
	assert(str && len > 0);

	if (*str == '+') {
		return (int64_t) util_get_uint64_bin_val(str + 1, len - 1);
	} else if (*str == '-') {
		return -(int64_t) util_get_uint64_bin_val(str + 1, len - 1);
	} else {
		return (int64_t) util_get_uint64_bin_val(str, len);
	}
}

int32_t util_get_int32_bin_val(const char *str, uint32_t len) {
	return (int32_t) util_get_int64_bin_val(str, len);
}

int util_get_int_bin_val(const char *str, uint32_t len) {
	return (int) util_get_int64_bin_val(str, len);
}




uint64_t util_get_uint64_oct_val(const char *str, uint32_t len) {
	#define	_IS_OCT_DIGIT(chr)	((chr) >= '0' && (chr) <= '7')

	assert(str && len > 2 && str[0] == '0' && str[1] == 'o' && _IS_OCT_DIGIT(str[2]));

	const char *s = str + len - 1;
	str += 2;

	uint64_t v = 0;
	uint64_t base = 1;
	while (s >= str) {
		const char chr = *(s--);
		if (_IS_OCT_DIGIT(chr)) {
			v += (chr - '0') * base;
			base *= 8;
		} else if (chr == '_') {
			;
		} else {
			assert(0);
		}
	}

	return v;

	#undef	_IS_OCT_DIGIT
}

uint32_t util_get_uint32_oct_val(const char *str, uint32_t len) {
	return (uint32_t) (util_get_uint64_oct_val(str, len) & 0xffffffff);
}

unsigned int util_get_uint_oct_val(const char *str, uint32_t len) {
	return (unsigned int) util_get_uint64_oct_val(str, len);
}

int64_t uint_get_int64_oct_val(const char *str, uint32_t len) {
	assert(str && len > 0);

	if (*str == '+') {
		return (int64_t) util_get_uint64_oct_val(str + 1, len - 1);
	} else if (*str == '-') {
		return -(int64_t) util_get_uint64_oct_val(str + 1, len - 1);
	} else {
		return (int64_t) util_get_uint64_oct_val(str, len);
	}
}

int32_t uint_get_int32_oct_val(const char *str, uint32_t len) {
	return (int32_t) uint_get_int64_oct_val(str, len);
}

int util_get_int_oct_val(const char *str, uint32_t len) {
	return (int) uint_get_int64_oct_val(str, len);
}




uint64_t util_get_uint64_dec_val(const char *str, uint32_t len) {
	#define	_IS_DEC_DIGIT(chr)	((chr) >= '0' && (chr) <= '9')

	assert(str && len > 0 && _IS_DEC_DIGIT(str[0]));

	const char *s = str + len - 1;

	uint64_t v = 0;
	uint64_t base = 1;
	while (s >= str) {
		const char chr = *(s--);
		if (_IS_DEC_DIGIT(chr)) {
			v += (chr - '0') * base;
			base *= 10;
		} else if (chr == '_') {
			;
		} else {
			assert(0);
		}
	}

	return v;

	#undef	_IS_DEC_DIGIT
}

uint32_t util_get_uint32_dec_val(const char *str, uint32_t len) {
	return (uint32_t) (util_get_uint64_dec_val(str, len) & 0xffffffff);
}

unsigned int util_get_uint_dec_val(const char *str, uint32_t len) {
	return (unsigned int) util_get_uint64_dec_val(str, len);
}

int64_t util_get_int64_dec_val(const char *str, uint32_t len) {
	assert(str && len > 0);

	if (*str == '+') {
		return (int64_t) util_get_uint64_dec_val(str + 1, len - 1);
	} else if (*str == '-') {
		return -(int64_t) util_get_uint64_dec_val(str + 1, len - 1);
	} else {
		return (int64_t) util_get_uint64_dec_val(str, len);
	}
}

int32_t util_get_int32_dec_val(const char *str, uint32_t len) {
	return (int32_t) util_get_int64_dec_val(str, len);
}

int util_get_int_dec_val(const char *str, uint32_t len) {
	return (int) util_get_int64_dec_val(str, len);
}




uint64_t util_get_uint64_hex_val(const char *str, uint32_t len) {
	#define	_IS_HEX_DIGIT(chr)	(((chr) >= '0' && (chr) <= '9') || ((chr) >= 'a' && (chr) <= 'f') || ((chr) >= 'A' && (chr) <= 'F'))

	assert(str && len > 2 && str[0] == '0' && str[1] == 'x' && _IS_HEX_DIGIT(str[2]));

	const char *s = str + len - 1;
	str += 2;

	uint64_t v = 0;
	uint64_t base = 1;
	while (s >= str) {
		const char chr = *(s--);
		if (_IS_HEX_DIGIT(chr)) {
			if (chr >= '0' && chr <= '9') {
				v += (chr - '0') * base;
			} else if (chr >= 'a' && chr <= 'f') {
				v += (10 + (chr - 'a')) * base;
			} else {
				v += (10 + (chr - 'A')) * base;
			}
			base *= 16;
		} else if (chr == '_') {
			;
		} else {
			assert(0);
		}
	}

	return v;

	#undef	_IS_HEX_DIGIT
}

uint32_t util_get_uint32_hex_val(const char *str, uint32_t len) {
	return (uint32_t) (util_get_uint64_hex_val(str, len) & 0xffffffff);
}

unsigned int util_get_uint_hex_val(const char *str, uint32_t len) {
	return (unsigned int) util_get_uint64_hex_val(str, len);
}

int64_t util_get_int64_hex_val(const char *str, uint32_t len) {
	assert(str && len > 0);

	if (*str == '+') {
		return (int64_t) util_get_uint64_hex_val(str + 1, len - 1);
	} else if (*str == '-') {
		return -(int64_t) util_get_uint64_hex_val(str + 1, len - 1);
	} else {
		return (int64_t) util_get_uint64_hex_val(str, len);
	}
}

int32_t util_get_int32_hex_val(const char *str, uint32_t len) {
	return (int32_t) util_get_int64_hex_val(str, len);
}

int util_get_int_hex_val(const char *str, uint32_t len) {
	return (int) util_get_int64_hex_val(str, len);
}




double util_get_double_val(const char *str, uint32_t len) {
	assert(str);
	assert(len > 0);

	double v = 0.0;
	double base = 1.0;

	#define	_GET_CHAR(idx)	((idx) >= len ? '\0' : str[(idx)])

	int idx = 0;
	
	while ((_GET_CHAR(idx) >= '0' && _GET_CHAR(idx) <= '9') || _GET_CHAR(idx) == '_') {
		if (_GET_CHAR(idx) == '_') {
			idx++;
			continue;
		}

		v *= 10.0;
		v += _GET_CHAR(idx) - '0';
		idx++;
	}

	if (_GET_CHAR(idx) == '.') {
		idx++;
	}

	base = 0.1;

	while ((_GET_CHAR(idx) >= '0' && _GET_CHAR(idx) <= '9') || _GET_CHAR(idx) == '_') {
		if (_GET_CHAR(idx) == '_') {
			idx++;
			continue;
		}

		v += (_GET_CHAR(idx) - '0') * base;
		base *= 0.1;
		idx++;
	}

	return v;

	#undef	_GET_CHAR
}

float util_get_float_val(const char *str, uint32_t len) {
	return (float) util_get_double_val(str, len);
}




void util_unescape_string(
	ResizableString *target,
	const char *source,
	size_t source_len
) {
	for (size_t i = 0; i < source_len; i++) {
		char chr = source[i];
		if (chr != '\\') {
			rstr_append_with_char(target, chr);
		} else {
			if (i + 1 < source_len) {
				char next_char = source[++i];
				char unescaped_char = util_unescape(next_char);
				assert(unescaped_char != -1);
				rstr_append_with_char(target, unescaped_char);
			} else {
				assert(0);
			}
		}
	}
}
