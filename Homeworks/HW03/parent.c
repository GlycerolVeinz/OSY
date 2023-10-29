#include "parent.h"

void parental_control(int pipefd[2], pid_t gen_pid, pid_t nsd_pid)
{
    // close unused pipe ends
    close(pipefd[0]);
    close(pipefd[1]);

    // *code*
    sleep(SLEEP_TIME);

    if (kill(gen_pid, SIGTERM) == FAIL) exit_err(SYS_CALL_FAIL);

 }