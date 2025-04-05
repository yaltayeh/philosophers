/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 13:59:03 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	remove_sem(t_table *table)
{
	if (table->philos)
		free(table->philos);
	if (table->forks_lock)
		free(table->forks_lock);
	if (table->sisters)
		free(table->sisters);
}

int	start_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_create(&table->philos[i].tid, NULL, \
				philo_routine, &table->philos[i]))
		{
			end_simulation(table);
			printf("pthead_create failed when trying "\
					"to create thread No.%d\n", i + 1);
			return (1);
		}
		table->philos[i].is_run = 1;
		i++;
	}
	return (0);
}

void	end_simulation(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->dead_lock);
	table->dead_for_ever = -1;
	pthread_mutex_unlock(&table->dead_lock);
	collect_forks(table);
	if (table->philos)
	{
		i = 0;
		while (i < table->nb_philo)
		{
			if (table->philos[i].is_run)
			{
				pthread_join(table->philos[i].tid, NULL);
				table->philos[i].is_run = 0;
			}
			i++;
		}
	}
	remove_sem(table);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (check_args(argc, argv))
		return (1);
	if (init_table(&table, argc, argv) != 0)
	{
		printf("init Error\n");
		remove_sem(&table);
		return (1);
	}
	if (start_simulation(&table))
		return (1);
	sister_watching(&table);
	end_simulation(&table);
	return (0);
}
