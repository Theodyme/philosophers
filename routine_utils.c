/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:40 by flplace           #+#    #+#             */
/*   Updated: 2023/01/31 21:42:33 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	lock_f(t_ph *ph)
{
	if (ph->ph_id % 2 == 0)
	{
		if (pthread_mutex_lock(&ph->fork->fork_m) == 0)
		{
			forkprinter(ph);
			pthread_mutex_lock(&ph->fork->next->fork_m);
			forkprinter(ph);
		}
	}
	else
	{
		if (pthread_mutex_lock(&ph->fork->next->fork_m) == 0)
		{
			forkprinter(ph);
			pthread_mutex_lock(&ph->fork->fork_m);
			forkprinter(ph);
		}
	}
	return ;
}

void	unlock_f(t_ph *ph)
{
	if (ph->ph_id % 2 == 0)
	{
		pthread_mutex_unlock(&ph->fork->fork_m);
		pthread_mutex_unlock(&ph->fork->next->fork_m);
	}
	else
	{
		pthread_mutex_unlock(&ph->fork->next->fork_m);
		pthread_mutex_unlock(&ph->fork->fork_m);
	}
	return ;
}

int	a_sleep(t_ph *ph)
{
	int	diff;

	diff = timestamp(ph) - ph->last_meal;
	pthread_mutex_lock(&ph->rules->end_m);
	if (ph->rules->end == 0)
	{
		pthread_mutex_lock(&ph->rules->print_m);
		printf("%ld %d is sleeping\n", timestamp(ph), ph->ph_id);
		pthread_mutex_unlock(&ph->rules->print_m);
	}
	pthread_mutex_unlock(&ph->rules->end_m);
	if ((diff + ph->rules->t_sleep) >= ph->rules->t_death)
	{
		nwait(ph->rules->t_death - (timestamp(ph) - ph->last_meal), ph);
		pthread_mutex_lock(&ph->rules->end_m);
		if (ph->rules->end != 3)
			ph->rules->end = 1;
		pthread_mutex_unlock(&ph->rules->end_m);
	}
	else
		nwait(ph->rules->t_sleep, ph);
	return (0);
}

int	a_eat(t_ph *ph)
{
	if (ending_c(ph, 1) == 1)
		return (1);
	pthread_mutex_lock(&ph->rules->print_m);
	printf("%ld %d is eating\n", timestamp(ph), ph->ph_id);
	pthread_mutex_unlock(&ph->rules->print_m);
	pthread_mutex_lock(&ph->last_meal_m);
	ph->last_meal = timestamp(ph);
	pthread_mutex_unlock(&ph->last_meal_m);
	ph->meals++;
	meals_c(ph);
	if (nwait_f(ph->rules->t_eat, ph) == 1)
		return (1);
	return (0);
}

int	a_think(t_ph *ph)
{
	pthread_mutex_lock(&ph->rules->end_m);
	if (ph->rules->end == 0)
	{
		pthread_mutex_lock(&ph->rules->print_m);
		printf("%ld %d is thinking\n", timestamp(ph), ph->ph_id);
		pthread_mutex_unlock(&ph->rules->print_m);
	}
	pthread_mutex_unlock(&ph->rules->end_m);
	nwait((ph->rules->t_eat - ph->rules->t_sleep + 1), ph);
	return (0);
}
