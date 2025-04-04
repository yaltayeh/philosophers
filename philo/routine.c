/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:18:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 16:19:30 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_meal(t_death_timer *timer)
{
	pthread_mutex_lock(&timer->lock);
	timer->last_meal = get_time_now() - timer->victim->table->start_time;
	pthread_mutex_unlock(&timer->lock);
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

int	eating(t_philo *philo)
{
	if (take_a_forks(philo))
		return (1);
	print(philo, "is eating");
	set_last_meal(philo->timer);
	philo->meals_eaten++;
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

void	*start_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id & 1)
		usleep(500);
	while (1)
	{
		if (check_dead(philo->table))
			break ;
		print(philo, "is thinking");
		if (check_dead(philo->table))
			break ;
		if (eating(philo))
			break ;
		if (check_dead(philo->table))
			break ;
		print(philo, "is sleeping");
		if (best_usleep(philo->table, philo->table->t2sleep))
			break ;
	}
	return (arg);
}
