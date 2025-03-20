/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:37:43 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/20 09:57:27 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_process(t_table *table, int i)
{
	table->philos[i].pid = fork();
	if (table->philos[i].pid == -1)
		return (-1);
	if (table->philos[i].pid == 0)
	{
		exit(0);
	}
	return (0);
}