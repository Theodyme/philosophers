 #include "philosophers.h"

 long int    whattimeisit(void)
{
    long int    ms;
    struct timeval  timestruct;

    if (gettimeofday(&timestruct, NULL) == -1)
    {
        printf("Couldn't get time of day.\n");
        return (-1);
    }
    ms = (timestruct.tv_sec * 1000) + (timestruct.tv_usec / 1000);
    return (ms); 
}

long int    time_monitor(t_ph *ph)
{
    return (whattimeisit() - ph->rules->start);
}