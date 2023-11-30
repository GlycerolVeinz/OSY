#include "gen.h"

volatile sig_atomic_t interupt = 0;

void sigterm_handler(int signum)
{
    interupt = 1;
    if (signum == SIGTERM)
    {
        exit_err(GEN_SIG_TERM);
    }
    else if (signum == -1)
    {
        exit_err(FAILED_CHILD);
    }
}

void generator(int pipefd[2])
{
    // close unused pipe ends
    checked_close(pipefd[PIPE_READ_FD]);

    // redirect stdout to pipe
    checked_close(STDOUT_FD);
    checked_dup2(pipefd[PIPE_WRITE_FD], STDOUT_FD);
    checked_close(pipefd[PIPE_WRITE_FD]);

    if (signal(SIGTERM, sigterm_handler) == SIG_ERR)
        exit_err(FAILED_CHILD);

    // *code*
    while (!interupt)
    {
        printf("%d %d\n", (rand() % 4096), (rand() % 4096));
        fflush(stdout);

        sleep(GEN_SLEEP_TIME);
    }
}