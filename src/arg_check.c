/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 15:45:09 by flplace           #+#    #+#             */
/*   Updated: 2022/12/31 07:44:58 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	argchecker(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
		return (0);
	if (av[5] && ft_atoi(av[5]) == 0)
	{
		printf("0 No one wants to eat. Everybody wants to dance!\n");
		return (0);
	}
	while (av[i])
	{
		if (ft_strisdigit(av[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_strisdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	cap_arg_check(t_ph **ph, pthread_t ***threads,
				t_rules **rules, char **av)
{
	if (ft_atoi(av[1]) == 0)
	{
		s_cleaner(ph, threads, rules, av);
		return (1);
	}
	if (av[5] && ft_atoi(av[5]) == 0)
	{
		printf("%ld No one wants to eat. Everybody wants to dance!\n",
			timestamp(*ph));
		s_cleaner(ph, threads, rules, av);
		return (1);
	}
	return (0);
}
