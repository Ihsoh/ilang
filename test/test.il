#include <test.ilh>

function fdopen(fd:int32, mode:*char):*struct FILE dummy

function malloc(sz:uint64):* dummy
function free(ptr:*) dummy
function __tst_vfprintf(file:*struct FILE, fmt:*char, arg:*__va_list):int dummy

function strcmp(a:*char, b:*char):int32 dummy

function exit(status:int) dummy




var _stdin:*struct FILE = NULL;
var _stdout:*struct FILE = NULL;
var _stderr:*struct FILE = NULL;

function _get_stdin():*struct FILE {
	if (_stdin == NULL) {
		return fdopen(0, "r");
	}
	return _stdin;
}

function _get_stdout():*struct FILE {
	if (_stdout == NULL) {
		return fdopen(1, "w");
	}
	return _stdout;
}

function _get_stderr():*struct FILE {
	if (_stderr == NULL) {
		return fdopen(2, "w");
	}
	return _stderr;
}




function tst_fdopen(fd:int32, mode:*char):*struct FILE {
	return fdopen(fd, mode);
}

function tst_malloc(sz:uint64):* {
	return malloc(sz);
}

function tst_free(ptr:*) {
	free(ptr);
}

function tst_printf(fmt:*char, ...) {
	var arg:__va_list;
	__va_start(arg);
	__tst_vfprintf(_stdout, fmt, &arg);
	__va_end(arg);
}

function tst_fprintf(file:*struct FILE, fmt:*char, ...) {
	var arg:__va_list;
	__va_start(arg);
	__tst_vfprintf(file, fmt, &arg);
	__va_end(arg);
}

function tst_strcmp(a:*char, b:*char):int32 {
	return strcmp(a, b);
}

function tst_exit(status:int) {
	exit(status);
}
