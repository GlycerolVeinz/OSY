#include "forkpipe.h"

int main(void)
{
    // prepare pipe
    int pipefd[2];
    if (pipe(pipefd) == -1)
        exit_err(FAILED_PIPE);

    pid_t gen_pid, nsd_pid;
    gen_pid = fork();
    if (gen_pid == FAILED_FORK)
        exit_err(FAILED_FORK);

    // GEN CHILD
    if (gen_pid == CHILD_SUCCESS)
    {
        generator(pipefd);
    }
    else
    {
        nsd_pid = fork();
        if (nsd_pid == FAILED_FORK)
            exit_err(FAILED_FORK);

        // NSD CHILD
        if (nsd_pid == CHILD_SUCCESS)
        {
            nsd_process(pipefd);
        }
        // MAIN
        else
        {
            parental_control(pipefd, gen_pid, nsd_pid)
        }
    }
}
