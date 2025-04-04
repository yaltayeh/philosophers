/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 11:39:07 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	collect_forks(t_table *table)
{
	int	i;
	
	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_unlock(&table->forks_lock[i]);
		i++;
	}
}

void	death_watcher(t_table *table)
{
	int				i;
	t_death_timer	*timer;
	unsigned long	now;

	while (1)
	{
		now = get_time_now() - table->start_time;
		i = 0;
		while (i < table->nb_philo)
		{
			timer = &table->timers[i];
			pthread_mutex_lock(&timer->lock);
			if (now - timer->last_meal >= timer->t2die)
			{
				table->dead_for_ever = timer->victim->id;
				pthread_mutex_unlock(&timer->lock);
				collect_forks(table);
				print(timer->victim, "died");
				return ;
			}
			pthread_mutex_unlock(&timer->lock);
			i++;
		}
		usleep(50);
	}
}

int main(int argc, char **argv)
{
	t_table	table;
	int		i;
	
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s nb_philo time_to_die time_to_eat time_to_sleep [nb_meal]\n", argv[0]);
		return (1);
	}
	if (init_table(&table, argc, argv) != 0)
	{
		printf("init Error\n");
		return (1);
	}
	i = 0;
	while (i < table.nb_philo)
	{
		if (pthread_create(&table.philos[i].tid, NULL, \
			start_routine, &table.philos[i]))
				break ;
		i++;
	}
	
	death_watcher(&table);
	
	while (i--)
		pthread_join(table.philos[i].tid, NULL);
	free(table.philos);
	free(table.forks_lock);
	return (0);
}
