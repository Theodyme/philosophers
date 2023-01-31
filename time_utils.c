/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:34 by flplace           #+#    #+#             */
/*   Updated: 2023/01/31 20:42:14 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	unix_timestamp(void)
{
	long int		ms;
	struct timeval	timestruct;

	if (gettimeofday(&timestruct, NULL) == -1)
	{
		printf("Couldn't get time of day.\n");
		return (-1);
	}
	ms = (timestruct.tv_sec * 1000) + (timestruct.tv_usec / 1000);
	return (ms);
}

long int	timestamp(t_ph *ph)
{
	long int	tmp;

	pthread_mutex_lock(&ph->rules->start_m);
	tmp = unix_timestamp() - ph->rules->start;
	pthread_mutex_unlock(&ph->rules->start_m);
	return (tmp);
}

void	nwait(long int time, t_ph *ph)
{
	long int	dest;
	int			i;

	i = 0;
	(void)ph;
	dest = unix_timestamp() + time;
	while (unix_timestamp() < dest)
	{
		if (i % 500)
			if (ending_c(ph, 0) == 1)
				return ;
		usleep(500);
	}
	return ;
}

int	nwait_f(long int time, t_ph *ph)
{
	long int	dest;
	int			i;

	i = 0;
	(void)ph;
	dest = unix_timestamp() + time;
	while (unix_timestamp() < dest)
	{
		if (i % 500)
			if (ending_c(ph, 0) == 1)
				return (1);
		usleep(500);
	}
	return (0);
}

void	forkprinter(t_ph *ph)
{
	pthread_mutex_lock(&ph->rules->end_m);
	if (ph->rules->end == 0)
	{
		pthread_mutex_lock(&ph->rules->print_m);
		printf("%ld %d has taken a fork\n", timestamp(ph), ph->ph_id);
		pthread_mutex_unlock(&ph->rules->print_m);
	}
	pthread_mutex_unlock(&ph->rules->end_m);
	return ;
}
