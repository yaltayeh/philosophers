/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:21:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 17:36:13 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_dead(t_table *table)
{
	pthread_mutex_lock(&table->dead_lock);
	if (table->dead_for_ever)
	{
		pthread_mutex_unlock(&table->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->dead_lock);
	return (0);
}

void	collect_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_unlock(&table->forks_lock[i]);
		i++;
	}
}

void	death_watcher(t_table *table)
{
	int				i;
	t_death_timer	*timer;
	unsigned long	now;

	while (1)
	{
		now = get_time_now() - table->start_time;
		i = 0;
		while (i < table->nb_philo)
		{
			timer = &table->timers[i];
			pthread_mutex_lock(&timer->lock);
			if (now - timer->last_meal >= timer->t2die)
			{
				table->dead_for_ever = timer->victim->id;
				pthread_mutex_unlock(&timer->lock);
				collect_forks(table);
				print(timer->victim, "died");
				return ;
			}
			pthread_mutex_unlock(&timer->lock);
			i++;
		}
		usleep(50);
	}
}
