
#include "philo.h"

unsigned long long	get_time_ms(void)
{
    unsigned long long mlscs;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    mlscs = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (mlscs);
}

int init_data(t_data *data)
{
    int i;

    i = 0;
    data->start_time = get_time_ms();
    data->forks = malloc(sizeof(pthread_mutex_t) * data->n);
    if(!data->forks)
        return( 1);
    while (i < (int)data->n)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&data->print_lock, NULL);
    data->philos = malloc(sizeof(t_philo) *data->n);
    if(!data->philos)
        return(1);
    i = 0;
    while (i < (int)data->n)
    {
        data->philos[i].id = i + 1;
        data->philos[i].meals = 0;
        data->philos[i].last_meal = data->start_time;
        data->philos[i].data = data;
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->n];
        i++;
    }
    return(0);
}