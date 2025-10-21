#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
# include <sys/time.h> 
# include <stdlib.h>  
typedef struct s_philo t_philo;

typedef struct s_data
{
    unsigned int    n;          // number of philosophers
    unsigned int    die;        // time to die (ms)
    unsigned int    eat;        // time to eat (ms)
    unsigned int    sleep;      // time to sleep (ms)
    int             must_eat;   // optional
    int             stop;
    long long       start_time; // simulation start time
    pthread_mutex_t *forks;     // array of fork mutexes
    pthread_mutex_t print_lock; // mutex for synchronized printing
    t_philo         *philos;    // array of philosophers
}   t_data;

struct s_philo
{
    int             id;    //Philosopher ID
    int             meals; // how many times he has eaten 
    long long       last_meal; // time (ms) when last meal happend
    pthread_t       thread; // the thread running this philosopher
    pthread_mutex_t *left_fork; // pointer to left fork mutex
    pthread_mutex_t *right_fork; //  pointer to right fork mutex
    t_data          *data; // Pointer to the shared data struct
};

int	ft_atoi(const char *str);
int init_data(t_data *data);
int	usleep_ms(unsigned long long time);
unsigned long long	get_time_ms(void);
void	*philo_routine(void *ar);
void	*monitor(void *ar);
void cleanup_data(t_data *data);
void print_state (t_philo *philo, char *msg);
int	ft_strcmp(char *s1, char *s2);
#endif