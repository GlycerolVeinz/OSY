#include "nsd_p.h"

void nsd_process(int pipefd[2])
{
    // close unused pipe ends
    checked_close(pipefd[PIPE_WRITE_FD]);

    // redirect stdout to pipe
    checked_close(STDIN_FD);
    checked_dup2(pipefd[PIPE_READ_FD], STDIN_FD);
    checked_close(pipefd[PIPE_READ_FD]);

    // *code*
    if (execl("./nsd", "nsd", NULL) == FAIL) exit_err(SYS_CALL_FAIL);
}