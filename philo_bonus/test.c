/*
memset
printf
malloc
free
write
fork
kill
exit
pthread_create
pthread_detach
pthread_join
usleep
gettimeofday
waitpid
sem_open
sem_close
sem_post
sem_wait
sem_unlink
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <limits.h>

int main()
{
	sem_open("test", _POSIX_NAME_MAX);
}