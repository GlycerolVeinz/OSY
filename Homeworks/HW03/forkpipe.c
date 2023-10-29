//
// Created by glycerolveinz on 25.10.23.
//

#include "forkpipe.h"
#include <sys/types.h>

#define FAILED_PIPE 100
#define FAILED_FORK 101
#define FAILED_CHILD 102

#define ERR_ERR "Error: unknown error"
#define ERROR_PIPE "Error: pipe() failed"
#define ERROR_FORK "ERROR"

#define SUC_CHILD "OK"

#define SUCCESS 0
#define CHILD_SUCCESS 0

#define FAIL -1
#define FORK_CHILD_FAIL 1
#define SYS_CALL_FAIL 2

void exit_err(int err_code)
{
	int ret;
	char *err_msg;

	switch (err_code)
	{
	case FAILED_PIPE:
		ret = SYS_CALL_FAIL;
		err_msg = ERROR_PIPE;
		break;

	case FAILED_FORK:
		ret = FAIL;
		err_msg = ERROR_FORK;
		break;

	case FAILED_CHILD:
		ret = FORK_CHILD_FAIL;
		err_msg = ERROR_FORK;
		break;

	default:
		ret = FAIL;
		err_msg = ERR_ERR;
		break;
	}

	perror(err_msg);
	exit(ret);
}

int main(void)
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror(ERROR_PIPE);
		exit(FAILED_PIPE);
	}

	pid_t gen_pid, nsd_pid;
	gen_pid = fork();
	if (gen_pid == FAILED_FORK)
	{
		perror(ERROR_FORK);
		exit(FAILED_FORK);
	}

	if (gen_pid == CHILD_SUCCESS)
	{
		// GEN CHILD
	}
	else
	{
		// MAIN
		
		// NSD
	}
	exit(SUCCESS);
}
