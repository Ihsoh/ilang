#ifndef	__COMPILER_PREPROCESSOR_PREPROCESSOR_H__
#define	__COMPILER_PREPROCESSOR_PREPROCESSOR_H__

#include <stdio.h>

#include "../../string.h"
#include "../../map.h"

#define	PP_DIRECTIVE_ERROR_EXIT_CODE		234

extern void pp_process(
	const char *file,
	const char *include_paths,
	const char *source,
	int len,
	Map *defs,
	FILE *out,
	ResizableString *result
);

#endif
