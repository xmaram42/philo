
#include "philo.h"

void	*monitor(void *ar)
{
	t_data	*data;
	int		i;

	data = (t_data *)ar;
	while (1)
	{
		i = 0;
		while (i < (int)data->n)
		{
			if (get_time_ms() - data->philos[i].last_meal > data->die)
			{
                data->stop = 1;
				print_state(&data->philos[i], "died 💀");
				return (NULL);
			}
			i++;
		}
		usleep(500); // small delay to prevent CPU overload
	}
	return (NULL);
}

void print_state(t_philo *philo, char *msg)
{
    long long tmstmp = get_time_ms() - philo->data->start_time;

    pthread_mutex_lock(&philo->data->print_lock);
if (!philo->data->stop || ft_strcmp(msg, "died 💀") == 0)
    printf("%lld %d %s\n", tmstmp, philo->id, msg);

    pthread_mutex_unlock(&philo->data->print_lock);
}

void	*philo_routine(void *ar)
{
	t_philo	*philo;

    philo = (t_philo *)ar;

    while(1)
    {
        if (philo->data->stop)
        break;
        print_state(philo, "is thinking 💭");
        pthread_mutex_lock(philo->left_fork);
        print_state(philo, "has taken a fork 🍴");
        pthread_mutex_lock(philo->right_fork);
        print_state(philo, "has taken a fork 🍴");
        print_state(philo, "is eating 🍝");
        philo->last_meal = get_time_ms();
        usleep_ms(philo->data->eat);
        philo->meals++;
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        print_state(philo, "is sleeping 😴");
        usleep(philo->data->sleep);
    }
    return(NULL);
}
