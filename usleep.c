/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:50:03 by maram             #+#    #+#             */
/*   Updated: 2025/10/22 14:50:04 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	usleep_ms(unsigned long long time)
{
	unsigned long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time)
		usleep(500);
	return (0);
}
