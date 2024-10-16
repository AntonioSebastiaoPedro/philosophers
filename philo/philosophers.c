/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 03:47:58 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/16 10:51:14 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	all_full(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (data->philosophers[i].meals_eaten == data->meals_required)
		{
			data->all_meals--;
			data->philosophers[i].just_full = 1;
		}
		i++;
	}
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
		data->philosophers[i].just_full = 0;
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
	while (philo->data->all_alive && philo->data->all_meals)
	{
		// if (philo->data->number_of_philosophers == 1)
		// {
		// 	pthread_mutex_lock(philo->left_fork);
		// 	print_status(philo, "has taken a fork");
		// 	print_status(philo, "died");
		// 	pthread_mutex_destroy(philo->left_fork);
		// 	philo->data->all_alive = 0;
		// }
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
		usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// if (philo->meals_eaten == philo->data->meals_required)
		// {
		// 	philo->just_full = 1;
		// 	break ;
		// }
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*monitor_die(void *arg)
{
	t_data	*data;
	int		i;
	long	time_since_last_meal;

	data = (t_data *)arg;
	while (data->all_alive && data->all_meals)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
		all_full(data);
			pthread_mutex_lock(&data->write_mutex);
			if (data->philosophers[i].just_full == 0)
			{
				time_since_last_meal = get_current_time()
					- data->philosophers[i].last_meal;
				if ((time_since_last_meal >= data->time_to_die + 5))
				{
					data->all_alive = 0;
					printf("%ld %d died\n", get_current_time()
						- data->start_time, data->philosophers[i].id);
				}
			}
			pthread_mutex_unlock(&data->write_mutex);
			if (!data->all_alive || !data->all_meals)
				break ;
			i++;
		}
	}
	return (NULL);
}

// void	*monitor_meals(void *arg)
// {
// 	t_data	*data;
// 	int		i;

// 	data = (t_data *)arg;
// 	while (data->all_meals)
// 	{
// 		i = 0;
// 		while (i < data->number_of_philosophers)
// 		{
// 			if ( data->philosophers[i].meals_eaten >= data->meals_required)
// 				data->all_meals -= 1;
// 			if (!data->all_meals)
// 				break ;
// 			i++;
// 		}
// 	}
// 	return (NULL);
// }

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
	pthread_join(monitor_for_die, NULL);
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
	t_data	data;

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
