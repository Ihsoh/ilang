#ifndef	__PREPROCESSOR_LINE_H__
#define	__PREPROCESSOR_LINE_H__

#include "../../string.h"

#include "parser.h"
#include "gen.h"

extern void expand_macro_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *key,
	int len
);

extern void pp_line_process(
	PPGeneratorContext *genctx,
	ParserASTNode *node
);

#endif
