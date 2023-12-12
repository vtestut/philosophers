/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:03:34 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/27 16:57:04 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

# define MAX_PHILO	250

typedef struct s_philo	t_philo;

typedef struct s_datas
{
	unsigned int	n;
	time_t			tt_die;
	time_t			tt_eat;
	time_t			tt_sleep;
	time_t			start_time;
	pthread_t		checker;
	pthread_mutex_t	stop_threads_mtx;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	*forks_mtx;
	bool			stop_warning;
	int				must_eat;
	t_philo			**philos;
}	t_datas;

typedef struct s_philo
{
	unsigned int		id;
	unsigned int		eat_count;
	unsigned int		fork[2];
	pthread_t			thread;
	pthread_mutex_t		meal_time_mtx;
	time_t				last_meal;
	t_datas				*d;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	FORK_1 = 4,
	FORK_2 = 5
}	t_state;

bool			check_params(int ac, char **av);
int				ft_atoi(char *str);
t_datas			*init_datas(int ac, char **av, int i);
void			*philosopher(void *d);
time_t			get_time(void);
void			delay_start(time_t start_time);
void			print_state(t_philo *philo, bool reaper, t_state status);
int				msg(char *str, char *detail, int exit_no);
void			*checker(void *d);
bool			activity_has_stopped(t_datas *d);
void			*free_all(t_datas *d);
void			destroy_mutexes(t_datas *d);
int				error_failure(char *str, char *details, t_datas *d);
void			*error_null(char *str, char *details, t_datas *d);

#endif