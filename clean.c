#include "philo.h"

void cleanup_data(t_data *data)
{
    int i;

    i = 0;
    while(i < (int)data->n)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print_lock);
    free(data->forks);
	free(data->philos);
}