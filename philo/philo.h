/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:25:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 14:52:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define DEFAULT_MUTEX PTHREAD_MUTEX_INITIALIZER

typedef struct s_sister
{
	long			last_meal;
	pthread_mutex_t	time_lock;
	long			t2die;

	int				meals_eaten;
	pthread_mutex_t	count_lock;
	int				nb_meals;

	struct s_philo	*victim;
}	t_sister;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	int				is_run;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	struct s_table	*table;
	t_sister		*sister;
}	t_philo;

typedef struct s_table
{
	int				nb_philo;
	t_philo			*philos;

	long			t2die;
	long			t2eat;
	long			t2sleep;
	int				nb_meals;
	long			start_time;

	pthread_mutex_t	*forks_lock;
	t_sister		*sisters;

	int				dead_for_ever;
	pthread_mutex_t	dead_lock;

	pthread_mutex_t	print_lock;
}	t_table;

int		init_table(t_table *table, int argc, char **argv);
long	get_time_now(void);
int		best_usleep(t_table *table, long millisecond);
void	print(t_philo *philo, char *msg);
void	*philo_routine(void *arg);

/*  dead  */
int		check_dead(t_table *table);
void	sister_watching(t_table *table);
void	collect_forks(t_table *table);

/*  simulation  */
int		start_simulation(t_table *table);
void	end_simulation(t_table *table);
void	remove_sem(t_table *table);

/*  eating  */
int		eating(t_philo *philo);
int		check_meals_eaten(t_sister *sister);
void	meals_eaten(t_sister *sister);

/*  check_args  */
int		check_args(int argc, char **argv);

#endif