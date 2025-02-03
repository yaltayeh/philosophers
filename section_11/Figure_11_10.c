#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "apue.h"

struct foo
{
	int				f_count;
	pthread_mutex_t	f_lock;
	int				f_id;
	/* ... more stuff here ... */
};

struct foo *foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 0;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
		{
			free(fp);
            return(NULL);
        }
        /* ... continue initialization ... */
    }
    return(fp);
}

void	foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void	foo_rele(struct foo *fp) /* release a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0)  /* last reference */
	{
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
	else
	{
        pthread_mutex_unlock(&fp->f_lock);
	}
}

void	*thr1_fn(void *arg)
{
	struct foo *fp;

	fp = (struct foo *)arg;
	// usleep(5);
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	fp->f_count++;
	fp->f_count++;
	fp->f_count++;
	fp->f_count++;
	fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
	return ((void *)0);
}

void	*thr2_fn(void *arg)
{
	struct foo *fp;

	fp = (struct foo *)arg;
	pthread_mutex_lock(&fp->f_lock);
	printf("count: %d\n", --fp->f_count);
	printf("count: %d\n", --fp->f_count);
	printf("count: %d\n", --fp->f_count);
	printf("count: %d\n", --fp->f_count);
	printf("count: %d\n", --fp->f_count);
	printf("count: %d\n", --fp->f_count);
    pthread_mutex_unlock(&fp->f_lock);
	return ((void *)0);
}

int main()
{
	struct foo *fp;
	pthread_t	tid[2];
	int			err;


	fp = foo_alloc(10);
	err = pthread_create(&tid[0], NULL, thr1_fn, fp);
	if (err != 0)
	{
        fprintf(stderr, "can’t create thread");
		exit(err);
	}
	err = pthread_create(&tid[1], NULL, thr2_fn, fp);
	if (err != 0)
	{
        fprintf(stderr, "can’t create thread");
		exit(err);
	}
	usleep(1000);
	printf("Done (%d)\n", fp->f_count);
	return (0);
}