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
	usleep_ms(philo->data->die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}

void	print_state(t_philo *philo, char *msg)
{
	long long	timestamp;
	int		stop;

	pthread_mutex_lock(&philo->data->print_lock);
	timestamp = get_time_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->state_lock);
	stop = philo->data->stop;
	pthread_mutex_unlock(&philo->data->state_lock);
	if (!stop || ft_strcmp(msg, "died 💀") == 0)
		printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

static void	set_hold(t_philo *philo, pthread_mutex_t *fork, int status)
{
	if (fork == philo->left_fork)
		philo->holding_left = status;
	else
		philo->holding_right = status;
}

static void	choose_order(t_philo *philo, pthread_mutex_t **first,
			pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

static int	lock_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	set_hold(philo, fork, 1);
	print_state(philo, "has taken a fork 🍴");
	if (simulation_stopped(philo->data))
	{
		pthread_mutex_unlock(fork);
		set_hold(philo, fork, 0);
		return (0);
	}
	return (1);
}

static void	release_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_unlock(fork);
	set_hold(philo, fork, 0);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	choose_order(philo, &first, &second);
	if (!lock_fork(philo, first))
		return (0);
	if (first == second)
	{
		release_fork(philo, first);
		return (0);
	}
	if (!lock_fork(philo, second))
	{
		release_fork(philo, first);
		return (0);
	}
	return (1);
}

int	eat_meal(t_philo *philo)
{
	print_state(philo, "is eating 🍝");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);

	usleep_ms(philo->data->eat, philo->data);

	pthread_mutex_unlock(philo->right_fork);
	philo->holding_right = 0;
	pthread_mutex_unlock(philo->left_fork);
	philo->holding_left = 0;
	if (simulation_stopped(philo->data))
		return (0);
	return (1);
}

static void	sync_start(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep_ms(philo->data->eat / 2, philo->data);
	else if (philo->data->n % 2 != 0)
		usleep_ms(philo->data->eat, philo->data);
}

static int	philo_cycle(t_philo *philo)
{
	print_state(philo, "is thinking 💭");
	if (!take_forks(philo))
		return (0);
	if (!eat_meal(philo))
		return (0);
	print_state(philo, "is sleeping 😴");
	usleep_ms(philo->data->sleep, philo->data);
	if (philo->data->n % 2 == 0)
		usleep_ms(1, philo->data);
	return (1);
}

static void	release_remaining(t_philo *philo)
{
	if (philo->holding_right)
	{
		pthread_mutex_unlock(philo->right_fork);
		philo->holding_right = 0;
	}
	if (philo->holding_left && philo->left_fork != philo->right_fork)
	{
		pthread_mutex_unlock(philo->left_fork);
		philo->holding_left = 0;
	}
}

void	*philo_routine(void *ar)
{
	t_philo	*philo;

	philo = (t_philo *)ar;
	if (philo->left_fork == philo->right_fork)
	{
		print_state(philo, "is thinking 💭");
		handle_single_philo(philo);
		return (NULL);
	}
	sync_start(philo);
	while (!simulation_stopped(philo->data))
	{
		if (!philo_cycle(philo))
			break ;
	}
	release_remaining(philo);
	return (NULL);
}
