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

#include "asmgen.h"
#include "asmgen_gas64.h"

#include "../preprocessor/preprocessor.h"

#define	_OPT_ARCH				"-arch"
#define	_OPT_ARCH_32			"32"
#define	_OPT_ARCH_64			"64"

#define	_OPT_PLATFORM			"-platform"
#define	_OPT_PLATFORM_WINDOWS	"windows"
#define	_OPT_PLATFORM_SYSTEM_V	"system-v"
#define	_OPT_PLATFORM_ISYSTEM	"isystem"

#define	_OPT_INCPATH			"-incpath"

#define	_OPT_ACTION				"-action"
#define	_OPT_ACTION_COMPILE		"compile"
#define	_OPT_ACTION_PRINTAST	"printast"
#define	_OPT_ACTION_PREPROCESS	"preprocess"

#define	_OPT_TARGET				"-target"
#define	_OPT_TARGET_GAS			"gas"
#define	_OPT_TARGET_IASM		"iasm"

#define	_OPT_DEF				"-def"

#define	_OPT_OUTPUT				"-output"

#define	_OPT_HELP				"-help"

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
	"ilang compiler backend version " BE_VERSION_MAJOR "." BE_VERSION_MINOR "\n"
	"\n"
	"The compiler backend application used to compile ilang ir source code, and output gas or iasm code.\n"
	"The target code is output to standard output.\n"
	"\n"
	"usage: ilcbe <source file> [-arch (32|64)] [-incpath <include path>] [-action (compile|printast|preprocess)]\n"
	"                           [-target (gas|iasm)] {-def <name> <value>} [-output <target file>]\n"
	"                           [-help]\n"
	"\n"
	"example: ilcbe source.ir -arch 64 -incpath \"DIR_PATH1;DIR_PATH2;...\" -action compile -target gas -output target.s\n"
	"\n"
	"options:\n"
	"  -arch (32|64)                            Specify size of machine word.\n"
	"  -incpath <include path>                  Specify include path, separated by ';'.\n"
	"  -action (compile|printast|preprocess)    Output target code or abstract syntax tree that represented by text.\n"
	"  -target (gas|iasm)                 		Output gas or iasm code when option '-action' is specified to 'compile'.\n"
	"  -def <name> <value>                      Define a object-like macro.\n"
	"  -output <target file>                    Specify a target file. write target code to standard output if target file not be specified.\n"
	"  -help                                    Display available options.\n";
	
	fputs(info, stdout);
}

int main(int argc, char *argv[]) {
	ParserContext *ctx = NULL;

	char *opt_file = NULL;
	char *opt_arch = NULL;
	char *opt_platform = NULL;
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
			if (strcmp(opt, _OPT_ARCH) == 0) {
				if (opt_arch != NULL) {
					_error("the architecture option '-arch' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_ARCH)
				opt_arch = opt;
				if (strcmp(opt_arch, _OPT_ARCH_32) != 0
						&& strcmp(opt_arch, _OPT_ARCH_64) != 0) {
					_error(
						"the option '-arch' is specified a invalid value '%s', maybe specify a '32' or '64'.",
						opt_arch
					);
				}
			} else if (strcmp(opt, _OPT_PLATFORM) == 0) {
				if (opt_platform != NULL) {
					_error("the platform option '-platform' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_ARCH)
				opt_platform = opt;
				if (strcmp(opt_platform, _OPT_PLATFORM_WINDOWS) != 0
						&& strcmp(opt_platform, _OPT_PLATFORM_SYSTEM_V) != 0
						&& strcmp(opt_platform, _OPT_PLATFORM_ISYSTEM) != 0) {
					_error(
						"the option '-platform' is specified a invalid value '%s', "
							"maybe specify a 'windows' or 'system-v' or 'isystem'.",
						opt_platform
					);
				}
			} else if (strcmp(opt, _OPT_INCPATH) == 0) {
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
						&& strcmp(opt_action, _OPT_ACTION_PREPROCESS) != 0) {
					_error(
						"the option '-action' is specified a invalid value '%s', maybe specify a 'compile' or 'printast' or 'preprocess'.",
						opt_action
					);
				}
			} else if (strcmp(opt, _OPT_TARGET) == 0) {
				if (opt_target != NULL) {
					_error("the target option '-target' cannot be specified multi-times.");
				}
				_NEXT_ARG(_OPT_TARGET)
				opt_target = opt;
				if (strcmp(opt_target, _OPT_TARGET_GAS) != 0
						&& strcmp(opt_target, _OPT_TARGET_IASM) != 0) {
					_error(
						"the option '-target' is specified a invalid value '%s', maybe specify a 'gas' or 'iasm'.",
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
	if (opt_arch == NULL) {
		opt_arch = _OPT_ARCH_32;
	}
	if (opt_incpath == NULL) {
		opt_incpath = "";
	}
	if (opt_action == NULL) {
		opt_action = _OPT_ACTION_COMPILE;
	}
	if (opt_target == NULL) {
		opt_target = _OPT_TARGET_GAS;
	}

	ResizableString *rstr_arch_name = rstr_new_with_cstr("__ARCH__");
	ResizableString *rstr_arch = rstr_new();
	rstr_appendf(rstr_arch, "\"%s\"", opt_arch);
	map_remove(&defs, rstr_arch_name, true, true);
	map_set(
		&defs,
		rstr_arch_name,
		rstr_arch,
		true
	);

	ResizableString *rstr_target_name = rstr_new_with_cstr("__TARGET__");
	ResizableString *rstr_target = rstr_new();
	rstr_appendf(rstr_target, "\"%s\"", opt_target);
	map_remove(&defs, rstr_target_name, true, true);
	map_set(
		&defs,
		rstr_target_name,
		rstr_target,
		true
	);

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

	// 语法解析。
	int arch = BE_ARCH_32;
	if (strcmp(opt_arch, _OPT_ARCH_32) == 0) {
		arch = BE_ARCH_32;
	} else if (strcmp(opt_arch, _OPT_ARCH_64) == 0) {
		arch = BE_ARCH_64;
	} else {
		assert(0);
	}
	ctx = be_parser_new_context(
		opt_file, RSTR_CSTR(&preprocessed_source), RSTR_LEN(&preprocessed_source), arch
	);
	be_parser_parse(ctx);

	// 语义解析。
	be_sem_process(ctx);

	int platform = BE_ASMGEN_PLATFORM_DEFAULT;
	if (opt_platform != NULL) {
		if (strcmp(opt_platform, _OPT_PLATFORM_WINDOWS) == 0) {
			platform = BE_ASMGEN_PLATFORM_WINDOWS;
		} else if (strcmp(opt_platform, _OPT_PLATFORM_SYSTEM_V) == 0) {
			platform = BE_ASMGEN_PLATFORM_SYSTEM_V;
		} else if (strcmp(opt_platform, _OPT_PLATFORM_ISYSTEM) == 0) {
			platform = BE_ASMGEN_PLATFORM_ISYSTEM;
		} else {
			assert(0);
		}
	}

	// 执行操作（编译、打印AST）。
	if (strcmp(opt_action, _OPT_ACTION_COMPILE) == 0) {
		if (strcmp(opt_target, _OPT_TARGET_GAS) == 0) {
			if (arch == BE_ARCH_32) {
				_error("target 'gas(32)' is not supported currently.");
			} else if (arch == BE_ARCH_64) {
				ASMGeneratorGas64Context *asmgen_ctx = be_asmgen_gas64_new_context(ctx, output, platform);
				be_asmgen_gas64_generate(asmgen_ctx);
				be_asmgen_gas64_free_context(asmgen_ctx);
			} else {
				assert(0);
			}
		} else if (strcmp(opt_target, _OPT_TARGET_IASM) == 0) {
			_error("target 'iasm' is not supported currently.");
		} else {
			assert(0);
		}
	} else if (strcmp(opt_action, _OPT_ACTION_PRINTAST) == 0) {
		// 打印语法树。
		be_parser_print_ast(ctx, output);
	} else {
		assert(0);
	}

	// 释放资源。
_end:
	rstr_free(&preprocessed_source);
	if (opt_output != NULL) {
		fclose(output);
	}
	if (ctx != NULL) {
		be_parser_free_context(ctx);
	}

	return 0;
}
