/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:34:50 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 20:59:43 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_meal(t_watcher *watcher)
{
	pthread_mutex_lock(&watcher->time_lock);
	watcher->last_meal = get_time_now() - watcher->victim->table->start_time;
	pthread_mutex_unlock(&watcher->time_lock);
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

void	meals_eaten(t_watcher *watcher)
{
	if (watcher->nb_meals != -1)
	{
		pthread_mutex_lock(&watcher->count_lock);
		watcher->meals_eaten++;
		pthread_mutex_unlock(& watcher->count_lock);
	}
}

int	check_meals_eaten(t_watcher *watcher)
{
	if (watcher->nb_meals != -1)
	{
		pthread_mutex_lock(&watcher->count_lock);
		if (watcher->meals_eaten >= watcher->nb_meals)
		{
			pthread_mutex_unlock(&watcher->count_lock);
			return (1);
		}
		pthread_mutex_unlock(& watcher->count_lock);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	if (take_a_forks(philo))
		return (1);
	print(philo, "is eating");
	set_last_meal(philo->watcher);
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
