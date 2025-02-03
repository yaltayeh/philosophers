#ifndef APUE_H
# define APUE_H

#include <pthread.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void err_exit(int code, char *msg);

#endif