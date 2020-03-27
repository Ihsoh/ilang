#include "parser.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "lexer.h"

#include "../../lexer.h"
#include "../../parser.h"

ParserContext * asm_parser_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	LexerContext *lexctx = asm_lexer_new_context(file, source, len);
	lexer_parse(lexctx);

	AsmParserContextData data;
	data.dummy = 0;

	ParserContext *ctx = parser_new_context_with_data(
		lexctx, sizeof(data), &data
	);

	return ctx;
}

void asm_parser_free_context(
	ParserContext * ctx
) {
	assert(ctx);

	asm_lexer_free_context(ctx->lexctx);
	parser_free_context(ctx);
}

void asm_parser_parse(
	ParserContext * ctx
) {
	assert(ctx);
	assert(ctx->ast == NULL);

	// TODO: 解析符号流，生成抽象语法树。
}
