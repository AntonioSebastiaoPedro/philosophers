/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:47:28 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 16:54:36 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
