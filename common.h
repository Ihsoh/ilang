#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>

#include "lexer.h"

extern void cmn_print_token(
	LexerToken *token,
	FILE *out
);

extern void cmn_print_code_by_token(
	FILE *out,
	LexerToken *token,
	int n_prev,
	int n_succ
);

#endif
