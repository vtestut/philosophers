/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:09:15 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 04:22:00 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILO	250

typedef struct s_philo	t_philo;

typedef struct s_table
{
	unsigned int	nb_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			start_time;
	pthread_t		grim_reaper;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	bool			sim_stop;
	int				must_eat;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_t			thread;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/*****************************************************************************/

//	parsing.c
bool			is_valid_input(int ac, char **av);
int				integer_atoi(char *str);

//	init.c
t_table			*init_table(int ac, char **av, int i);

//	routines.c
void			*philosopher(void *data);
// void			sleep_think(t_table *table, time_t sleep_time);

//	utils.c
time_t			get_time(void);
void			sim_start_delay(time_t start_time);
void			write_status(t_philo *philo, bool reaper, t_status status);

//	grim_reaper.c
void			*grim_reaper(void *data);
bool			has_simulation_stopped(t_table *table);

//	exit.c
void			*free_table(t_table *table);
void			destroy_mutexes(t_table *table);
int				error_failure(char *str, char *details, t_table *table);
void			*error_null(char *str, char *details, t_table *table);
int				msg(char *str, char *detail, int exit_no);

#endif