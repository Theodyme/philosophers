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

void    waiting(long int time, t_ph *ph)
{
    long int dest;

    dest = whattimeisit() + time;
    while (whattimeisit() < dest)
    {
        if (ending_c(ph, 0) == 1)
			return ;
        // ending_c(ph, 0);
        usleep(100);
    }
    return ;
}

int    waiting_f(long int time, t_ph *ph)
{
    long int dest;

    dest = whattimeisit() + time;
	// printf("checking waiting_f...\n");
    while (whattimeisit() < dest)
    {
        if (ending_c(ph, 1) == 1)
			return (1);
        usleep(100);
    }
    return (0);
}
