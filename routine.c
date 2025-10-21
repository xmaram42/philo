#include "philo.h"
static void set_stop(t_data *data)
{
    pthread_mutex_lock(&data->state_lock);
    data->stop = 1;
    pthread_mutex_unlock(&data->state_lock);
}

static int simulation_stopped(t_data *data)
{
    int stop;

    pthread_mutex_lock(&data->state_lock);
    stop = data->stop;
    pthread_mutex_unlock(&data->state_lock);
    return (stop);
}
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
    int         finished;
    long long   last_meal;
    int         meals;

    data = (t_data *)ar;
    while (1)
    {
        i = 0;
        finished = 0;
        while (i < (int)data->n)
        {
            pthread_mutex_lock(&data->philos[i].meal_lock);
            last_meal = data->philos[i].last_meal;
            meals = data->philos[i].meals;
            pthread_mutex_unlock(&data->philos[i].meal_lock);
            if (get_time_ms() - last_meal > data->die)
            {
                set_stop(data);
                print_state(&data->philos[i], "died 💀");
                return (NULL);
            }
            if (data->must_eat > 0 && meals >= data->must_eat)
                finished++;
            i++;
        }
        if (data->must_eat > 0 && finished == (int)data->n)
        {
            set_stop(data);
            return (NULL);
        }
        if (simulation_stopped(data))
            return (NULL);
        usleep(500);
    }
    return (NULL);
}

void print_state(t_philo *philo, char *msg)
{
    long long   timestamp;
    int         stop;

    pthread_mutex_lock(&philo->data->print_lock);
    timestamp = get_time_ms() - philo->data->start_time;
    pthread_mutex_lock(&philo->data->state_lock);
    stop = philo->data->stop;
    pthread_mutex_unlock(&philo->data->state_lock);
    if (!stop || ft_strcmp(msg, "died 💀") == 0)
        printf("%lld %d %s\n", timestamp, philo->id, msg);
    pthread_mutex_unlock(&philo->data->print_lock);
}
static void take_forks(t_philo *philo)
{
    pthread_mutex_t   *first;
    pthread_mutex_t   *second;

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

static void eat_meal(t_philo *philo)
{
    print_state(philo, "is eating 🍝");
    pthread_mutex_lock(&philo->meal_lock);
    philo->last_meal = get_time_ms();
    philo->meals++;
    pthread_mutex_unlock(&philo->meal_lock);
    usleep_ms(philo->data->eat);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
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
    if (philo->id % 2 == 0)
        usleep_ms(philo->data->eat / 2);
    while (!simulation_stopped(philo->data))
    {
        print_state(philo, "is thinking 💭");
        take_forks(philo);
        eat_meal(philo);
        print_state(philo, "is sleeping 😴");
        usleep_ms(philo->data->sleep);
        if (philo->data->n % 2 == 0)
            usleep_ms(1);
    }
    return (NULL);
}
