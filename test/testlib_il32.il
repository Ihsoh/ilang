#include <testlib.ilh>




function __tst_fdopen(fd:int, mode:*char):*struct FILE {

}

function __tst_malloc(sz:size_t):* {

}

function __tst_free(ptr:*) {

}

function __tst_strcmp(a:*char, b:*char):int {

}

function __tst_exit(status:int) {

}

function __tst_fputs(str:*char, fptr:*struct FILE):int {

}

function __tst_fputi(n:int, fptr:*struct FILE):int {

}

function __tst_assert(file:*char, line:int) {

}
