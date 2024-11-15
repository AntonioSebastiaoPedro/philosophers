/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 03:47:58 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 17:42:14 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_for_die;

	data->start_time = get_current_time();
	data->all_alive = 1;
	if (pthread_create(&monitor_for_die, NULL, monitor_die, (void *)data) != 0)
		return (1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_join(data->philosophers[i++].thread, NULL);
	// pthread_join(monitor_for_die, NULL);
	pthread_detach(monitor_for_die);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!count_args(argc))
		return (1);
	if (!check_args(argc, argv))
		return (1);
	data.number_of_philosophers = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.meals_required = -1;
	data.all_meals = -1;
	if (argc == 6)
	{
		data.meals_required = ft_atoi(argv[5]);
		data.all_meals = data.meals_required;
	}
	init_philos(&data);
	simulation(&data);
	free_mutex_philos(&data);
	return (0);
}
