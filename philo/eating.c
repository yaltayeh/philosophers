/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:34:50 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 23:18:33 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_meal(t_sister *sister)
{
	pthread_mutex_lock(&sister->time_lock);
	sister->last_meal = get_time_now() - sister->victim->table->start_time;
	pthread_mutex_unlock(&sister->time_lock);
}

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
	if (take_a_forks(philo))
		return (1);
	print(philo, "is eating");
	set_last_meal(philo->sister);
	if (best_usleep(philo->table, philo->table->t2eat))
	{
		pthread_mutex_unlock(philo->lfork);
		pthread_mutex_unlock(philo->rfork);
		return (1);
	}
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
	return (0);
}
