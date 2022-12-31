/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setups.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:47 by flplace           #+#    #+#             */
/*   Updated: 2022/12/31 07:17:34 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ph_init(t_ph *ph, t_list **list, int nph, t_rules *rules)
{
	int	i;

	i = 0;
	while (i != nph)
	{
		(ph + i)->meals = 0;
		(ph + i)->last_meal = 0;
		(ph + i)->ph_id = i + 1;
		(ph + i)->rules = rules;
		(ph + i)->fork = addback(&(*list));
		i++;
	}
	return ;
}

t_rules	init_rules(char **av, int nph)
{
	t_rules	rules;

	pthread_mutex_init(&(rules.end_m), NULL);
	pthread_mutex_init(&(rules.hungry_ppl_m), NULL);
	pthread_mutex_init(&(rules.start_m), NULL);
	rules.end = 0;
	rules.start = 0;
	rules.hungry_ppl = nph;
	rules.t_death = ft_atoi(av[2]);
	rules.t_eat = ft_atoi(av[3]);
	rules.t_sleep = ft_atoi(av[4]);
	if (av[5])
		rules.cap = ft_atoi(av[5]);
	else
		rules.cap = 0;
	return (rules);
}

void	struct_init(t_ph **ph, pthread_t ***threads, t_rules **rules, char **av)
{
	int	i;

	i = 0;
	*threads = malloc(sizeof(pthread_t *) * ft_atoi(av[1]));
	while (i != ft_atoi(av[1]))
	{
		(*threads)[i] = malloc(sizeof(pthread_t));
		i++;
	}
	*ph = malloc(sizeof(t_ph) * ft_atoi(av[1]));
	*rules = malloc(sizeof(t_rules));
	**rules = init_rules(av, ft_atoi(av[1]));
	(*rules)->start = unix_timestamp();
	return ;
}

void	s_cleaner(t_ph **ph, pthread_t ***threads, t_rules **rules, char **av)
{
	int	i;

	i = 0;
	while (i != ft_atoi(av[1]))
	{
		free((*threads)[i]);
		i++;
	}
	free(*threads);
	free(*ph);
	free(*rules);
	return ;
}
