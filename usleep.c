/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:50:03 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:26:05 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	usleep_ms(unsigned long long time, t_data *data)
{
	unsigned long long	start;

	start = get_time_ms();
	while (!simulation_stopped(data))
	{
		if (get_time_ms() - start >= time)
			break ;
		usleep(100);
	}
	return (0);
}
