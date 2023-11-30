#include "util.h"

void exit_err(int err_code)
{
    int ret;
    char *err_msg;
    FILE *stream;
    int sig_safe = 0;
    int len;
    int fd;

    switch (err_code)
    {
    case SYS_CALL_FAIL:
        ret = SYS_CALL_FAIL;
        err_msg = "";
        stream = stderr;
        break;

    case FAILED_PIPE:
        ret = SYS_CALL_FAIL;
        err_msg = "";
        stream = stderr;
        break;

    case FAILED_FORK:
        ret = SYS_CALL_FAIL;
        err_msg = "";
        stream = stderr;
        break;

    case FAILED_CHILD:
        sig_safe = 1;
        len = 6;
        ret = FORK_CHILD_FAIL;
        err_msg = ERROR_FORK;
        stream = stdout;
        break;

    case CHILD_SUCCESS:
        ret = SUCCESS;
        err_msg = SUC_CHILD;
        stream = stdout;
        break;

    case GEN_SIG_TERM:
        sig_safe = 1;
        ret = CHILD_SUCCESS;
        err_msg = GEN_TERM;
        stream = stderr;
        len = 16;
        break;

    default:
        ret = FAIL;
        err_msg = ERR_ERR;
        stream = stderr;
        break;
    }

    if (!sig_safe)
    {
        fputs(err_msg, stream);
        fflush(stream);
        exit(ret);
    }
    else
    {
        if (stream == stdout) fd = STDOUT_FD;
        else fd = STDERR_FD;
        write(fd, err_msg, sizeof(char) * len);
        fsync(STDERR_FD);
        _Exit(ret);
    }
}

void checked_close(int fd)
{
    if (close(fd) == FAIL)
        exit_err(SYS_CALL_FAIL);
}

void checked_dup2(int oldfd, int newfd)
{
    if (dup2(oldfd, newfd) == FAIL)
        exit_err(SYS_CALL_FAIL);
}
