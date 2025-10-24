/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:41:48 by maram             #+#    #+#             */
/*   Updated: 2025/10/24 18:32:04 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_stop(t_data *data)
{
	pthread_mutex_lock(&data->state_lock);
	data->stop = 1;
	pthread_mutex_unlock(&data->state_lock);
}

int	simulation_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->state_lock);
	stop = data->stop;
	pthread_mutex_unlock(&data->state_lock);
	return (stop);
}

int	check_philo_state(t_data *d, int i, int *finished)
{
	long long	last;
	int			meals;

	pthread_mutex_lock(&d->philos[i].meal_lock);
	last = d->philos[i].last_meal;
	meals = d->philos[i].meals;
	pthread_mutex_unlock(&d->philos[i].meal_lock);
	if (get_time_ms() - last > d->die)
	{
		set_stop(d);
		print_state(&d->philos[i], "died 💀");
		return (1);
	}
	if (d->must_eat > 0 && meals >= d->must_eat)
		(*finished)++;
	return (0);
}

int	check_all_philos(t_data *d)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < (int)d->n)
	{
		if (check_philo_state(d, i, &finished))
			return (1);
		i++;
	}
	if (d->must_eat > 0 && finished == (int)d->n)
	{
		set_stop(d);
		return (1);
	}
	return (0);
}

void	*monitor(void *ar)
{
	t_data	*d;

	d = (t_data *)ar;
	while (!simulation_stopped(d))
	{
		if (check_all_philos(d))
			break ;
		usleep(1000);
	}
	return (NULL);
}
