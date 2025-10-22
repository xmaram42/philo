/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:41:14 by maram             #+#    #+#             */
/*   Updated: 2025/10/22 15:07:30 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_threads(t_data *d)
{
	int	i;

	i = 0;
	while (i < (int)d->n)
	{
		d->philos[i].last_meal = d->start_time;
		if (pthread_create(&d->philos[i].thread, NULL,
				philo_routine, &d->philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	join_threads(t_data *d)
{
	int	i;

	i = 0;
	while (i < (int)d->n)
		pthread_join(d->philos[i++].thread, NULL);
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor_thread;

	if (validate_args(ac, av) || parse_args(ac, av, &data))
		return (1);
	if (init_data(&data))
		return (printf("Error initializing data\n"), 1);
	data.start_time = get_time_ms();
	if (start_threads(&data))
		return (printf("Thread creation failed\n"), 1);
	pthread_create(&monitor_thread, NULL, monitor, &data);
	pthread_join(monitor_thread, NULL);
	join_threads(&data);
	cleanup_data(&data);
	return (0);
}
