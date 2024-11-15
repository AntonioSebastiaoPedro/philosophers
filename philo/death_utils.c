/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:38:02 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 20:29:11 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_death(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->alive_mutex);
	if (!philo->data->all_alive)
	{
		pthread_mutex_unlock(&philo->data->alive_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->alive_mutex);
	return (0);
}

void	check_death(t_data *data, int id)
{
	long	time_last_meal;

	if (!data->philosophers[id].just_full)
	{
		time_last_meal = get_current_time() - data->philosophers[id].last_meal;
		if ((time_last_meal >= data->time_to_die + 2))
		{
			data->all_alive = 0;
			printf("%ld %d died\n", get_current_time() - data->start_time,
				data->philosophers[id].id);
		}
	}
}

void	*monitor_die(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (data->all_alive && data->all_meals != 0)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
			all_full(data);
			pthread_mutex_lock(&data->write_mutex);
			pthread_mutex_lock(&data->alive_mutex);
			check_death(data, i);
			pthread_mutex_unlock(&data->alive_mutex);
			pthread_mutex_unlock(&data->write_mutex);
			if (!data->all_alive || !data->all_meals)
				break ;
			i++;
		}
	}
	return (NULL);
}
