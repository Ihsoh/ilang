#include <testlib.ilh>

function __tst_fdopen(fd:int, mode:*char):*struct FILE dummy

function __tst_malloc(sz:size_t):* dummy
function __tst_free(ptr:*) dummy

function __tst_strcmp(a:*char, b:*char):int dummy

function __tst_exit(status:int) dummy

function __tst_fputs(str:*char, fptr:*struct FILE):int dummy
function __tst_fputi(n:int, fptr:*struct FILE):int dummy

function __tst_assert(file:*char, line:int) dummy




var _stdin:*struct FILE = NULL;
var _stdout:*struct FILE = NULL;
var _stderr:*struct FILE = NULL;

function _get_stdin():*struct FILE {
	if (_stdin == NULL) {
		return __tst_fdopen(0, "r");
	}
	return _stdin;
}

function _get_stdout():*struct FILE {
	if (_stdout == NULL) {
		return __tst_fdopen(1, "w");
	}
	return _stdout;
}

function _get_stderr():*struct FILE {
	if (_stderr == NULL) {
		return __tst_fdopen(2, "w");
	}
	return _stderr;
}




function tst_fdopen(fd:int, mode:*char):*struct FILE {
	return __tst_fdopen(fd, mode);
}

function tst_malloc(sz:size_t):* {
	return __tst_malloc(sz);
}

function tst_free(ptr:*) {
	__tst_free(ptr);
}

function tst_strcmp(a:*char, b:*char):int {
	return __tst_strcmp(a, b);
}

function tst_exit(status:int) {
	__tst_exit(status);
}

function tst_fputs(str:*char, fptr:*struct FILE):int {
	return __tst_fputs(str, fptr);
}

function tst_fputi(n:int, fptr:*struct FILE):int {
	return __tst_fputi(n, fptr);
}

function tst_assert(file:*char, line:int) {
	__tst_assert(file, line);
}
