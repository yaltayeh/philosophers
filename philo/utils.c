/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:54:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 15:42:17 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_now(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int	best_usleep(t_table *table, long millisecond)
{
	long	start;

	start = get_time_now();
	while (get_time_now() - start < millisecond)
	{
		if (check_dead(table))
			return (1);
		usleep(50);
	}
	return (0);
}

void	print(t_philo *philo, char *msg)
{
	int	is_dead;

	pthread_mutex_lock(&philo->table->dead_lock);
	is_dead = philo->table->dead_for_ever;
	pthread_mutex_unlock(&philo->table->dead_lock);
	if (is_dead && is_dead != philo->id)
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d %s\n", \
			get_time_now() - philo->table->start_time, \
			philo->id, msg);
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	wait_run(t_table *table)
{
	int		is_running;

	is_running = 0;
	while (!is_running)
	{
		if (pthread_mutex_lock(&table->run_lock))
			return (-1);
		is_running = table->is_running;
		if (pthread_mutex_unlock(&table->run_lock))
			return (-1);
		usleep(50);
	}
	return (is_running);
}
