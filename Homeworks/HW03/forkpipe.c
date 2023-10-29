#include "forkpipe.h"

#define SLEEP_TIME 5

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

#define PIPE_READ_FD 0
#define PIPE_WRITE_FD 1

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
        // close unused pipe ends
        close(pipefd[PIPE_READ_FD]);

        // redirect stdout to pipe
        close(STDOUT_FD);
        dup2(pipefd[PIPE_WRITE_FD], STDOUT_FD);
        close(pipefd[PIPE_WRITE_FD]);

        // *code*
        printf("%d %d\n", (rand() % 4096), (rand() % 4096));
    }
    else
    {
        nsd_pid = fork();
        if (nsd_pid == FAILED_FORK)
            exit_err(FAILED_FORK);

        // NSD CHILD
        if (nsd_pid == CHILD_SUCCESS)
        {
            // close unused pipe ends
            close(pipefd[PIPE_WRITE_FD]);

            // redirect stdin to pipe
            close(STDIN_FD);
            dup2(pipefd[PIPE_READ_FD], STDIN_FD);
            close(pipefd[PIPE_READ_FD]);

            // *code*
        }
        // MAIN
        else
        {
            // close unused pipe ends
            close(pipefd[0]);
            close(pipefd[1]);

            // *code*
            sleep(SLEEP_TIME);
        }
    }
    exit(SUCCESS);
}
