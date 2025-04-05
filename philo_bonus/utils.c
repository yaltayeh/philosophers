/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:54:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 15:10:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_now(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int	best_usleep(t_philo *philo, long millisecond)
{
	long	start;

	start = get_time_now();
	while (get_time_now() - start < millisecond)
	{
		if (check_dead(philo))
			return (1);
		usleep(50);
	}
	return (0);
}

int	print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->dead_lock);
	if (philo->dead == -1)
	{
		pthread_mutex_unlock(&philo->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->dead_lock);
	sem_wait(philo->table->print_sem);
	printf("%lu %d %s\n", \
		get_time_now() - philo->table->start_time, \
		philo->id, msg);
	sem_post(philo->table->print_sem);
	return (0);
}
