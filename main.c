#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

typedef struct s_table
{
	int				nb_philo;
	unsigned long	t2die;
	unsigned long	t2eat;
	unsigned long	t2sleep;
	unsigned long	nb_meals;

	int				id;
	int				*checkforks;
	pthread_mutex_t	*forks;
	int				nb_forks;
	pthread_mutex_t	counter_lock;
	pthread_mutex_t	print_lock;
	int				end_simulation;
	unsigned long	start_time;


}	t_table;

typedef struct s_philo
{
	t_table			*table;
	int				id;
	unsigned long	last_eat;
	int				*check_forks[2];
	pthread_mutex_t	*forks[2];
}	t_philo;


unsigned long	get_time_now()
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}


void	print_msg(t_philo *philo, const char *msg)
{
	struct timeval	tv;
	long			ts;
	
	gettimeofday(&tv, NULL);
	ts = get_time_now() - philo->table->start_time;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d %s\n", ts, philo->id + 1, msg);
	// if (table->end_simulation == 0)
		pthread_mutex_unlock(&philo->table->print_lock);
}

int	take_fork(t_philo *philo, int *checkfork, pthread_mutex_t *fork)
{
	volatile int	loop;
	unsigned long	now;

	while (1)
	{
		// check is died
		now = get_time_now();
		if (now - philo->last_eat >= philo->table->t2die)
		{
			philo->table->end_simulation = 1;
			return (1);
		}
		if (philo->table->end_simulation)
			return (1);
		if (*checkfork == 0)
		{
			pthread_mutex_lock(fork);
			if (*checkfork == 0)
			{
				*checkfork = 1;
				pthread_mutex_unlock(fork);
				return (0);
			}
			pthread_mutex_unlock(fork);
		}
	}
	return (0);
}

int	best_usleep(t_table *table, unsigned long usec)
{
	while (usec > 10000)
	{
		if (table->end_simulation)
			return (1);
		usleep(10000);
		usec -= 10000;
	}
	if (usec > 0)
		usleep(usec);
	return (0);
}

void	*create_thread(void *arg)
{
	t_philo	philo;

	philo.table = (t_table *)arg;
	pthread_mutex_lock(&philo.table->counter_lock);
	philo.id = philo.table->id++;
	pthread_mutex_unlock(&philo.table->counter_lock);

	if (philo.id & 1)
	{
		philo.check_forks[0] = &philo.table->checkforks[philo.id];
		philo.check_forks[1] = &philo.table->checkforks[(philo.id + 1) % philo.table->nb_forks];
		philo.forks[0] = &philo.table->forks[philo.id];
		philo.forks[1] = &philo.table->forks[(philo.id + 1) % philo.table->nb_forks];
		usleep(1000);
	}
	else
	{
		philo.check_forks[0] = &philo.table->checkforks[(philo.id + 1) % philo.table->nb_forks];
		philo.check_forks[1] = &philo.table->checkforks[philo.id];
		philo.forks[0] = &philo.table->forks[(philo.id + 1) % philo.table->nb_forks];
		philo.forks[1] = &philo.table->forks[philo.id];
	}
	
	last_eat = get_time_now();
	while (1)
	{
		print_msg(data, id, "is thinking");
		
		if (take_fork(data, forks_id[0], last_eat) != 0)
			break ;
		print_msg(&philo, "has taken a fork");
		if (take_fork(&philo, philo.check_forks[1], philo.forks[1]) != 0)
			break ;
		print_msg(&philo, "has taken a fork");

		print_msg(data, id, "is eating");
		last_eat = get_time_now();
		usleep(data->t2eat * 1000);
		pthread_mutex_lock(&data->fork_lock);
		data->forks[forks_id[0]] = 0;
		data->forks[forks_id[1]] = 0;
		pthread_mutex_unlock(&data->fork_lock);
		print_msg(data, id, "is sleeping");
		usleep(data->t2sleep * 1000);
	}
	if (philo.table->end_simulation)
		print_msg(&philo, "died");
	return (NULL);
}

unsigned long ft_atolu(const char *s)
{
	unsigned long	nbr;

	nbr = 0;
	while (*s && *s >= '0' && *s <= '9')
		nbr = nbr * 10 + *s++ - '0';
	return (nbr);
}

int	is_valid_number(const char *s)
{
	if (!*s)
		return (0);
	while (*s && *s >= '0' && *s <= '9')
		s++;
	if (*s)
		return (0);
	return (1);
}

int	init_data(t_table *table, int argc, char **argv)
{
	int	i;

	memset(table, 0, sizeof(*table));
	if (argc != 5 && argc != 6)
		return (-1);
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (-1);
		i++;
	}
	table->nb_philo = ft_atolu(argv[1]);
	table->nb_forks = table->nb_philo;
	table->t2die = ft_atolu(argv[2]);
	table->t2eat = ft_atolu(argv[3]);
	table->t2sleep = ft_atolu(argv[4]);
	if (argc == 6)
		table->nb_meals = ft_atolu(argv[5]);
	else
		table->nb_meals = UINT64_MAX;
	table->checkforks = malloc(table->nb_forks * sizeof(int));
	if (!table->checkforks)
		return (-1);
	memset(table->checkforks, 0, table->nb_forks * sizeof(int));
	table->forks = malloc(table->nb_forks * sizeof(pthread_mutex_t));
	if (!table->forks)
		return (-1);
	memset(data->forks, 0, data->nb_forks * sizeof(int));
	return (0);
}

int main(int argc, char **argv)
{
	t_table			table;
	int				i;
	pthread_t		*tid;
	
	if (init_data(&table, argc, argv) != 0)
		return (-1);
	tid = malloc(table.nb_philo * sizeof(pthread_t));
	if (!tid)
		return (-1);

	table.counter_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	table.print_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

	i = 0;
	table.start_time = get_time_now();
	while (i < table.nb_philo)
	{
		if (i == 1 || pthread_create(&tid[i], NULL, create_thread, &table) != 0)
		{
			free(table.checkforks);
			free(table.forks);
			table.end_simulation = 1;
			while (--i >= 0)
				pthread_join(tid[i], NULL);
			free(tid);
			printf("can't create thread\n");
			exit(1);
		}
		i++;
	}
	i = 0;
	while (i < table.nb_philo)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
	free(table.checkforks);
	free(table.forks);
	free(tid);
	return (0);
}