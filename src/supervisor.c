/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:40:23 by flplace           #+#    #+#             */
/*   Updated: 2023/01/28 15:51:28 by flplace          ###   ########.fr       */
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

void	ending_control(int nphilo, pthread_t **threads, t_ph *ph)
{
	int	i;

	i = 0;
	while (1)
	{
		if (((timestamp(ph + i) - (ph + i)->last_meal)
				> (ph + i)->rules->t_death))
		{
			pthread_mutex_lock(&(ph + i)->rules->end_m);
			if ((ph + i)->rules->hungry_ppl != 0)
				printf("%ld %d died(outside)\n",
					timestamp(ph + i), (ph + i)->ph_id);
			(ph + i)->rules->end = 3;
			pthread_mutex_unlock(&(ph + i)->rules->end_m);
			break ;
		}
		i++;
		if (i >= nphilo)
		{
			usleep(2000);
			i = 0;
		}
	}
	ending_threads(nphilo, threads);
	return ;
}
