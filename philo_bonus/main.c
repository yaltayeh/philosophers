/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:31:31 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/19 17:39:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <signal.h>

void	error_handler(t_table *table)
{
	int	i;

	(void)table;
	i = 0;
	while (i < table->nb_philo)
	{
		if (table->philos[i].pid)
			kill(table->philos[i].pid, SIGKILL);
		table->philos[i].pid = 0;
		i++;
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
		table.philos[i].pid = fork();
		if (table.philos[i].pid == -1)
			error_handler(&table);
		if (table.philos[i].pid == 0)
		{
			exit(0);
		}
		i++;
	}
	if (fork)
	return (0);
}

