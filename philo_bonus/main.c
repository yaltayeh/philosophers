/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 15:39:35 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*meals_counter(void *arg)
{
	t_table	*table;
	long	philo_end_eat;

	table = (t_table *)arg;
	philo_end_eat = 0;
	while (philo_end_eat < table->nb_philo)
	{
		sem_wait(table->meals_sem);
		philo_end_eat++;
	}
	dead_for_ever(table);
	return (NULL);
}

int	start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;

	if (table->nb_meals != -1 && \
		pthread_create(&table->meals_counter, NULL, meals_counter, table) == -1)
	{
		printf("pthread_create: create meals_counter\n");
		return (1);
	}
	i = 0;
	while (i < table->nb_philo)
	{
		pid = fork();
		if (pid == 0)
			child_process(table, i + 1);
		if (pid == -1)
		{
			printf("fork failed when trying "\
					"to create thread No.%d\n", i + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void	end_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->d4e_sem);
		i++;
	}
	while (waitpid(-1, NULL, 0) != -1)
		;
	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->meals_sem);
		i++;
	}
	pthread_join(table->meals_counter, NULL);
	remove_semaphores(table);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (check_args(argc, argv))
		return (1);
	if (init_table(&table, argc, argv) != 0)
	{
		remove_semaphores(&table);
		return (1);
	}
	if (start_simulation(&table))
	{
		end_simulation(&table);
		return (1);
	}
	while (waitpid(-1, NULL, 0) != -1)
		;
	end_simulation(&table);
	return (0);
}
