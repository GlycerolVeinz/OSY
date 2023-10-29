
#ifndef UTIL_H
#define UTIL_H

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

/*
    * exit_err - prints error message and exits with error code

    * err_code: internal error code, that sets return code and error message

    * err_codes(ret) and messages(msg):
        * FAILED_PIPE: ret=2  msg=(emprty string)
        * FAILED_FORK: ret=2  msg=(emprty string)
        * FAILED_CHILD: ret=1 msg="ERROR"
        * CHILD_SUCCESS: ret=0 msg="OK"
        * default: ret=1 msg="Error: unknown error"
*/
void exit_err(int err_code);

#endif // UTIL_H