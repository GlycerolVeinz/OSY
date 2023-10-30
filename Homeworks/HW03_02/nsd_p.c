#include "nsd_p.h"

void nsd_process(int pipefd[2])
{
    // close unused pipe ends
    checked_close(pipefd[PIPE_READ_FD]);

    // redirect stdout to pipe
    checked_close(STDOUT_FD);
    checked_dup2(pipefd[PIPE_WRITE_FD], STDOUT_FD);
    checked_close(pipefd[PIPE_WRITE_FD]);

    // *code*
    execl("./nsd.exe", "nsd.exe", NULL);
}