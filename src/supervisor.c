/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:40:23 by flplace           #+#    #+#             */
/*   Updated: 2023/01/28 20:48:51 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	ending_threads(int nphilo, pthread_t **threads)
{
	int	i;

	i = 0;
	while (i != nphilo)
	{
		if (pthread_join(*threads[i], NULL) == 1)
			printf("le thread %d s'est termine de facon inattendue.", (i + 1));
		i++;
	}
	return ;
}

int	hunger_reader(t_ph *ph)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ph->rules->hungry_ppl_m);
	i = ph->rules->hungry_ppl;
	pthread_mutex_unlock(&ph->rules->hungry_ppl_m);
	return (i);
}

int	lastmeal_reader(t_ph *ph, int i)
{
	int	tmp;

	tmp = 0;
	pthread_mutex_lock(&(ph + i)->last_meal_m);
	tmp = ph->last_meal;
	pthread_mutex_unlock(&(ph + i)->last_meal_m);
	return (tmp);
}

void	ending_control(int nphilo, pthread_t **threads, t_ph *ph)
{
	int	i;

	i = 0;
	while (1 && nphilo != 1)
	{
		if (((timestamp(ph + i) - lastmeal_reader(ph, i))
				> (ph + i)->rules->t_death) || hunger_reader(ph) == 0)
		{
			pthread_mutex_lock(&(ph + i)->rules->end_m);
			if (hunger_reader(ph + i) == 0)
				printf("%ld Everyone is full, time to dance!\n",
					timestamp(ph + 1));
			else
				printf("%ld %d died\n", timestamp(ph + i), (ph + i)->ph_id);
			(ph + i)->rules->end = 3;
			pthread_mutex_unlock(&(ph + i)->rules->end_m);
			break ;
		}
		if (++i >= nphilo)
			usleep(2000);
		if (++i >= nphilo)
			i = 0;
	}
	ending_threads(nphilo, threads);
	return ;
}
