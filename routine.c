#include "philo.h"

static void handle_single_philo(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_state(philo, "has taken a fork 🍴");
    usleep_ms(philo->data->die);
    pthread_mutex_unlock(philo->left_fork);
}

void    *monitor(void *ar)
{
    t_data  *data;
    int     i;

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
        if (data->stop)
            return (NULL);
        usleep(500);
    }
    return (NULL);
}

void print_state(t_philo *philo, char *msg)
{
    long long   timestamp;

    pthread_mutex_lock(&philo->data->print_lock);
    timestamp = get_time_ms() - philo->data->start_time;
    if (!philo->data->stop || ft_strcmp(msg, "died 💀") == 0)
        printf("%lld %d %s\n", timestamp, philo->id, msg);
    pthread_mutex_unlock(&philo->data->print_lock);
}

void    *philo_routine(void *ar)
{
    t_philo *philo;

    philo = (t_philo *)ar;
    if (philo->left_fork == philo->right_fork)
    {
        print_state(philo, "is thinking 💭");
        handle_single_philo(philo);
        return (NULL);
    }
    while (1)
    {
        if (philo->data->stop)
            break ;
        print_state(philo, "is thinking 💭");
        pthread_mutex_lock(philo->left_fork);
        print_state(philo, "has taken a fork 🍴");
        pthread_mutex_lock(philo->right_fork);
        print_state(philo, "has taken a fork 🍴");
        print_state(philo, "is eating 🍝");
        philo->last_meal = get_time_ms();
        usleep_ms(philo->data->eat);
        philo->meals++;
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        print_state(philo, "is sleeping 😴");
        usleep_ms(philo->data->sleep);
    }
    return (NULL);
}