#ifndef NSD_P_H
#define NSD_P_H

#include "util.h"

/**
 * @brief Launches NSD "app" (process)
 *
 * @param pipe_fd pipe file descriptors
 *
  */
void nsd_process(int pipefd[2]);

#endif // NSD_P_H