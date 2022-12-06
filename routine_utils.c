#include "philosophers.h"

void    lock_f(t_ph *ph)
{
    if (ph->ph_id % 2 != 0)
    {
        if (pthread_mutex_lock(&ph->fork->fork_m) == 0)
        {
			if (end_flag_c(ph) == 0)
	            printf("%ld %d has taken a fork\n", time_monitor(ph), ph->ph_id);
            pthread_mutex_lock(&ph->fork->next->fork_m);
			if (end_flag_c(ph) == 0)
				printf("%ld %d has taken a fork\n", time_monitor(ph), ph->ph_id);
        }
    }
    else
    {
        if (pthread_mutex_lock(&ph->fork->next->fork_m) == 0)
            pthread_mutex_lock(&ph->fork->fork_m);
    }
    return ;
}

void    unlock_f(t_ph *ph)
{
    if (ph->ph_id % 2 != 0)
    {
        pthread_mutex_unlock(&ph->fork->fork_m);
        pthread_mutex_unlock(&ph->fork->next->fork_m);
    }
    else
    {
		// printf("%d unlocking forks\n", ph->ph_id);
        pthread_mutex_unlock(&ph->fork->next->fork_m);
        pthread_mutex_unlock(&ph->fork->fork_m);
    }
    return ;
}


int     success_c(t_ph *ph, int ulock_f)
{
    pthread_mutex_lock(&ph->rules->success_m);
    if (ph->rules->success == 0)
    {
        pthread_mutex_unlock(&ph->rules->success_m);
        pthread_mutex_lock(&ph->rules->end_m);
        if (ulock_f == 1)
            unlock_f(ph);
        // printf("ending simulation here for %d: time: %ld, end flag: %d, success flag: %d with %d/%d meals.\n", ph->ph_id, (time_monitor(ph) - ph->last_meal), ph->rules->end, success_c(ph), ph->meals, ph->rules->cap);
        if (ph->rules->end != 3)
            ph->rules->end = 3;
        pthread_mutex_unlock(&ph->rules->end_m);
        return (1);
    }
    pthread_mutex_unlock(&ph->rules->success_m);
    return (0);
}

int     a_sleep(t_ph *ph)
{
    int diff;

    diff = time_monitor(ph) - ph->last_meal;
    if (ending_c(ph, 0) == 0)
        printf("%ld %d is sleeping\n", time_monitor(ph), ph->ph_id);
    if ((diff + ph->rules->t_sleep) >= ph->rules->t_death)
    {
        waiting(ph->rules->t_death - (time_monitor(ph) - ph->last_meal), ph);
        pthread_mutex_lock(&ph->rules->end_m);
        ph->rules->end = 1;
        pthread_mutex_unlock(&ph->rules->end_m);
    }
    else
        waiting(ph->rules->t_sleep, ph);
    return 0;
}

int     a_eat(t_ph *ph)
{
		// printf("%d before a_eat check, end flag is %d\n", ph->ph_id, end_flag_c(ph));
        if (ending_c(ph, 1) == 1)
		{
			// printf("%d is leaving the a_eat() with 1\n", ph->ph_id);
			return (1);
		}
        printf("%ld %d is eating\n", time_monitor(ph), ph->ph_id);
		ph->last_meal = time_monitor(ph);
        ph->meals++;
        meals_c(ph);
		if (waiting_f(ph->rules->t_eat, ph) == 1)
			return (1);
		// printf("%ld %d JUST ATE %ld MILLISEC after %ld\n", time_monitor(ph), ph->ph_id, ph->rules->t_eat, ph->last_meal);
        return (0);
}

int     a_think(t_ph *ph)
{
        if (end_flag_c(ph) == 0)
            printf("%ld %d is thinking\n", time_monitor(ph), ph->ph_id);
        if (ph->rules->t_eat > ph->rules->t_sleep)
            waiting((ph->rules->t_eat - ph->rules->t_sleep), ph);
        return 0;
}
