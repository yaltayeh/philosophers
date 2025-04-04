/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:48:51 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 11:39:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_table *table)
{
	int	i;

	table->forks_lock = malloc(table->nb_philo * sizeof(pthread_mutex_t));
	if (!table->forks_lock)
		return (-1);
	i = 0;
	while (i < table->nb_philo)
	{
		table->forks_lock[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		i++;
	}
	return (0);
}

static int	init_philos(t_table *table)
{
	int	i;

	table->philos = malloc(table->nb_philo * sizeof(t_philo));
	if (!table->philos)
		return (-1);
	i = 0;
	while (i < table->nb_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].rfork = &table->forks_lock[i];
		table->philos[i].lfork = &table->forks_lock[(i + 1) % table->nb_philo];
		table->philos[i].table = table;
		i++;
	}
	return (0);
}

static int	init_timers(t_table *table)
{
	int	i;

	table->timers = malloc(table->nb_philo * sizeof(t_death_timer));
	if (!table->timers)
		return (-1);
	i = 0;
	while (i < table->nb_philo)
	{
		table->timers[i].last_meal = 0;
		table->timers[i].lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		table->timers[i].victim = &table->philos[i];
		table->timers[i].victim->timer = &table->timers[i];
		table->timers[i].t2die = table->t2die;
		i++;
	}
	return (0);
}

int	init_table(t_table *table, int argc, char **argv)
{
	memset(table, 0, sizeof(*table));
	table->nb_philo = atol(argv[1]);
	table->t2die = atol(argv[2]);
	table->t2eat = atol(argv[3]);
	table->t2sleep = atol(argv[4]);
	if (argc == 6)
		table->nb_meal = atol(argv[5]);
	else
		table->nb_meal = -1;
	table->dead_for_ever = 0;
	table->dead_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	table->print_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	if (init_forks(table) != 0)
		return (-1);
	if (init_philos(table) != 0)
		return (-1);
	if (init_timers(table))
		return (-1);
	table->start_time = get_time_now();
	return (0);
}
