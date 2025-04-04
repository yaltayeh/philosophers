/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:54:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 16:16:53 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time_now(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

int	best_usleep(t_table *table, unsigned long millisecond)
{
	unsigned long	start;

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
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d %s\n", \
			get_time_now() - philo->table->start_time, \
			philo->id, msg);
	pthread_mutex_unlock(&philo->table->print_lock);
}
