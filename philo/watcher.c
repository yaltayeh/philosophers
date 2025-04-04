/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
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

int	check_time(t_watcher *watcher, long now)
{
	t_table	*table;

	table = watcher->victim->table;
	pthread_mutex_lock(&watcher->time_lock);
	if (now - watcher->last_meal >= watcher->t2die)
	{
		pthread_mutex_unlock(&watcher->time_lock);
		pthread_mutex_lock(&table->dead_lock);
		table->dead_for_ever = watcher->victim->id;
		pthread_mutex_unlock(&table->dead_lock);
		collect_forks(table);
		print(watcher->victim, "died");
		return (1);
	}
	pthread_mutex_unlock(&watcher->time_lock);
	return (0);
}

int	check_meals(t_watcher *watcher)
{
	if (watcher->nb_meals == -1)
		return (0);
	pthread_mutex_lock(&watcher->count_lock);
	if (watcher->meals_eaten >= watcher->nb_meals)
	{
		pthread_mutex_unlock(&watcher->count_lock);
		pthread_join(watcher->victim->tid, NULL);
		watcher->victim->is_run = 0;
		return (1);
	}
	pthread_mutex_unlock(&watcher->count_lock);
	return (0);
}

void	death_watcher(t_table *table)
{
	int				i;
	t_watcher	*watcher;
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
			watcher = &table->watchers[i];
			i++;
			if (watcher->victim->is_run == 0)
				continue ;
			nb_running++;
			if (check_meals(watcher))
				continue ;
			if (check_time(watcher, now))
				return ;
		}
		usleep(50);
	}
}
