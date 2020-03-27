#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "../../string.h"
#include "../../array.h"
#include "../../list.h"
#include "../../map.h"

#include "../../util.h"

#include "version.h"
#include "lexer.h"
#include "parser.h"
#include "semantics.h"

#include "../preprocessor/preprocessor.h"

#define	_OPT_INCPATH				"-incpath"

#define	_OPT_ACTION					"-action"
#define	_OPT_ACTION_COMPILE			"compile"
#define	_OPT_ACTION_PRINTAST		"printast"
#define	_OPT_ACTION_PREPROCESS		"preprocess"
#define	_OPT_ACTION_PRINTTOKENS		"printtokens"

#define	_OPT_TARGET					"-target"
#define	_OPT_TARGET_FLAT			"flat"

#define	_OPT_DEF					"-def"

#define	_OPT_OUTPUT					"-output"

#define	_OPT_HELP					"-help"

static void _error(
	const char *fmt,
	...
) {
	va_list argptr;
    va_start(argptr, fmt);

	fputs("ERROR: ", stderr);
	vfprintf(stderr, fmt, argptr);
	fputc('\n', stderr);

	va_end(argptr);

	exit(1);
}

static void _test(void) {
	rstr_test();
	rarr_test();
	llst_test();
	map_test();
}

static void _help(void) {
	char * info =
	"ilang compiler assembler version " ASM_VERSION_MAJOR "." ASM_VERSION_MINOR "\n";
	
	fputs(info, stdout);
}

#include "ins.h"
#include "lexer.h"

int main(int argc, char *argv[]) {
	ParserContext *ctx = NULL;

	char *opt_file = NULL;
	char *opt_incpath = NULL;
	char *opt_action = NULL;
	char *opt_target = NULL;
	char *opt_output = NULL;

	// 跑测试。
	_test();

	// 没有任何参数时直接打印帮助信息。
	if (argc == 1) {
		_help();
		exit(0);
	}

	Map defs;
	map_init(
		&defs,
		map_rstr_comparer, map_rstr_comparer,
		(MapReleaser) rstr_delete, (MapReleaser) rstr_delete
	);

	for (int i = 1; i < argc; i++) {
		#define	_NEXT_ARG(opt_name) { if (i + 1 < argc) { opt = argv[++i]; } else { _error("the option '%s' expects a parameter.", opt_name); } }

		char *opt = argv[i];

		if (*opt == '-') {
			if (strcmp(opt, _OPT_INCPATH) == 0) {
				if (opt_incpath != NULL) {
					_error("the include path option '-incpath' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_INCPATH)
				opt_incpath = opt;
			} else if (strcmp(opt, _OPT_ACTION) == 0) {
				if (opt_action != NULL) {
					_error("the action option '-action' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_ACTION)
				opt_action = opt;
				if (strcmp(opt_action, _OPT_ACTION_COMPILE) != 0
						&& strcmp(opt_action, _OPT_ACTION_PRINTAST) != 0
						&& strcmp(opt_action, _OPT_ACTION_PREPROCESS) != 0
						&& strcmp(opt_action, _OPT_ACTION_PRINTTOKENS) != 0) {
					_error(
						"the option '-action' is specified a invalid value '%s', maybe specify a 'compile' or 'printast' or 'preprocess' or 'printtokens'.",
						opt_action
					);
				}
			} else if (strcmp(opt, _OPT_TARGET) == 0) {
				if (opt_target != NULL) {
					_error("the target option '-target' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_TARGET)
				opt_target = opt;
				if (strcmp(opt_target, _OPT_TARGET_FLAT) != 0) {
					_error(
						"the option '-target' is specified a invalid value '%s', maybe specify a 'flat'.",
						opt_target
					);
				}
			} else if (strcmp(opt, _OPT_DEF) == 0) {
				_NEXT_ARG(_OPT_DEF)
				const char *name = opt;
				_NEXT_ARG(_OPT_DEF)
				const char *value = opt;

				ResizableString *rstr_name = rstr_new_with_cstr(name);
				ResizableString *rstr_value = rstr_new_with_cstr(value);

				map_remove(&defs, rstr_name, true, true);
				map_set(&defs, rstr_name, rstr_value, true);
			} else if (strcmp(opt, _OPT_OUTPUT) == 0) {
				if (opt_output != NULL) {
					_error("the output option '-output' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_OUTPUT)
				opt_output = opt;
			} else if (strcmp(opt, _OPT_HELP) == 0) {
				_help();
				exit(0);
			} else {
				_error(
					"the option '%s' is invalid.",
					opt
				);
			}
		} else {
			if (opt_file != NULL) {
				_error(
					"the source file cannot be specified multi-times, '%s' and '%s'.",
					opt_file,
					opt
				);
			}
			opt_file = opt;
		}

		#undef	_NEXT_ARG
	}

	if (opt_file == NULL) {
		_error("the source file not be specified.");
	}
	if (opt_incpath == NULL) {
		opt_incpath = "";
	}
	if (opt_action == NULL) {
		opt_action = _OPT_ACTION_COMPILE;
	}
	if (opt_target == NULL) {
		opt_target = _OPT_TARGET_FLAT;
	}

	// 读取源文件。
	const char *source = NULL;
	int len = 0;
	source = (const char *)util_get_file_content(opt_file, &len);
	if (source == NULL) {
		_error("cannot open source file.");
	}

	// 打开输出文件。
	FILE *output = stdout;
	if (opt_output != NULL) {
		output = fopen(opt_output, "w");
		if (output == NULL) {
			_error("cannot write target code to target file '%s'.", opt_output);
		}
	}

	// 预处理。
	ResizableString preprocessed_source;
	rstr_init(&preprocessed_source);
	pp_process(
		opt_file, opt_incpath, source, len, &defs, NULL, &preprocessed_source
	);
	free((void *)source);

	map_free(&defs, true, true);

	if (strcmp(opt_action, _OPT_ACTION_PREPROCESS) == 0) {
		fputs(RSTR_CSTR(&preprocessed_source), output);
		goto _end;
	}

	if (strcmp(opt_action, _OPT_ACTION_PRINTTOKENS) == 0) {
		// 词法分析。
		LexerContext *lexctx = asm_lexer_new_context(
			opt_file, RSTR_CSTR(&preprocessed_source), RSTR_LEN(&preprocessed_source)
		);
		lexer_parse(lexctx);
		lexer_print_tokens(lexctx, output);
		lexer_free_context(lexctx);
	} else {
		// 语法解析。
		ctx = asm_parser_new_context(
			opt_file, RSTR_CSTR(&preprocessed_source), RSTR_LEN(&preprocessed_source)
		);
		asm_parser_parse(ctx);

		// 语义解析。
		asm_sem_process(ctx);

		if (strcmp(opt_action, _OPT_ACTION_COMPILE) == 0) {
			assert(0);
		} else if (strcmp(opt_action, _OPT_ACTION_PRINTAST) == 0) {
			assert(0);
		} else {
			assert(0);
		}
	}

// 释放资源。
_end:
	rstr_free(&preprocessed_source);
	if (opt_output != NULL) {
		fclose(output);
	}
	if (ctx != NULL) {
		asm_parser_free_context(ctx);
	}

	return 0;
}
