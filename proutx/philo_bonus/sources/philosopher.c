/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:30:41 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	eat_sleep_routine(t_philo *philo)
{
	grab_fork(philo);
	grab_fork(philo);
	write_status(philo, false, EATING);
	sem_wait(philo->sem_meal);
	philo->last_meal = get_time_in_ms();
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_eat);
	write_status(philo, false, SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->nb_forks_held -= 2;
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(time_to_think);
}

static void	lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(CHILD_EXIT_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	sim_start_delay(philo->table->start_time);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(CHILD_EXIT_PHILO_FULL);
	}
	if (DEBUG_FORMATTING == true)
		print_status_debug(philo, PURPLE, "has taken a fork", GOT_FORK_1);
	else
		print_status(philo, "has taken a fork");
	philo_sleep(philo->table->time_to_die);
	if (DEBUG_FORMATTING == true)
		print_status_debug(philo, RED, "died", DIED);
	else
		print_status(philo, "died");
	free_table(philo->table);
	exit(CHILD_EXIT_PHILO_DEAD);
}

static void	philosopher_routine(t_philo *philo)
{
	if (philo->id % 2)
		think_routine(philo, true);
	while (1)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
}

void	philosopher(t_table *table)
{
	t_philo	*philo;

	philo = table->this_philo;
	if (philo->table->nb_philos == 1)
		lone_philo_routine(philo);
	init_philo_ipc(table, philo);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	}
	if (philo->table->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		child_exit(table, CHILD_EXIT_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->table->start_time;
	sem_post(philo->sem_meal);
	sim_start_delay(philo->table->start_time);
	philosopher_routine(philo);
}
