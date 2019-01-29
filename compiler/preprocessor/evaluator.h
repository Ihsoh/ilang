#ifndef	__COMPILER_PREPROCESSOR_EVALUATOR_H__
#define	__COMPILER_PREPROCESSOR_EVALUATOR_H__

#include "gen.h"

extern int pp_eval(
	PPGeneratorContext *genctx,
	const char *expr,
	int len
);

#endif
