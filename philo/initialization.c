/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:48:51 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 17:05:39 by yaltayeh         ###   ########.fr       */
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
		table->forks_lock[i] = (pthread_mutex_t)DEFAULT_MUTEX;
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
		table->philos[i].is_run = 0;
		table->philos[i].rfork = &table->forks_lock[i];
		table->philos[i].lfork = &table->forks_lock[(i + 1) % table->nb_philo];
		table->philos[i].table = table;
		i++;
	}
	return (0);
}

static int	init_sisters(t_table *table)
{
	int	i;

	table->sisters = malloc(table->nb_philo * sizeof(t_sister));
	if (!table->sisters)
		return (-1);
	i = 0;
	while (i < table->nb_philo)
	{
		table->sisters[i].last_meal = 0;
		table->sisters[i].time_lock = (pthread_mutex_t)DEFAULT_MUTEX;
		table->sisters[i].t2die = table->t2die;
		table->sisters[i].meals_eaten = 0;
		table->sisters[i].count_lock = (pthread_mutex_t)DEFAULT_MUTEX;
		table->sisters[i].nb_meals = table->nb_meals;
		table->sisters[i].victim = &table->philos[i];
		table->sisters[i].victim->sister = &table->sisters[i];
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
		table->nb_meals = atol(argv[5]);
	else
		table->nb_meals = -1;
	table->dead_for_ever = 0;
	table->dead_lock = (pthread_mutex_t)DEFAULT_MUTEX;
	table->print_lock = (pthread_mutex_t)DEFAULT_MUTEX;
	if (init_forks(table) != 0)
		return (-1);
	if (init_philos(table) != 0)
		return (-1);
	if (init_sisters(table))
		return (-1);
	table->start_time = get_time_now();
	return (0);
}
