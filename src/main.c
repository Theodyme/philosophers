/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:18 by flplace           #+#    #+#             */
/*   Updated: 2023/01/26 18:45:20 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	meals_c(t_ph *ph)
{
	if (ph->meals == ph->rules->cap)
	{
		pthread_mutex_lock(&ph->rules->hungry_ppl_m);
		ph->rules->hungry_ppl--;
		pthread_mutex_unlock(&ph->rules->hungry_ppl_m);
		return (1);
	}
	return (0);
}

void	routine(t_ph *ph)
{
	pthread_mutex_lock(&ph->rules->end_m);
	while (ph->rules->end == 0)
	{
		pthread_mutex_unlock(&ph->rules->end_m);
		if (ending_c(ph, 0) == 1)
			return ;
		lock_f(ph);
		if (a_eat(ph) == 1)
			return ;
		unlock_f(ph);
		a_sleep(ph);
		a_think(ph);
		if (ending_c(ph, 0) == 1)
			return ;
		pthread_mutex_lock(&ph->rules->end_m);
	}
	pthread_mutex_unlock(&ph->rules->end_m);
	return ;
}

void	*philosopher(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	if (&(ph->fork->fork_m) == &(ph->fork->next->fork_m))
	{
		nwait(ph->rules->t_death, ph);
		printf("%ld %d died\n", timestamp(ph), ph->ph_id);
		return (NULL);
	}
	pthread_mutex_lock(&ph->rules->start_m);
	ph->rules->start = unix_timestamp();
	pthread_mutex_unlock(&ph->rules->start_m);
	ph->last_meal = timestamp(ph);
	if (ph->ph_id % 2 != 0)
		nwait(ph->rules->t_eat, ph);
	routine(ph);
	return (NULL);
}

void	ending_threads(int nphilo, pthread_t **threads, t_ph *ph)
{
	int	i;

	i = 0;
	// (void)ph;
	while (1)
	{
		printf("thread n %d\n", i);
		if (((timestamp(ph + i) - (ph + i)->last_meal) > (ph + i)->rules->t_death))
		{
			printf("%ld %d died\n", timestamp(ph + i), (ph + i)->ph_id);
			break;
		}
		printf("after if in ending_threads %d\n", i);
		i++;
		if (i > nphilo)
		{
			usleep(2000);
			i = 0;
		}
	}
	i = 0;
	while (i != nphilo)
	{
		if (pthread_join(*threads[i], NULL) == 1)
			printf("le thread %d s'est termine de facon inattendue.", (i + 1));
		i++;
	}
	return ;
}

int	main(int ac, char **av)
{
	pthread_t	**threads;
	t_list		*list;
	t_rules		*rules;
	t_ph		*ph;
	int			i;

	i = -1;
	if (argchecker(ac, av) == 0)
		return (1);
	list = NULL;
	struct_init(&ph, &threads, &rules, av);
	ph_init(ph, &list, ft_atoi(av[1]), rules);
	pthread_mutex_lock(&rules->start_m);
	while (++i != ft_atoi(av[1]))
		pthread_create(threads[i], NULL, philosopher, (ph + i));
	pthread_mutex_unlock(&rules->start_m);
	ending_threads(ft_atoi(av[1]), threads, ph);
	if (rules->hungry_ppl == 0)
		printf("%ld Everyone is full, now it's time to dance!\n",
			timestamp(ph + 1));
	destroy_list(list);
	s_cleaner(&ph, &threads, &rules, av);
	return (0);
}
