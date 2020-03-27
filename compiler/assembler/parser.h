#ifndef __COMPILER_ASSEMBLER_PARSER_H__
#define __COMPILER_ASSEMBLER_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

typedef struct {
	int dummy;
} AsmParserContextData;

extern ParserContext * asm_parser_new_context(
	const char *file,
	const char *source,
	int len
);
extern void asm_parser_free_context(
	ParserContext * ctx
);
extern void asm_parser_parse(
	ParserContext * ctx
);

#endif
