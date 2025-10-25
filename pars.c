/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:43:44 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:30:27 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
			{
				printf("Error: invalid character in argument %d\n", i);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	parse_args(int ac, char **av, t_data *data)
{
	data->num_philo = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->num_eat = ft_atoi(av[5]);
	else
		data->num_eat = -1;
	if (data->num_philo <= 0 || data->time_die <= 0
		|| data->time_eat <= 0 || data->time_sleep <= 0)
	{
		printf("Error: arguments must be greater than 0\n");
		return (1);
	}
	return (0);
}
