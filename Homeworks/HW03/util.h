
#ifndef UTIL_H
#define UTIL_H

// all project libraries
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// my error codes
#define FAILED_PIPE 100
#define FAILED_FORK 101
#define FAILED_CHILD 102
#define GEN_SIG_TERM 103

// my error messages
#define ERR_ERR "Error: unknown error"
#define ERROR_PIPE "Error: pipe() failed"

// error messages for the task
#define ERROR_FORK "ERROR"
#define SUC_CHILD "OK"
#define GEN_TERM "GEN TERMINATED"

// success return codes
#define SUCCESS 0
#define CHILD_SUCCESS 0

// failed return codes
#define FAIL -1
#define FORK_CHILD_FAIL 1
#define SYS_CALL_FAIL 2

// constants
// file descriptors
#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

// pipe file descriptors
#define PIPE_READ_FD 0
#define PIPE_WRITE_FD 1

// sleep times
#define SLEEP_TIME 5
#define GEN_SLEEP_TIME 1

/*
    * exit_err - prints error message and exits with error code

    * err_code: internal error code, that sets return code and error message

    * err_codes(ret) and messages(msg):
        * FAILED_PIPE: ret=2  msg=(empty string)
        * FAILED_FORK: ret=2  msg=(empty string)
        * FAILED_CHILD: ret=1 msg="ERROR"
        * CHILD_SUCCESS: ret=0 msg="OK"
        * GEN_SIG_TERM: ret=1 msg="GEN TERMINATED"
        * default: ret=1 msg="Error: unknown error"
*/
void exit_err(int err_code);

#endif // UTIL_H