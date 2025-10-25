/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:40:28 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:27:24 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < (int)data->num_philo)
	{
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		pthread_mutex_destroy(&data->fork_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->die_mutex);
	free(data->fork_mutex);
	free(data->philos);
}
