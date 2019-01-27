#include "line_concat.h"

#include <assert.h>

void _process(
	ResizableString *dst,
	const char *file,
	const char *src,
	int len
) {
	assert(dst);
	assert(file);
	assert(src);
	assert(len >= 0);

	for (int i = 0; i < len; i++) {
		char chr = src[i];
		if (chr == '\\') {
			if (i + 1 < len
					&& src[i + 1] == '\n') {
				i += 2;
				int n = 1;
				for (; i < len; i++) {
					chr = src[i];
					if (chr == '\\') {
						if (i + 1 < len
								&& src[i + 1] == '\n') {
							i++;
							n++;
						} else {
							rstr_append_with_char(dst, chr);
						}
					} else {
						rstr_append_with_char(dst, chr);
						if (chr == '\n') {
							break;
						}
					}
				}
				for (int j = 0; j < n; j++) {
					rstr_append_with_char(dst, '\n');
				}
			} else {
				rstr_append_with_char(dst, chr);
			}
		} else {
			rstr_append_with_char(dst, chr);
		}
	}
}

void pp_line_concat_process(
	ResizableString *dst,
	const char *file,
	const char *src,
	int len
) {
	assert(dst);
	assert(file);
	assert(src);

	_process(dst, file, src, len);
}
