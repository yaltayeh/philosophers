#include <stdio.h>
#include <stdlib.h>

void err_exit(int code, char *msg)
{
	perror(msg);
	exit(code);
}