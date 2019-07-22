#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * __tst_fdopen(int fd, char *mode) {
	return fdopen(fd, mode);
}

void * __tst_malloc(size_t sz) {
	return malloc(sz);
}

void __tst_free(void *ptr) {
	return free(ptr);
}

int __tst_vfprintf(FILE *stream, const char *format, va_list *arg) {
	return vfprintf(stream, format, *arg);
}

int __tst_strcmp(char *a, char *b) {
	return strcmp(a, b);
}

void __tst_exit(int status) {
	return exit(status);
}
