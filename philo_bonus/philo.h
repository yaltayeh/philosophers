/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:25:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/19 17:36:11 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>


typedef struct s_philo
{
	int				id;
	pid_t			pid;
	// pthread_t		tid;
	// unsigned long	last_meal;
	// int				meals_eaten;
	// pthread_mutex_t	*lfork;
	// pthread_mutex_t	*rfork;
	struct s_table	*table;
}	t_philo;



typedef struct s_table
{
	int				nb_philo;
	t_philo			*philos;

	long			t2die;
	long			t2eat;
	long			t2sleep;
	
	int				nb_meal;
	
	int				dead_for_ever;
	unsigned long	start_time;
	// pthread_mutex_t	dead_lock;
	// pthread_mutex_t	*forks_lock;
	
	// pthread_mutex_t	print_lock;
}	t_table;

int				init_table(t_table *table, int argc, char **argv);
unsigned long	get_time_now();

#endif