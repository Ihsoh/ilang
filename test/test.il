#include <test.ilh>

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
