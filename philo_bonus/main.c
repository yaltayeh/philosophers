/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:31:31 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/16 01:32:03 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	
	return (0);
}

