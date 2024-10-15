/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 03:47:58 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/15 08:54:45 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_full(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (data->philosophers[i].meals_eaten == data->meals_required)
			data->all_meals--;
		i++;
	}
	if (data->all_meals == 0)
	{
		data->all_alive = 0;
		return (0);
	}
	return (0);
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
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_init(&data->forks[i++], NULL);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = &data->forks[i];
		data->philosophers[i].right_fork = &data->forks[(i + 1)
			% data->number_of_philosophers];
		data->philosophers[i].last_meal = get_current_time();
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].data = data;
		i++;
	}
	return (0);
}

long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	sleep_for(int time_in_ms)
{
	usleep(time_in_ms * 1000);
}

void	print_status(t_philosopher *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	if (philo->data->all_alive)
	{
		printf("%ld %d %s\n", get_current_time() - philo->data->start_time,
			philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (philo->data->all_alive)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}

		philo->last_meal = get_current_time();
		print_status(philo, "is eating");
		philo->meals_eaten++;
		if (philo->meals_eaten == philo->data->meals_required)
			philo->data->all_alive = 0;
		sleep_for(philo->data->time_to_eat);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		print_status(philo, "is sleeping");
		sleep_for(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (data->all_alive)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->write_mutex);
			long time_since_last_meal = get_current_time() - data->philosophers[i].last_meal;

			if (time_since_last_meal >= data->time_to_die + 5)
			{
				data->all_alive = 0;
				printf("%ld %d died\n", get_current_time() - data->start_time, data->philosophers[i].id);
			}
			pthread_mutex_unlock(&data->write_mutex);
			if (!data->all_alive)
				break;
			i++;
		}
	}
	return (NULL);
}

int	simulation(t_data *data)
{
	int	i;
	pthread_t	monitor;

	data->start_time = get_current_time();
	data->all_alive = 1;

	if (pthread_create(&monitor, NULL, monitor_routine, (void *)data) != 0)
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
	pthread_join(monitor, NULL);
	return (0);
}

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
	free(data->philosophers);
	free(data->forks);
}

int	main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: \
			./philo number_number_of_philosophers time_to_die time_to_eat time_to_sleep \
			[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	data.number_of_philosophers = atoi(argv[1]);
	data.time_to_die = atoi(argv[2]);
	data.time_to_eat = atoi(argv[3]);
	data.time_to_sleep = atoi(argv[4]);
	data.meals_required = -1;
	if (argc == 6)
	{
		data.meals_required = atoi(argv[5]);
		data.all_meals = data.meals_required;
	}
	init_philos(&data);
	simulation(&data);
	free_mutex_philos(&data);
	return (0);
}