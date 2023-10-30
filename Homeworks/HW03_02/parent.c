#include "parent.h"

void parental_control(int pipefd[2], pid_t gen_pid, pid_t nsd_pid)
{
    // close unused pipe ends
    checked_close(pipefd[0]);
    checked_close(pipefd[1]);

    // *code*
    sleep(SLEEP_TIME);

    if (kill(gen_pid, SIGTERM) == FAIL) exit_err(SYS_CALL_FAIL);

    int gen_status, nsd_status;
    if (waitpid(gen_pid, &gen_status, 0) == FAIL) exit_err(FAILED_CHILD);
    if (waitpid(nsd_pid, &nsd_status, 0) == FAIL) exit_err(FAILED_CHILD);

    exit_err(CHILD_SUCCESS);
}