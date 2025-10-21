#include "philo.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int ft_atoi(const char *str)
{
    int                 i;
    int                 sg;
    unsigned long int   rus;

    i = 0;
    sg = 1;
    rus = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sg *= -1;
        i++;
    }
    while (ft_isdigit(str[i]))
    {
        rus = rus * 10 + (str[i] - '0');
        i++;
    }
    if (sg == -1 && rus >= 9223372036854775808ULL)
        return (0);
    if (rus >= __LONG_LONG_MAX__ && sg != -1)
        return (-1);
    return (sg * (int)rus);
}
