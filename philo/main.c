/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/29 13:36:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d %s\n", \
			get_time_now() - philo->table->start_time, \
			philo->id, msg);
	pthread_mutex_unlock(&philo->table->print_lock);
}



int	check_dead(t_philo *philo)
{
	unsigned long	delta;

	pthread_mutex_lock(&philo->table->dead_lock);
	if (philo->table->dead_for_ever)
	{
		pthread_mutex_unlock(&philo->table->dead_lock);
		return (1);
	}
	delta = get_time_now() - philo->last_meal;
	if (delta >= (unsigned long)philo->table->t2die)
	{
		print(philo, "died");
		philo->table->dead_for_ever = 1;
		pthread_mutex_unlock(&philo->table->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->dead_lock);
	return (0);
}

int	best_usleep(t_table *table, unsigned long millisecond)
{
	unsigned long	start;

	start = get_time_now();
	while (get_time_now() - start < millisecond)
	{
		if (check_dead(&table->philos[0]))
			return (1);
		usleep(50);
	}
	return (0);
}

int eating(t_philo *philo)
{
	
	pthread_mutex_lock(philo->rfork);
	if (check_dead(philo))
	{
		pthread_mutex_unlock(philo->rfork);
		return (1);
	}
	print(philo, "has taken a fork");
	pthread_mutex_lock(philo->lfork);
	if (check_dead(philo))
	{
		pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
		return (1);
	}
	print(philo, "has taken a fork");
	print(philo, "is eating");
	philo->last_meal = get_time_now();
	philo->meals_eaten++;
	if (best_usleep(philo->table, philo->table->t2eat))
	{
		pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
		return (1);
	}
	
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
	
	return (0);
}
void	*start_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 ==0)
		usleep(500);
	while (!philo->table->dead_for_ever)
	{
		if (check_dead(philo))
			break ;
		print(philo, "is thinking");
		if (check_dead(philo))
			break ;
		if (eating(philo))
			break ;
		if (check_dead(philo))
			break ;
		print(philo, "is sleeping");
		if (best_usleep(philo->table, philo->table->t2sleep))
			break ;
	}
	return arg;
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
	while (i--)
		pthread_join(table.philos[i].tid, NULL);
	free(table.philos);
	free(table.forks_lock);
	return (0);
}