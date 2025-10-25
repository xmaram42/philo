/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:44:21 by maram             #+#    #+#             */
/*   Updated: 2025/10/25 18:35:58 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

# define EAT   0
# define SLEEP 1
# define THINK 2
# define FORK  3
# define DEAD  4

typedef struct s_philo	t_philo;

typedef struct s_data
{
	unsigned int		num_philo;      // Number of philosophers
	unsigned long long	time_die;       // Time to die (ms)
	unsigned long long	time_eat;       // Time to eat (ms)
	unsigned long long	time_sleep;     // Time to sleep (ms)
	long long			start_time;     // Simulation start time (ms)
	long long			num_eat;        // Meals required (or -1)
	bool				dead;           // True if any philosopher died
	pthread_t			monitor;        // Monitor thread
	pthread_mutex_t		*fork_mutex;    // Array of fork mutexes
	pthread_mutex_t		print_mutex;    // Protects print output
	pthread_mutex_t		eat_mutex;      // Protects meal count updates
	pthread_mutex_t		die_mutex;      // Protects death flag
	t_philo				*philos;        // Array of philosopher structs
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
void	set_stop(t_data *data);
void				*philo_routine(void *ar);
void	print_state(t_philo *philo, int state);
void				*monitor(void *ar);
int	usleep_ms(unsigned long long time, t_data *data);
int					ft_strcmp(char *s1, char *s2);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
bool	simulation_stopped(t_data *data);
#endif