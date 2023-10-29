#include "gen.h"

sigterm_handler(int signum)
{
    if (signum == SIGTERM)
    {
        exit_err(GEN_SIG_TERM);
    }
}

void generator(int pipefd[2])
{
    // close unused pipe ends
    close(pipefd[PIPE_READ_FD]);

    // redirect stdout to pipe
    close(STDOUT_FD);
    dup2(pipefd[PIPE_WRITE_FD], STDOUT_FD);
    close(pipefd[PIPE_WRITE_FD]);

    // *code*
    while (1)
    {
        printf("%d %d\n", (rand() % 4096), (rand() % 4096));

        if(signal(SIGTERM, sigterm_handler) == SIG_ERR) exit_err(FAILED_CHILD);

        sleep(GEN_SLEEP_TIME);
    }
}