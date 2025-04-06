/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:18:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/06 14:33:08 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id & 1)
		usleep(500);
	while (1)
	{
		if (check_dead(philo->table))
			break ;
		print(philo, "is thinking");
		if (check_meals_eaten(philo->sister))
			break ;
		if (check_dead(philo->table))
			break ;
		if (eating(philo))
			break ;
		if (check_dead(philo->table))
			break ;
		print(philo, "is sleeping");
		if (best_usleep(philo->table, philo->table->t2sleep))
			break ;
		meals_eaten(philo->sister);
	}
	return (arg);
}
