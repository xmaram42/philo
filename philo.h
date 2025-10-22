/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:44:21 by maram             #+#    #+#             */
/*   Updated: 2025/10/22 15:01:01 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	unsigned int	n;
	unsigned int	die;
	unsigned int	eat;
	unsigned int	sleep;
	int				must_eat;
	int				stop;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	t_philo			*philos;
}	t_data;

struct s_philo
{
	int				id;
	int				meals;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	pthread_mutex_t	meal_lock;
};
int					validate_args(int ac, char **av);
int					parse_args(int ac, char **av, t_data *data);
int					init_data(t_data *d);
void				cleanup_data(t_data *data);
unsigned long long	get_time_ms(void);
int					simulation_stopped(t_data *data);
void				*philo_routine(void *ar);
void				print_state(t_philo *philo, char *msg);
void				*monitor(void *ar);
int					usleep_ms(unsigned long long time);
int					ft_strcmp(char *s1, char *s2);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
#endif