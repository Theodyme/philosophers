/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setups.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:51:47 by flplace           #+#    #+#             */
/*   Updated: 2023/01/31 21:51:29 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ph_init(t_ph *ph, t_list **list, int nph, t_rules *rules)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(rules->start_m), NULL);
	while (i != nph)
	{
		pthread_mutex_init(&((ph + i)->last_meal_m), NULL);
		(ph + i)->meals = 0;
		pthread_mutex_lock(&(ph + i)->last_meal_m);
		(ph + i)->last_meal = 0;
		pthread_mutex_unlock(&(ph + i)->last_meal_m);
		(ph + i)->ph_id = i + 1;
		(ph + i)->rules = rules;
		(ph + i)->fork = addback(&(*list));
		i++;
	}
	return (0);
}

t_rules	init_rules(char **av, int nph)
{
	t_rules	rules;

	pthread_mutex_init(&(rules.end_m), NULL);
	pthread_mutex_init(&(rules.print_m), NULL);
	pthread_mutex_init(&(rules.hungry_ppl_m), NULL);
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

int	struct_init(t_ph **ph, t_rules **rules, char **av)
{
	int	i;

	i = 0;
	*ph = malloc(sizeof(t_ph) * ft_atoi(av[1]));
	*rules = malloc(sizeof(t_rules));
	if (ph == NULL || rules == NULL)
	{
		s_cleaner(ph, rules);
		return (1);
	}
	**rules = init_rules(av, ft_atoi(av[1]));
	(*rules)->start = unix_timestamp();
	return (0);
}

void	s_cleaner(t_ph **ph, t_rules **rules)
{
	int	i;

	i = 0;
	if (*ph)
		free(*ph);
	if (*rules)
		free(*rules);
	return ;
}
