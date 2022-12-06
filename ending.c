#include "philosophers.h"

/*	Return the end flag;
	0:	continues ;
	1:	someone died - the line "X has died" needs to be printed.
	2:
	3:	someone died - the line has already been printed and the philo consulting this should leave.	*/

int		end_flag_c(t_ph *ph)
{
	int flag;

    pthread_mutex_lock(&ph->rules->end_m);
	flag = ph->rules->end;
    pthread_mutex_unlock(&ph->rules->end_m);
	return (flag);
}

/*	Check if someone starved to death. Pass the flag to 1 if it's the first death of the simulation.	*/
void	starve_c(t_ph *ph)
{
	    if (((time_monitor(ph) - ph->last_meal) > ph->rules->t_death))
        {
            pthread_mutex_lock(&ph->rules->end_m);
            // printf("ending simulation here for %d: time: %ld, end flag: %d, success flag: %d with %d/%d meals.\n", ph->ph_id, (time_monitor(ph) - ph->last_meal), ph->rules->end, success_c(ph, 0), ph->meals, ph->rules->cap);
            if (ph->rules->end != 3)
                ph->rules->end = 1;
            pthread_mutex_unlock(&ph->rules->end_m);
        }
		return ;
}

/*	Return 1 if the simulation ends. return 0 if it continues.
	First check if someone starved to death for the first time.
	Second, check the flag.	*/
int    ending_c(t_ph *ph, int ulock_f)
{
        if (success_c(ph, ulock_f) == 1)
            return (1);
		starve_c(ph);
		// printf("%d current ending_c, end flag is %d\n", ph->ph_id, end_flag_c(ph));
        pthread_mutex_lock(&ph->rules->end_m);
        if (ph->rules->end != 0)
        {
            if (ulock_f == 1)
			{
				// printf("%d unlocking forks\n", ph->ph_id);
                unlock_f(ph);
			}
			if (ph->rules->end == 1)
            {
                ph->rules->end = 3;
                printf("%ld %d died\n", time_monitor(ph), ph->ph_id);
            }
            pthread_mutex_unlock(&ph->rules->end_m);
            return (1);
        }
        pthread_mutex_unlock(&ph->rules->end_m);
        return (0);
}
