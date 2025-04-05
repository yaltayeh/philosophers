/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:48:51 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 14:48:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static sem_t	*create_semaphore(const char *name, unsigned int value)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0644, value);
	if (sem == SEM_FAILED)
	{
		printf("Error: sem_open: %s\n", name);
		return (sem);
	}
	return (sem);
}

static int	init_semaphores(t_table *table)
{
	table->forks_sem = create_semaphore(SEM_FORKS_NAME, table->nb_philo);
	if (table->forks_sem == SEM_FAILED)
		return (1);
	table->d4e_sem = create_semaphore(SEM_D4E_NAME, 0);
	if (table->d4e_sem == SEM_FAILED)
		return (1);
	table->print_sem = create_semaphore(SEM_PRINT_NAME, 1);
	if (table->print_sem == SEM_FAILED)
		return (1);
	table->meals_sem = create_semaphore(SEM_MEALS_NAME, 0);
	if (table->meals_sem == SEM_FAILED)
		return (1);
	table->dead_sem = create_semaphore(SEM_DEAD_NAME, 1);
	if (table->dead_sem == SEM_FAILED)
		return (1);
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
	table->forks_sem = SEM_FAILED;
	table->d4e_sem = SEM_FAILED;
	table->print_sem = SEM_FAILED;
	table->meals_sem = SEM_FAILED;
	table->dead_sem = SEM_FAILED;
	if (init_semaphores(table))
		return (1);
	table->start_time = get_time_now();
	return (0);
}
