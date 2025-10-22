/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:47:13 by maram             #+#    #+#             */
/*   Updated: 2025/10/22 14:47:19 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork 🍴");
	usleep_ms(philo->data->die);
	pthread_mutex_unlock(philo->left_fork);
}

void	print_state(t_philo *philo, char *msg)
{
	long long	timestamp;
	int			stop;

	pthread_mutex_lock(&philo->data->print_lock);
	timestamp = get_time_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->state_lock);
	stop = philo->data->stop;
	pthread_mutex_unlock(&philo->data->state_lock);
	if (!stop || ft_strcmp(msg, "died 💀") == 0)
		printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->id % 2 == 0)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	else
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	pthread_mutex_lock(first);
	print_state(philo, "has taken a fork 🍴");
	if (first == second)
	{
		pthread_mutex_unlock(first);
		return ;
	}
	pthread_mutex_lock(second);
	print_state(philo, "has taken a fork 🍴");
}

void	eat_meal(t_philo *philo)
{
	print_state(philo, "is eating 🍝");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	usleep_ms(philo->data->eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philo_routine(void *ar)
{
	t_philo	*philo;

	philo = (t_philo *)ar;
	if (philo->left_fork == philo->right_fork)
	{
		print_state(philo, "is thinking 💭");
		return (handle_single_philo(philo), NULL);
	}
	if (philo->id % 2 == 0)
		usleep_ms(philo->data->eat / 2);
	else if (philo->data->n % 2 != 0)
		usleep_ms(philo->data->eat);
	while (!simulation_stopped(philo->data))
	{
		print_state(philo, "is thinking 💭");
		take_forks(philo);
		eat_meal(philo);
		print_state(philo, "is sleeping 😴");
		usleep_ms(philo->data->sleep);
		if (philo->data->n % 2 == 0)
			usleep_ms(1);
		else
			usleep_ms(philo->data->eat);
	}
	return (NULL);
}
