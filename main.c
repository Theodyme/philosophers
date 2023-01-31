/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:18 by flplace           #+#    #+#             */
/*   Updated: 2023/01/31 21:51:41 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		lock_f(ph);
		if (a_eat(ph) == 1)
			return ;
		unlock_f(ph);
		a_sleep(ph);
		a_think(ph);
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
		pthread_mutex_lock(&ph->rules->print_m);
		printf("%ld 1 died\n", timestamp(ph));
		pthread_mutex_unlock(&ph->rules->print_m);
		return (NULL);
	}
	pthread_mutex_lock(&ph->rules->start_m);
	ph->rules->start = unix_timestamp();
	pthread_mutex_unlock(&ph->rules->start_m);
	pthread_mutex_lock(&ph->last_meal_m);
	ph->last_meal = timestamp(ph);
	pthread_mutex_unlock(&ph->last_meal_m);
	if (ph->ph_id % 2 != 0)
		nwait(ph->rules->t_eat, ph);
	routine(ph);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_list		*list;
	t_rules		*rules;
	t_ph		*ph;
	int			i;

	if (argchecker(ac, av) == 0)
		return (1);
	list = NULL;
	if (struct_init(&ph, &rules, av) == 1)
		return (0);
	i = -1;
	ph_init(ph, &list, ft_atoi(av[1]), rules);
	pthread_mutex_lock(&rules->start_m);
	while (++i != ft_atoi(av[1]))
		pthread_create(&(ph + i)->thread, NULL, philosopher, (ph + i));
	pthread_mutex_unlock(&rules->start_m);
	ending_control(ft_atoi(av[1]), ph);
	destroy_list(list);
	s_cleaner(&ph, &rules);
	return (0);
}
