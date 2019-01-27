#ifndef	__MYASSERT_H__
#define	__MYASSERT_H__

#include <assert.h>

#define MYASSERT(cond, fmt, args...)	{ if (!(cond)) { myerror(__FILE__, __LINE__, (fmt), ##args); } }

extern void myerror(const char *file, int line, const char *fmt, ...);

#endif
