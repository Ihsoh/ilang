#include <stdio.h>

int __tst_vfprintf(FILE *stream, const char *format, va_list *arg) {
	return vfprintf(stream, format, *arg);
}
