/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:25:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 22:07:00 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <semaphore.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>

# define DEFAULT_MUTEX PTHREAD_MUTEX_INITIALIZER

# define SEM_FORKS_NAME "/philo_forks"
# define SEM_D4E_NAME "/philo_dead_for_ever"
# define SEM_PRINT_NAME "/philo_print"
# define SEM_MEALS_NAME "/philo_meals"
# define SEM_DEAD_NAME "/philo_dead"

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	pthread_mutex_t	dead_lock;
	int				dead;
	int				meals_eaten;

	pthread_t		sister_tid;
	pthread_mutex_t	meal_lock;
	long			last_meal;

	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	int			nb_philo;
	long		t2die;
	long		t2eat;
	long		t2sleep;
	int			nb_meals;

	long		start_time;
	pthread_t	meals_counter;

	sem_t		*forks_sem;
	sem_t		*d4e_sem;
	sem_t		*print_sem;
	sem_t		*meals_sem;
	sem_t		*dead_sem;
}	t_table;

int		init_table(t_table *table, int argc, char **argv);
long	get_time_now(void);
int		best_usleep(t_philo *philo, long millisecond);
int		print(t_philo *philo, char *msg);
void	*philo_routine(void *arg);
int		check_args(int argc, char **argv);

/*  dead  */
int		check_dead(t_philo *philo);
void	dead_for_ever(t_table *table);

/*  simulation  */
int		start_simulation(t_table *table);
void	end_simulation(t_table *table);
void	remove_semaphores(t_table *table);
void	close_semaphores(t_table *table);
void	child_process(t_table *table, int id);
int		eating(t_philo *philo);

#endif