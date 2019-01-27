#include "myassert.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void myerror(const char *file, int line, const char *fmt, ...) {
	char buffer[4096];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	fprintf(stderr, "ASSERT FAILED(%s:%d): %s\n", file, line, buffer);

	exit(0);
}
