#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>


# define MAX_PHILOS	250

typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		grim_reaper;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;


/*  PARSING                                                         */


#endif