/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:54:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/03/23 12:58:51 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
# include <stdlib.h>

unsigned long	get_time_now()
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
