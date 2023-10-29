#ifndef GEN_H
#define GEN_H

#include "util.h"

/*
    * sigterm_handler - handles SIGTERM signal, writes "GEN TERMINATED" to stdout and exits with code 0

    * signum: sigterm int value
*/
void sigterm_handler(int signum);

/*
    * generator - generates random numbers and writes them to stdout within 1sec intervals
*/
void generator(int pipefd[2]);

#endif // GEN_H