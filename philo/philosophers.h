/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 17:46:22 by ansebast          #+#    #+#             */
/*   Updated: 2024/10/21 17:39:10 by ansebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	int				just_full;
	long			last_meal;
	pthread_t		thread;
	struct s_data	*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philosopher;

typedef struct s_data
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				all_meals;
	int				all_alive;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	alive_mutex;
	t_philosopher	*philosophers;
}					t_data;

int					ft_atoi(char *str);
int					ft_isint(char *str);
int					count_args(int ac);
int					check_args(int ac, char **av);
int					is_death(t_philosopher *philo);
int					init_philos(t_data *data);
long				get_current_time(void);
void				print_status(t_philosopher *philo, const char *status);
void				check_death(t_data *data, int id);
void				*monitor_die(void *arg);
void				take_forks(t_philosopher *philo);
void				unlock_forks(t_philosopher *philo);
void				all_full(t_data *data);
void				free_mutex_philos(t_data *data);
void				*philosopher_routine(void *arg);
void				sleep_philo(t_philosopher *philo);
void				fill_philos(t_data *data);

#endif
