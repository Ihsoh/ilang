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

int __tst_strcmp(char *a, char *b) {
	return strcmp(a, b);
}

void __tst_exit(int status) {
	return exit(status);
}

int __tst_fputs(char *str, FILE *fptr) {
	return fputs(str, fptr);
}

int __tst_fputi(int n, FILE *fptr) {
	return fprintf(fptr, "%d", n);
}

void __tst_assert(char *file, int line) {
	fprintf(stderr, "ASSERT FAILED(%s:%d)!\n", file, line);
	exit(1);
}
