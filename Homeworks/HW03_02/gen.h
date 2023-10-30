#ifndef GEN_H
#define GEN_H

#include "util.h"

/**
 * @brief sigterm_handler - handles SIGTERM signal, writes "GEN TERMINATED" to stdout and exits with code 0
 * @param signum: sigterm int value
*/
void sigterm_handler(int signum);

/**
 * @brief generator - generates random numbers and writes them to stdout within 1sec intervals
 * @property needs to be terminated externally by SIGTERM signal
*/
void generator(int pipefd[2]);

#endif // GEN_H