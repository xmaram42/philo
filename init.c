/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:40:34 by maram             #+#    #+#             */
/*   Updated: 2025/10/24 18:40:33 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	init_mutexes(t_data *d)
{
	int	i;

	i = 0;
	d->forks = malloc(sizeof(pthread_mutex_t) * d->n);
	if (!d->forks)
		return (1);
	while (i < (int)d->n)
		pthread_mutex_init(&d->forks[i++], NULL);
	pthread_mutex_init(&d->print_lock, NULL);
	pthread_mutex_init(&d->state_lock, NULL);
	return (0);
}

int	init_philos(t_data *d)
{
	int	i;

	i = 0;
	d->philos = malloc(sizeof(t_philo) * d->n);
	if (!d->philos)
		return (1);
	while (i < (int)d->n)
	{
		d->philos[i].id = i + 1;
		d->philos[i].meals = 0;
		d->philos[i].last_meal = d->start_time;
		d->philos[i].data = d;
		d->philos[i].left_fork = &d->forks[i];
		d->philos[i].right_fork = &d->forks[(i + 1) % d->n];
		pthread_mutex_init(&d->philos[i++].meal_lock, NULL);
	}
	return (0);
}

int	init_data(t_data *d)
{
	d->stop = 0;
	if (init_mutexes(d))
		return (1);
	if (init_philos(d))
		return (1);
	return (0);
}
