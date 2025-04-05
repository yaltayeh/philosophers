/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sister.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:21:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 21:00:21 by yaltayeh         ###   ########.fr       */
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

int	check_time(t_sister *sister, long now)
{
	t_table	*table;

	table = sister->victim->table;
	pthread_mutex_lock(&sister->time_lock);
	if (now - sister->last_meal >= sister->t2die)
	{
		pthread_mutex_unlock(&sister->time_lock);
		pthread_mutex_lock(&table->dead_lock);
		table->dead_for_ever = sister->victim->id;
		pthread_mutex_unlock(&table->dead_lock);
		collect_forks(table);
		print(sister->victim, "died");
		return (1);
	}
	pthread_mutex_unlock(&sister->time_lock);
	return (0);
}

int	check_meals(t_sister *sister)
{
	if (sister->nb_meals == -1)
		return (0);
	pthread_mutex_lock(&sister->count_lock);
	if (sister->meals_eaten >= sister->nb_meals)
	{
		pthread_mutex_unlock(&sister->count_lock);
		pthread_join(sister->victim->tid, NULL);
		sister->victim->is_run = 0;
		return (1);
	}
	pthread_mutex_unlock(&sister->count_lock);
	return (0);
}

void	sister_watching(t_table *table)
{
	int				i;
	t_sister		*sister;
	long			now;
	int				nb_running;

	nb_running = 1;
	while (nb_running)
	{
		now = get_time_now() - table->start_time;
		i = 0;
		nb_running = 0;
		while (i < table->nb_philo)
		{
			sister = &table->sisters[i];
			i++;
			if (sister->victim->is_run == 0)
				continue ;
			nb_running++;
			if (check_meals(sister))
				continue ;
			if (check_time(sister, now))
				return ;
		}
		usleep(50);
	}
}
