#include "semantics.h"

#include "../../parser.h"
#include "../../util.h"
#include "../../common.h"

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>






void asm_sem_process(
	ParserContext *ctx
) {
	assert(ctx);
	assert(ctx->ast);

	// TODO: 对抽象语法树进行语义检查。
}
