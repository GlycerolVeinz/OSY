#include "util.h"

void exit_err(int err_code)
{
	int ret;
	char *err_msg;

	switch (err_code)
	{
	case FAILED_PIPE:
		ret = SYS_CALL_FAIL;
		err_msg = "";
		break;

	case FAILED_FORK:
		ret = SYS_CALL_FAIL;
		err_msg = "";
		break;

	case FAILED_CHILD:
		ret = FORK_CHILD_FAIL;
		err_msg = ERROR_FORK;
		break;

    case CHILD_SUCCESS:
        ret = SUCCESS;
        err_msg = SUC_CHILD;
        break;

    case GEN_SIG_TERM:
        ret = CHILD_SUCCESS;
        err_msg = GEN_TERM;
        break;

	default:
		ret = FAIL;
		err_msg = ERR_ERR;
		break;
	}

	puts(err_msg);
	exit(ret);
}
