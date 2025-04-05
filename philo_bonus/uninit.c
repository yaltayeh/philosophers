/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uninit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:31:24 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/05 14:48:42 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_semaphores(t_table *table)
{
	if (table->forks_sem != SEM_FAILED)
		sem_close(table->forks_sem);
	if (table->d4e_sem != SEM_FAILED)
		sem_close(table->d4e_sem);
	if (table->print_sem != SEM_FAILED)
		sem_close(table->print_sem);
	if (table->meals_sem != SEM_FAILED)
		sem_close(table->meals_sem);
	if (table->dead_sem != SEM_FAILED)
		sem_close(table->dead_sem);
	table->forks_sem = SEM_FAILED;
	table->d4e_sem = SEM_FAILED;
	table->print_sem = SEM_FAILED;
	table->meals_sem = SEM_FAILED;
	table->dead_sem = SEM_FAILED;
}

void	remove_semaphores(t_table *table)
{
	close_semaphores(table);
	sem_unlink(SEM_FORKS_NAME);
	sem_unlink(SEM_D4E_NAME);
	sem_unlink(SEM_PRINT_NAME);
	sem_unlink(SEM_MEALS_NAME);
	sem_unlink(SEM_DEAD_NAME);
}

void	dead_for_ever(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->d4e_sem);
		i++;
	}
	usleep(100);
	i = 0;
	while (i < table->nb_philo)
	{
		sem_post(table->forks_sem);
		i++;
	}
}
