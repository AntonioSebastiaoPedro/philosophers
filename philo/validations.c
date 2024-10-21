/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:18:31 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 17:53:39 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	count_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		printf("Usage: ./philo number_of_philosophers");
		printf(" time_to_die time_to_eat time_to_sleep");
		printf(" [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	return (1);
}

void	print_args_error(int i)
{
	if (i == 1)
		printf("Invalid number of philosophers\n");
	else if (i == 2)
		printf("Invalid time to die\n");
	else if (i == 3)
		printf("Invalid time to eat\n");
	else if (i == 4)
		printf("Invalid time to sleep\n");
	else if (i == 5)
		printf("Invalid number of times each philosopher must eat\n");
}

int	check_args(int ac, char **av)
{
	int	i;
	int	arg;

	i = 1;
	arg = 0;
	while (i < ac)
	{
		arg = ft_atoi(av[i]);
		if (!ft_isint(av[i]) || arg < 1)
		{
			print_args_error(i);
			return (0);
		}
		i++;
	}
	return (1);
}
