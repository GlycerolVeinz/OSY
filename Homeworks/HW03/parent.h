#ifndef PARENT_H
#define PARENT_H

#include "util.h"

/**
 * @brief Parent process function
 *
 * @param pipe_fd pipe file descriptors
 * @param gen_pid generator process id
 * @param nsd_pid number sequence detector process id
 */
void parental_control(int pipefd[2], pid_t gen_pid, pid_t nsd_pid);

#endif // PARENT_H