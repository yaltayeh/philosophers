/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:34:50 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 15:28:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_a_forks(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	if (check_dead(philo))
	{
		sem_post(philo->table->forks_sem);
		return (1);
	}
	print(philo, "has taken a fork");
	sem_wait(philo->table->forks_sem);
	if (check_dead(philo))
	{
		sem_post(philo->table->forks_sem);
		sem_post(philo->table->forks_sem);
		return (1);
	}
	print(philo, "has taken a fork");
	return (0);
}

int	eating(t_philo *philo)
{
	if (check_dead(philo))
		return (1);
	if (take_a_forks(philo))
		return (1);
	print(philo, "is eating");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_now();
	pthread_mutex_unlock(&philo->meal_lock);
	if (best_usleep(philo, philo->table->t2eat))
	{
		sem_post(philo->table->forks_sem);
		sem_post(philo->table->forks_sem);
		return (1);
	}
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
	if (check_dead(philo))
		return (1);
	return (0);
}
