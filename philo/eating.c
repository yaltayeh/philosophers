/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:34:50 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/06 17:38:18 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_a_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->rfork);
	if (check_dead(philo->table))
	{
		pthread_mutex_unlock(philo->rfork);
		return (1);
	}
	print(philo, "has taken a fork");
	pthread_mutex_lock(philo->lfork);
	if (check_dead(philo->table))
	{
		pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
		return (1);
	}
	print(philo, "has taken a fork");
	return (0);
}

void	meals_eaten(t_sister *sister)
{
	if (sister->nb_meals != -1)
	{
		pthread_mutex_lock(&sister->count_lock);
		sister->meals_eaten++;
		pthread_mutex_unlock(& sister->count_lock);
	}
}

int	check_meals_eaten(t_sister *sister)
{
	if (sister->nb_meals != -1)
	{
		pthread_mutex_lock(&sister->count_lock);
		if (sister->meals_eaten >= sister->nb_meals)
		{
			pthread_mutex_unlock(&sister->count_lock);
			return (1);
		}
		pthread_mutex_unlock(& sister->count_lock);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	if (philo->table->nb_philo == 1)
	{
		while (check_dead(philo->table) == 0)
			usleep(50);
		return (1);
	}
	if (take_a_forks(philo))
		return (1);
	print(philo, "is eating");
	pthread_mutex_lock(&philo->sister->time_lock);
	philo->sister->last_meal = \
		get_time_now() - philo->sister->victim->table->start_time;
	pthread_mutex_unlock(&philo->sister->time_lock);
	if (best_usleep(philo->table, philo->table->t2eat))
	{
		pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
		return (1);
	}
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
	return (0);
}
