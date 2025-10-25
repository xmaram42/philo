/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:47:13 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:25:21 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, FORK);
	usleep_ms(philo->data->time_die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}

/* ================================================== */
/* =============== PRINT STATE ======================= */
/* ================================================== */
void	print_state(t_philo *philo, int state)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	timestamp = get_time_ms() - philo->data->start_time;
	if (!simulation_stopped(philo->data) || state == DEAD)
	{
		if (state == EAT)
			printf("%lld %d is eating 🍝\n", timestamp, philo->id);
		else if (state == SLEEP)
			printf("%lld %d is sleeping 😴\n", timestamp, philo->id);
		else if (state == THINK)
			printf("%lld %d is thinking 💭\n", timestamp, philo->id);
		else if (state == FORK)
			printf("%lld %d has taken a fork 🍴\n", timestamp, philo->id);
		else if (state == DEAD)
			printf("%lld %d died 💀\n", timestamp, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/* ================================================== */
/* =============== TAKE FORKS ======================== */
/* ================================================== */
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
	print_state(philo, FORK);
	if (first == second)
	{
		pthread_mutex_unlock(first);
		return ;
	}
	pthread_mutex_lock(second);
	print_state(philo, FORK);
}

void	eat_meal(t_philo *philo)
{
	long long	start;

	/* 🔹 Update last_meal BEFORE printing eating */
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);

	print_state(philo, EAT);

	start = get_time_ms();
	while (!simulation_stopped(philo->data)
		&& get_time_ms() - start < philo->data->time_eat)
		usleep(500);

	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}



void	*philo_routine(void *ar)
{
	t_philo	*philo;

	philo = (t_philo *)ar;
	if (philo->left_fork == philo->right_fork)
	{
		print_state(philo, THINK);
		handle_single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep_ms(philo->data->time_eat / 2, philo->data);
	while (!simulation_stopped(philo->data))
	{
		print_state(philo, THINK);
		take_forks(philo);
		eat_meal(philo);
		print_state(philo, SLEEP);
		usleep_ms(philo->data->time_sleep, philo->data);
	}
	return (NULL);
}
