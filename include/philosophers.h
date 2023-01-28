/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:52:23 by flplace           #+#    #+#             */
/*   Updated: 2023/01/28 15:29:05 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

# define RED "\x1B[31m"
# define WHT "\x1B[0m"

typedef struct s_list
{
	pthread_mutex_t	fork_m;
	struct s_list	*next;
}				t_list;

typedef struct s_rules
{
	int				cap;
	pthread_mutex_t	hungry_ppl_m;
	int				hungry_ppl;
	pthread_mutex_t	end_m;
	int				end;
	pthread_mutex_t	start_m;
	long int		start;
	long int		t_death;
	long int		t_eat;
	long int		t_sleep;
}				t_rules;

typedef struct s_ph
{
	t_list			*fork;
	int				ph_id;
	int				meals;
	long int		last_meal;
	t_rules			*rules;
}				t_ph;

/*		arg checker		*/
int			cap_arg_check(t_ph **ph, pthread_t ***threads,
				t_rules **rules, char **av);
int			argchecker(int ac, char **av);

/*      setup           */
void		ph_init(t_ph *ph, t_list **list, int nph, t_rules *rules);
t_rules		init_rules(char **av, int nph);
void		struct_init(t_ph **ph, pthread_t ***threads,
				t_rules **rules, char **av);
void		s_cleaner(t_ph **ph, pthread_t ***threads,
				t_rules **rules, char **av);

/*      time utilities             */
long int	timestamp(t_ph *ph);
long int	unix_timestamp(void);
void		nwait(long int time, t_ph *ph);
int			nwait_f(long int time, t_ph *ph);

/*      routine utilities          */
void		routine(t_ph *ph);
int			a_sleep(t_ph *ph);
int			a_eat(t_ph *ph);
int			a_think(t_ph *ph);
void		lock_f(t_ph *ph);
void		unlock_f(t_ph *ph);
void		forkprinter(t_ph *ph);

/*		supervisor					*/
void		ending_threads(int nphilo, pthread_t **threads, t_ph *ph);
void		ending_control(int nphilo, pthread_t **threads, t_ph *ph);

/*		ending utilities			*/
int			ending_c(t_ph *ph, int ulock_f);
int			meals_c(t_ph *ph);
int			hunger_check(t_ph *ph, int ulock_f);
int			end_flag_c(t_ph *ph);
void		starve_c(t_ph *ph);

/*      linked list utilities      */
t_list		*addback(t_list **list);
t_list		*find_last(t_list *list);
void		destroy_list(t_list *list);

/*      utilities                    */
int			ft_strlen(char *str);
int			ft_isdigit(int n);
int			ft_strisdigit(char *str);
int			ft_atoi(char *str);
int			ft_is_numeric(char b);
int			ft_is_operand(char b);
int			ft_is_space(char b);

#endif
