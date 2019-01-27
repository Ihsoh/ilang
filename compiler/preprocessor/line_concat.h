#ifndef	__PREPROCESSOR_LINE_CONCAT_H__
#define	__PREPROCESSOR_LINE_CONCAT_H__

#include "../../string.h"

extern void pp_line_concat_process(
	ResizableString *dst,
	const char *file,
	const char *src,
	int len
);

#endif
