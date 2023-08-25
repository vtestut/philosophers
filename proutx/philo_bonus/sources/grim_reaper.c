/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:29:18 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	kill_all_philos(t_table *table, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

void	*global_gluttony_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count < 0 || table->time_to_die == 0
		|| table->nb_philos == 1)
		return (NULL);
	sim_start_delay(table->start_time);
	while (table->philo_full_count < table->nb_philos)
	{
		if (has_simulation_stopped(table) == true)
			return (NULL);
		sem_wait(table->sem_philo_full);
		if (has_simulation_stopped(table) == false)
			table->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_dead);
	sem_post(table->sem_stop);
	return (NULL);
}

void	*global_famine_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->nb_philos == 1)
		return (NULL);
	sim_start_delay(table->start_time);
	if (has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_philo_dead);
	if (has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_full);
	sem_post(table->sem_stop);
	return (NULL);
}

static bool	end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		write_status(philo, true, DIED);
		sem_post(table->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (table->must_eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= (unsigned int)table->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

void	*personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sem_wait(table->this_philo->sem_philo_dead);
	sem_wait(table->this_philo->sem_philo_full);
	sim_start_delay(table->start_time);
	while (!end_condition_reached(table, table->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}
