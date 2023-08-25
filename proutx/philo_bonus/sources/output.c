/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:20:23 by vtestut          #+#    #+#             */
/*   Updated: 2023/08/17 12:30:11 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #1\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #2\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		if (!reaper_report)
			sem_post(philo->sem_write);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}

void	write_outcome(t_table *table)
{
	sem_post(table->sem_write);
	sem_wait(table->sem_write);
	printf("%d/%d philosophers had at least %d meals.\n",
		table->philo_full_count, table->nb_philos, table->must_eat_count);
	sem_post(table->sem_write);
	return ;
}
