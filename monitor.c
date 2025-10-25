/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:41:48 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:36:22 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_stop(t_data *data)
{
	pthread_mutex_lock(&data->die_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->die_mutex);
}

bool	simulation_stopped(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->die_mutex);
	stopped = data->dead;
	pthread_mutex_unlock(&data->die_mutex);
	return (stopped);
}

int	check_philo_state(t_data *d, int i, int *finished)
{
	long long	last;
	int			meals;

	pthread_mutex_lock(&d->philos[i].meal_lock);
	last = d->philos[i].last_meal;
	meals = d->philos[i].meals;
	pthread_mutex_unlock(&d->philos[i].meal_lock);
	if (get_time_ms() - last > d->time_die)
	{
		set_stop(d);
		print_state(&d->philos[i], DEAD);
		return (1);
	}
	if (d->num_eat > 0 && meals >= d->num_eat)
		(*finished)++;
	return (0);
}

int	check_all_philos(t_data *d)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < (int)d->num_philo)
	{
		if (check_philo_state(d, i, &finished))
			return (1);
		i++;
	}
	if (d->num_eat > 0 && finished == (int)d->num_philo)
	{
		set_stop(d);
		return (1);
	}
	return (0);
}
void	*monitor(void *arg)
{
	t_data			*data;
	unsigned int	i;

	data = (t_data *)arg;
	while (!simulation_stopped(data))
	{
		i = 0;
		while (i < data->num_philo && !simulation_stopped(data))
		{
			pthread_mutex_lock(&data->philos[i].meal_lock);
			if (get_time_ms() - data->philos[i].last_meal > data->time_die)
			{
				print_state(&data->philos[i], DEAD);
				set_stop(data);
				pthread_mutex_unlock(&data->philos[i].meal_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
