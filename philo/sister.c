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
	int	is_dead;

	pthread_mutex_lock(&table->dead_lock);
	is_dead = table->dead_for_ever;
	pthread_mutex_unlock(&table->dead_lock);
	return (is_dead);
}

int	check_time(t_sister *sister, long now)
{
	t_table	*table;
	long	last_meal;

	table = sister->victim->table;
	pthread_mutex_lock(&sister->time_lock);
	last_meal = sister->last_meal;
	pthread_mutex_unlock(&sister->time_lock);
	if (now - last_meal >= sister->t2die)
	{
		pthread_mutex_lock(&table->dead_lock);
		table->dead_for_ever = sister->victim->id;
		pthread_mutex_unlock(&table->dead_lock);
		print(sister->victim, "died");
		return (1);
	}
	return (0);
}

int	check_meals(t_sister *sister)
{
	int	meals_eaten;

	if (sister->nb_meals == -1)
		return (0);
	pthread_mutex_lock(&sister->count_lock);
	meals_eaten = sister->meals_eaten;
	pthread_mutex_unlock(&sister->count_lock);

	if (meals_eaten >= sister->nb_meals)
	{
		sister->victim->is_created = 0;
		return (1);
	}
	return (0);
}

void	sister_watching(t_table *table)
{
	int				i;
	t_sister		*sister;
	long			now;
	int				nb_finished;

	nb_finished = 0;
	while (nb_finished < table->nb_philo)
	{
		now = get_time_now() - table->start_time;
		i = 0;
		while (i < table->nb_philo)
		{
			sister = &table->philos[i].sister;
			i++;
			// if (sister->victim->is_created == 0)
			// 	continue ;
			// nb_running++;
			// if (check_meals(sister))
			// 	continue ;
			if (check_time(sister, now))
				return ;
		}
	}
}
