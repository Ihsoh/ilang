#ifndef	__PREPROCESSOR_COMMENT_H__
#define	__PREPROCESSOR_COMMENT_H__

#include "../../string.h"

extern void pp_comment_process(
	ResizableString *dst,
	const char *file,
	const char *src,
	int len
);

#endif
