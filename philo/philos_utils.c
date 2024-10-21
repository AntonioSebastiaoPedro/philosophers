/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:35:49 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 17:01:30 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_mutex_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->alive_mutex);
	free(data->philosophers);
	free(data->forks);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		if (is_death(philo))
			break ;
		if (philo->data->number_of_philosophers == 1)
		{
			print_status(philo, "has taken a fork");
			usleep(philo->data->time_to_die * 1000);
			break ;
		}
		take_forks(philo);
		philo->last_meal = get_current_time();
		print_status(philo, "is eating");
		usleep(philo->data->time_to_eat * 1000);
		philo->meals_eaten++;
		unlock_forks(philo);
		if (philo->meals_eaten == philo->data->meals_required)
			break ;
		sleep_philo(philo);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philosophers = malloc(sizeof(t_philosopher)
			* data->number_of_philosophers);
	data->forks = malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers);
	if (!data->philosophers || !data->forks)
		return (1);
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->alive_mutex, NULL);
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_init(&data->forks[i++], NULL);
	i = 0;
	fill_philos(data);
	return (0);
}

void	sleep_philo(t_philosopher *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	fill_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = &data->forks[i];
		data->philosophers[i].right_fork = &data->forks[(i + 1)
			% data->number_of_philosophers];
		data->philosophers[i].last_meal = get_current_time();
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].just_full = 0;
		data->philosophers[i].data = data;
		i++;
	}
}
