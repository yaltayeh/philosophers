/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:25:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 10:59:54 by yaltayeh         ###   ########.fr       */
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

typedef struct s_death_timer
{
	unsigned long	last_meal;
	pthread_mutex_t	lock;
	struct s_philo	*victim;
	unsigned long	t2die;
}	t_death_timer;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	int				meals_eaten;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	struct s_table	*table;
	t_death_timer	*timer;
}	t_philo;

typedef struct s_table
{
	int				nb_philo;
	t_philo			*philos;

	long			t2die;
	long			t2eat;
	long			t2sleep;
	int				nb_meal;
	
	pthread_mutex_t	*forks_lock;
	t_death_timer	*timers;
	
	int				dead_for_ever;
	pthread_mutex_t	dead_lock;

	unsigned long	start_time;
	pthread_mutex_t	print_lock;
}	t_table;

int				init_table(t_table *table, int argc, char **argv);
unsigned long	get_time_now();
void			*start_routine(void *arg);
int				best_usleep(t_table *table, unsigned long millisecond);
void			print(t_philo *philo, char *msg);
// int				check_dead(t_philo *philo);
int	check_dead(t_table *table);

#endif