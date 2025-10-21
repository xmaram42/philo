#include "philo.h"

int usleep_ms(unsigned long long time)
{
    unsigned long long start;

    start = get_time_ms();
    while (get_time_ms() - start < time)
        usleep(500);
    return (0);
}