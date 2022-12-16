/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ending.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:03 by flplace           #+#    #+#             */
/*   Updated: 2022/12/16 19:15:13 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	end_flag_c(t_ph *ph)
{
	int	flag;

	pthread_mutex_lock(&ph->rules->end_m);
	flag = ph->rules->end;
	pthread_mutex_unlock(&ph->rules->end_m);
	return (flag);
}

void	starve_c(t_ph *ph)
{
	if (((timestamp(ph) - ph->last_meal) > ph->rules->t_death))
	{
		pthread_mutex_lock(&ph->rules->end_m);
		if (ph->rules->end != 3)
			ph->rules->end = 1;
		pthread_mutex_unlock(&ph->rules->end_m);
	}
	return ;
}

int	hunger_check(t_ph *ph, int ulock_f)
{
	pthread_mutex_lock(&ph->rules->hungry_ppl_m);
	if (ph->rules->hungry_ppl == 0)
	{
		pthread_mutex_unlock(&ph->rules->hungry_ppl_m);
		pthread_mutex_lock(&ph->rules->end_m);
		if (ulock_f == 1)
			unlock_f(ph);
		if (ph->rules->end != 3)
			ph->rules->end = 3;
		pthread_mutex_unlock(&ph->rules->end_m);
		return (0);
	}
	pthread_mutex_unlock(&ph->rules->hungry_ppl_m);
	return (ulock_f);
}

int	ending_c(t_ph *ph, int ulock_f)
{
	ulock_f = hunger_check(ph, ulock_f);
	starve_c(ph);
	pthread_mutex_lock(&ph->rules->end_m);
	if (ph->rules->end != 0)
	{
		if (ulock_f == 1)
			unlock_f(ph);
		if (ph->rules->end == 1)
		{
			ph->rules->end = 3;
			printf("%ld %d died\n", timestamp(ph), ph->ph_id);
		}
		pthread_mutex_unlock(&ph->rules->end_m);
		return (1);
	}
	pthread_mutex_unlock(&ph->rules->end_m);
	return (0);
}
