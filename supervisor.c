/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:40:23 by flplace           #+#    #+#             */
/*   Updated: 2023/01/31 22:04:03 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ending_threads(int nphilo, t_ph *ph)
{
	int	i;

	i = 0;
	while (i != nphilo)
	{
		if (pthread_join((ph + i)->thread, NULL) == 1)
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
	tmp = (ph + i)->last_meal;
	pthread_mutex_unlock(&(ph + i)->last_meal_m);
	return (tmp);
}

void	print_supervisor(t_ph *ph, int id, char *str, int flag)
{
	pthread_mutex_lock(&(ph)->rules->print_m);
	printf("%ld ", timestamp(ph));
	if (flag == 0)
		printf("%d ", id);
	printf("%s\n", str);
	pthread_mutex_unlock(&(ph)->rules->print_m);
	return ;
}

void	ending_control(int nphilo, t_ph *ph)
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
				print_supervisor((ph + i), (ph + i)->ph_id,
					"Everyone is full, time to dance!", 1);
			else
				print_supervisor((ph + i), (ph + i)->ph_id, "died", 0);
			(ph + i)->rules->end = 3;
			pthread_mutex_unlock(&(ph + i)->rules->end_m);
			break ;
		}
		if (++i >= nphilo)
			usleep(2000);
		if (i >= nphilo)
			i = 0;
	}
	ending_threads(nphilo, ph);
	return ;
}
