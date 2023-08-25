/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 11:35:04 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:29:37 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*set_local_sem_name(const char *str, unsigned int id)
{
	unsigned int	i;
	unsigned int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

static bool	set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (false);
	return (true);
}

static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		if (!set_philo_sem_names(philos[i]))
			return (error_null(STR_ERR_MALLOC, NULL, table));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = false;
		i++;
	}
	return (philos);
}

static bool	init_global_semaphores(t_table *table)
{
	unlink_global_sems();
	table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_forks == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_write == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_full == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_dead == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_stop == SEM_FAILED)
		return (sem_error_cleanup(table));
	return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = integer_atoi(av[i++]);
	table->time_to_die = integer_atoi(av[i++]);
	table->time_to_eat = integer_atoi(av[i++]);
	table->time_to_sleep = integer_atoi(av[i++]);
	table->must_eat_count = -1;
	table->philo_full_count = 0;
	table->stop_sim = false;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	if (!init_global_semaphores(table))
		return (NULL);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	return (table);
}
