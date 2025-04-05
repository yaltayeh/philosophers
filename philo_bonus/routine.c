/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:18:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 15:25:44 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->dead_lock);
	if (philo->dead)
	{
		pthread_mutex_unlock(&philo->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->dead_lock);
	return (0);
}

void	*sister_routine(void *arg)
{
	t_philo		*victim;

	victim = (t_philo *)arg;
	while (check_dead(victim) == 0)
	{
		pthread_mutex_lock(&victim->meal_lock);
		if (get_time_now() - victim->last_meal >= victim->table->t2die)
		{
			pthread_mutex_unlock(&victim->meal_lock);
			sem_wait(victim->table->dead_sem);
			pthread_mutex_lock(&victim->dead_lock);
			if (victim->dead == 0)
			{
				victim->dead = 1;
				dead_for_ever(victim->table);
			}
			pthread_mutex_unlock(&victim->dead_lock);
			sem_post(victim->table->dead_sem);
			return (NULL);
		}
		pthread_mutex_unlock(&victim->meal_lock);
		usleep(50);
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_dead(philo))
			break ;
		print(philo, "is thinking");
		if (philo->table->nb_meals != -1 \
			&& philo->meals_eaten >= philo->table->nb_meals)
		{
			sem_post(philo->table->meals_sem);
			pthread_mutex_lock(&philo->dead_lock);
			philo->dead = -1;
			pthread_mutex_unlock(&philo->dead_lock);
			break ;
		}
		if (eating(philo))
			break ;
		print(philo, "is sleeping");
		if (best_usleep(philo, philo->table->t2sleep))
			break ;
		philo->meals_eaten++;
	}
	return (NULL);
}

void	create_threads(t_philo *philo)
{
	if (pthread_create(&philo->sister_tid, NULL, sister_routine, philo) != 0)
	{
		close_semaphores(philo->table);
		exit(1);
	}
	if (pthread_create(&philo->tid, NULL, philo_routine, philo) != 0)
	{
		pthread_join(philo->sister_tid, NULL);
		close_semaphores(philo->table);
		exit(1);
	}
}

void	child_process(t_table *table, int id)
{
	t_philo	philo;

	philo.id = id;
	philo.dead = 0;
	philo.dead_lock = (pthread_mutex_t)DEFAULT_MUTEX;
	philo.meals_eaten = 0;
	philo.meal_lock = (pthread_mutex_t)DEFAULT_MUTEX;
	philo.last_meal = get_time_now();
	philo.table = table;
	create_threads(&philo);
	sem_wait(philo.table->d4e_sem);
	pthread_mutex_lock(&philo.dead_lock);
	if (philo.dead == 1)
	{
		pthread_mutex_unlock(&philo.dead_lock);
		print(&philo, "died");
		pthread_mutex_lock(&philo.dead_lock);
	}
	philo.dead = -1;
	pthread_mutex_unlock(&philo.dead_lock);
	pthread_join(philo.tid, NULL);
	pthread_join(philo.sister_tid, NULL);
	close_semaphores(philo.table);
	exit(0);
}
