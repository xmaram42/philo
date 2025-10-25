/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:40:34 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:08:01 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL));
}

int	init_mutexes(t_data *d)
{
	int	i;

	i = 0;
	d->fork_mutex = malloc(sizeof(pthread_mutex_t) * d->num_philo);
	if (!d->fork_mutex)
		return (1);
	while (i < (int)d->num_philo)
		pthread_mutex_init(&d->fork_mutex[i++], NULL);
	pthread_mutex_init(&d->print_mutex, NULL);
	pthread_mutex_init(&d->eat_mutex, NULL);
	pthread_mutex_init(&d->die_mutex, NULL);
	return (0);
}

int	init_philos(t_data *d)
{
	int	i;

	i = 0;
	d->philos = malloc(sizeof(t_philo) * d->num_philo);
	if (!d->philos)
		return (1);
	while (i < (int)d->num_philo)
	{
		d->philos[i].id = i + 1;
		d->philos[i].meals = 0;
		d->philos[i].last_meal = d->start_time;
		d->philos[i].data = d;
		d->philos[i].left_fork = &d->fork_mutex[i];
		d->philos[i].right_fork = &d->fork_mutex[(i + 1) % d->num_philo];
		pthread_mutex_init(&d->philos[i++].meal_lock, NULL);
	}
	return (0);
}

int	init_data(t_data *d)
{
	d->dead = 0;
	d->start_time = get_time_ms();
	if (init_mutexes(d))
		return (1);
	if (init_philos(d))
		return (1);
	return (0);
}
