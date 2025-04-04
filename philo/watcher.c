/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:21:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 19:45:52 by yaltayeh         ###   ########.fr       */
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

int	check_time(t_death_timer *timer, long now)
{
	t_table	*table;

	table = timer->victim->table;
	pthread_mutex_lock(&timer->time_lock);
	if (now - timer->last_meal >= timer->t2die)
	{
		pthread_mutex_unlock(&timer->time_lock);
		pthread_mutex_lock(&table->dead_lock);
		table->dead_for_ever = timer->victim->id;
		pthread_mutex_unlock(&table->dead_lock);
		collect_forks(table);
		print(timer->victim, "died");
		return (1);
	}
	pthread_mutex_unlock(&timer->time_lock);
	return (0);
}

int	check_meals(t_death_timer *timer)
{
	if (timer->nb_meals == -1)
		return (0);
	pthread_mutex_lock(&timer->count_lock);
	if (timer->meals_eaten >= timer->nb_meals)
	{
		pthread_mutex_unlock(&timer->count_lock);
		pthread_join(timer->victim->tid, NULL);
		timer->victim->is_run = 0;
		return (1);
	}
	pthread_mutex_unlock(&timer->count_lock);
	return (0);
}

void	death_watcher(t_table *table)
{
	int				i;
	t_death_timer	*timer;
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
			timer = &table->timers[i];
			i++;
			if (timer->victim->is_run == 0)
				continue ;
			nb_running++;
			if (check_meals(timer))
				continue ;
			if (check_time(timer, now))
				return ;
		}
		usleep(50);
	}
}
