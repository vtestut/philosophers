/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:38:33 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:29:46 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static bool	philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

static bool	philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

void	init_philo_ipc(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!philo_open_global_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (!philo_open_local_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->personal_grim_reaper, NULL,
			&personal_grim_reaper, table) != 0)
		child_exit(table, CHILD_EXIT_ERR_PTHREAD);
	return ;
}
