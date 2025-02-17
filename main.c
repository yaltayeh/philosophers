#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_data
{
	unsigned long	nb_philo;
	unsigned long	t2die;
	unsigned long	t2eat;
	unsigned long	t2sleep;
	unsigned long	nb_meals;

	int				id;
	int				*forks;
	int				nb_forks;
	pthread_mutex_t	fork_lock;
	pthread_mutex_t	counter_lock;
	pthread_mutex_t	print_lock;
	int				end_simulation;
	unsigned long	start_time;


}	t_data;

void	print_msg(t_data *data, int philo_id, const char *msg)
{
	struct timeval	tv;
	long			ts;
	
	gettimeofday(&tv, NULL);
	ts = (tv.tv_sec * 1000000 + tv.tv_usec - data->start_time) / 1000;
	pthread_mutex_lock(&data->print_lock);
	printf("%lu %d %s\n", ts, philo_id, msg);
	if (data->end_simulation == 0)
		pthread_mutex_unlock(&data->print_lock);
}

unsigned long	get_time_now()
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int	take_fork(t_data *data, int fork_id, unsigned long last_eat)
{
	volatile int	loop;
	unsigned long	now;

	loop = 1;
	while (loop)
	{
		// check is died
		now = get_time_now();
		if (now - last_eat >= data->t2die)
		{
			data->end_simulation = 1;
			return (-1);
		}
		if (data->forks[fork_id] == 0)
		{
			pthread_mutex_lock(&data->fork_lock);
			if (data->forks[fork_id] == 0)
			{
				data->forks[fork_id] = 1;
				loop = 0;
			}
			pthread_mutex_unlock(&data->fork_lock);
		}
		if (loop)
			usleep(1000);
	}
	return (0);
}

void	*create_thread(void *arg)
{
	t_data			*data;
	int				id;
	unsigned long	last_eat;
	int				forks_id[2];

	data = (t_data *)arg;
	pthread_mutex_lock(&data->counter_lock);
	id = data->id++;
	pthread_mutex_unlock(&data->counter_lock);

	if (id & 1)
	{
		forks_id[0] = id;
		forks_id[1] = (id + 1) % data->nb_forks;
	}
	else
	{
		forks_id[0] = (id + 1) % data->nb_forks;
		forks_id[1] = id;
	}
	
	last_eat = get_time_now();
	while (1)
	{
		print_msg(data, id, "is thinking");
		
		if (take_fork(data, forks_id[0], last_eat) != 0)
			break ;
		print_msg(data, id, "has taken a fork");
		if (take_fork(data, forks_id[1], last_eat) != 0)
			break ;
		print_msg(data, id, "has taken a fork");

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
	if (data->end_simulation)
		print_msg(data, id, "died");
	exit(0);
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

int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	memset(data, 0, sizeof(*data));
	if (argc != 5 && argc != 6)
		return (-1);
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (-1);
		i++;
	}
	data->nb_philo = ft_atolu(argv[1]);
	data->nb_forks = data->nb_philo;
	data->t2die = ft_atolu(argv[2]);
	data->t2eat = ft_atolu(argv[3]);
	data->t2sleep = ft_atolu(argv[4]);
	if (argc == 6)
		data->nb_meals = ft_atolu(argv[5]);
	else
		data->nb_meals = UINT64_MAX;
	data->forks = malloc(data->nb_forks * sizeof(int));
	if (!data->forks)
		return (-1);
	memset(data->forks, 0, data->nb_forks * sizeof(int));
	return (0);
}

int main(int argc, char **argv)
{
	t_data			data;
	unsigned long	i;
	pthread_t		tid;
	struct timeval	tv;
	
	if (init_data(&data, argc, argv) != 0)
		return (-1);

	if (pthread_mutex_init(&data.fork_lock, NULL) != 0)
	{
		printf("can't create fork lock\n");
		exit(1);
	}
	if (pthread_mutex_init(&data.counter_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data.fork_lock);
		printf("can't create counter lock\n");
		exit(1);
	}
	if (pthread_mutex_init(&data.print_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data.fork_lock);
		pthread_mutex_destroy(&data.counter_lock);
		printf("can't create print lock\n");
		exit(1);
	}

	i = 0;
	gettimeofday(&tv, NULL);
	data.start_time = tv.tv_sec * 1000000 + tv.tv_usec;
	while (i < data.nb_philo)
	{
		if (pthread_create(&tid, NULL, create_thread, &data) != 0)
		{
			free(data.forks);
			pthread_mutex_destroy(&data.fork_lock);
			pthread_mutex_destroy(&data.counter_lock);
			pthread_mutex_destroy(&data.print_lock);
			printf("can't create thread\n");
			exit(1);
		}
		i++;
	}
	while (1)
		usleep(1000);
	return (0);
}