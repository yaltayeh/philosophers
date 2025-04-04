/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dead.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:19:45 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 10:54:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
int	check_dead(t_philo *philo)
{
	unsigned long	delta;

	pthread_mutex_lock(&philo->table->dead_lock);
	if (philo->table->dead_for_ever)
	{
		pthread_mutex_unlock(&philo->table->dead_lock);
		return (1);
	}
	delta = get_time_now() - philo->last_meal;
	if (delta >= (unsigned long)philo->table->t2die)
	{
		print(philo, "died");
		philo->table->dead_for_ever = 1;
		pthread_mutex_unlock(&philo->table->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->dead_lock);
	return (0);
}
*/

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