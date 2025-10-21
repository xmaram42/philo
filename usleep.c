
#include "philo.h"

int	usleep_ms(unsigned long long time)
{
    unsigned long long start;

    start = get_time_ms(); // to know the time now
    while (get_time_ms() - start < time)
        usleep(500); // we can sleep mli of ms
    return (0);
}
