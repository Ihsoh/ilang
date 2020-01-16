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





#include "../preprocessor/preprocessor.h"

#define	_OPT_INCPATH			"-incpath"

#define	_OPT_ACTION				"-action"
#define	_OPT_ACTION_COMPILE		"compile"
#define	_OPT_ACTION_PRINTAST	"printast"
#define	_OPT_ACTION_PREPROCESS	"preprocess"

#define	_OPT_TARGET				"-target"
#define	_OPT_TARGET_GAS			"flat"

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
	"ilang compiler assembler version " ASM_VERSION_MAJOR "." ASM_VERSION_MINOR "\n";
	
	fputs(info, stdout);
}

int main(int argc, char *argv[]) {
	

	char *opt_incpath = NULL;
	char *opt_action = NULL;
	char *opt_target = NULL;
	char *opt_output = NULL;

	return 0;
}
