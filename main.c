#include "philo.h"

int validate_args(int ac, char **av)
{
    int i;
    int j;

    if (ac != 5 && ac != 6)
    {
        printf("Error: wrong number of arguments\n");
        return (1);
    }
    i = 1;
    while (i < ac)
    {
        j = 0;
        while (av[i][j])
        {
            if (av[i][j] < '0' || av[i][j] > '9')
            {
                printf("Error: invalid character in argument %d\n", i);
                return (1);
            }
            j++;
        }
        i++;
    }
    return (0);
}

int parse_args(int ac, char **av, t_data *data)
{
    data->n = ft_atoi(av[1]);
    data->die = ft_atoi(av[2]);
    data->eat = ft_atoi(av[3]);
    data->sleep = ft_atoi(av[4]);
    if (ac == 6)
        data->must_eat = ft_atoi(av[5]);
    else
        data->must_eat = -1;
    if (data->n <= 0 || data->die <= 0 || data->eat <= 0 || data->sleep <= 0)
    {
        printf("Error: arguments must be greater than 0\n");
        return (1);
    }
    return (0);
}

int main(int ac, char **av)
{
    t_data      data;
    pthread_t   monitor_thread;
    int         i;

    if (validate_args(ac, av))
        return (1);
    if (parse_args(ac, av, &data))
        return (1);
    if (init_data(&data))
        return (printf("Error initializing data\n"), 1);
		data.start_time = get_time_ms();
    i = 0;
    while (i < (int)data.n)
    {
        data.philos[i].last_meal = data.start_time;
        i++;
    }
    i = 0;
    while (i < (int)data.n)
    {
        pthread_create(&data.philos[i].thread, NULL, philo_routine, &data.philos[i]);
        i++;
    }
    pthread_create(&monitor_thread, NULL, monitor, &data);
    pthread_join(monitor_thread, NULL);
    i = 0;
    while (i < (int)data.n)
    {
        pthread_join(data.philos[i].thread, NULL);
        i++;
    }
    cleanup_data(&data);
    return (0);
}