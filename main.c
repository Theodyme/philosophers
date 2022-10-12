#include "philosophers.h"

int     meals_c(t_ph *ph)
{
    if (ph->meals == ph->rules->cap)
        {
            // printf("(%d): J'ai plus faim ! (%d repas)\n", ph->ph_id, meals);
            pthread_mutex_lock(&ph->rules->success_m);
            ph->rules->success--;
            // printf("success flag: %d\n", ph->rules->success);
            pthread_mutex_unlock(&ph->rules->success_m);
            return (1);
        }
    return (0);
}

void    *philosopher(void *arg)
{
    t_ph *ph;

    ph = (t_ph *)arg;
    if (&(ph->fork->fork_m) == &(ph->fork->next->fork_m))
    {
        waiting(ph->rules->t_death, ph);
        printf("%ld %d died\n", time_monitor(ph), ph->ph_id);
        pthread_exit(EXIT_SUCCESS);
    }
    pthread_mutex_lock(&ph->rules->end_m);
    pthread_mutex_unlock(&ph->rules->end_m);
    if (ph->ph_id == 1)
        ph->rules->start = whattimeisit();
    // while (whattimeisit() < ph->rules->start)
    // {
    //     usleep(10);
    // }
    // sleep(1);
    ph->last_meal = 0;
    // printf("%d --> %ld\n", ph->ph_id, time_monitor(ph));
    if (ph->ph_id % 2 != 0)
        waiting(ph->rules->t_eat, ph);
    pthread_mutex_lock(&ph->rules->end_m);
    while (ph->rules->end == 0)
    {
        pthread_mutex_unlock(&ph->rules->end_m);
        ending_c(ph);
        lock_f(ph);
        ending_c_f(ph);
        activity(ph, ph->rules->t_eat, 3);
        unlock_f(ph);
        activity(ph, ph->rules->t_sleep, 1);
        activity(ph, 0, 2);
        ending_c(ph);
        pthread_mutex_lock(&ph->rules->end_m);
    }
    pthread_mutex_unlock(&ph->rules->end_m);
    pthread_exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    pthread_t **threads;
    t_list *list = NULL;
    t_rules *rules = NULL;
    t_ph *ph = NULL;
    int i;
    int err;

    i = 0;
    if (ac < 5 || ac > 6)
        return (0);
    err = 0;
    struct_init(&ph, &threads, &rules, av);
    list_init(ph, &list, ft_atoi(av[1]));
    rules->start = whattimeisit();
    pthread_mutex_lock(&rules->end_m);
    while(i != ft_atoi(av[1]))
    {
        (ph + i)->meals = 0;
        (ph + i)->ph_id = i + 1;
        (ph + i)->rules = rules;
        pthread_create(threads[i], NULL, philosopher, (ph + i));
        i++;
    }
    pthread_mutex_unlock(&rules->end_m);
    i = 0;
    while(i != ft_atoi(av[1]))
    {
        err = pthread_join(*threads[i], NULL);
        if (err == 1)
            printf("le thread %d n'a pas voulu mourir correctement. :'(", (i + 1));
        i++;
    }
    destroy_list(list);
    return (0);
}
