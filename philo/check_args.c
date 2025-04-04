/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:58:45 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/04/04 20:20:10 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

static void	usage_msg(void)
{
	printf("Usage: philo nb_philo time_to_die time_to_eat " \
		"time_to_sleep [nb_meal]\n");
}

static int	is_pos_number(char *s)
{
	if (*s == '+')
		s++;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		s++;
	}
	return (1);
}

void	print_number_error_msg(int argc, char **argv, int i)
{
	int	indents[5];

	if (argc == 5)
		printf("philo %n%s %n%s %n%s %n%s\n", \
			&indents[0], argv[1], \
			&indents[1], argv[2], \
			&indents[2], argv[3], \
			&indents[3], argv[4]);
	if (argc == 6)
		printf("philo %n%s %n%s %n%s %n%s %n%s\n", \
			&indents[0], argv[1], \
			&indents[1], argv[2], \
			&indents[2], argv[3], \
			&indents[3], argv[4], \
			&indents[4], argv[5]);
	printf("%*s^\n", indents[i - 1], "");
	printf("Error: %s in not positive number\n", argv[i]);
}

int	check_args(int argc, char **argv)
{
	int	i;
	
	if (argc != 5 && argc != 6)
	{
		usage_msg();
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (is_pos_number(argv[i]) == 0)
		{
			print_number_error_msg(argc, argv, i);
			return (1);
		}
		i++;
	}
	return (0);
}