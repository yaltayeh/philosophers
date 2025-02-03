#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
	int	n;

	printf("%s%n", s, &n);
	printf("%*sstructure at 0x%lx\n", n, "", (unsigned long)fp);
	printf("%*sfoo.a = %d\n", n, "", fp->a);
	printf("%*sfoo.b = %d\n", n, "", fp->b);
	printf("%*sfoo.c = %d\n", n, "", fp->c);
	printf("%*sfoo.d = %d\n", n, "", fp->d);
}

void *thr_fn1(void *arg)
{
	struct foo  foo = {1, 2, 3, 4};
	printfoo("thread 1:\n", &foo);
	pthread_exit((void *)&foo);
}

void *thr_fn2(void *arg)
{
	struct foo  foo;
	printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
	printf("thread 2: %p\n", &foo);
	pthread_exit((void *)0);
}

int main(void)
{
	int         err;
	pthread_t   tid1, tid2;
	struct foo  *fp;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
		err_exit(err, "can’t create thread 1");
	err = pthread_join(tid1, (void *)&fp);
	if (err != 0)
		err_exit(err, "can’t join with thread 1");
	sleep(1);
	printf("parent starting second thread\n");
		err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
		err_exit(err, "can’t create thread 2");
	sleep(1);
	printfoo("parent:\n", fp);
	exit(0);
}