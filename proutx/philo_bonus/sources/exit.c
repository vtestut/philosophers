/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:27:50 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:29:05 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (msg(str, details, 0));
}

void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

void	child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->personal_grim_reaper, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	free_table(table);
	exit(exit_code);
}
