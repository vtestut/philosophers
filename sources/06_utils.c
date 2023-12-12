/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 14:02:45 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/27 16:57:04 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	delay_start(time_t start_time)
{
	while (get_time() < start_time)
		continue ;
}

int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, "philo");
	else
		printf(str, "philo", detail);
	return (exit_no);
}

static void	print_state_aux(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time() - philo->d->start_time,
		philo->id + 1, str);
}

void	print_state(t_philo *philo, bool reaper_report, t_state status)
{
	pthread_mutex_lock(&philo->d->print_mtx);
	if (activity_has_stopped(philo->d) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->d->print_mtx);
		return ;
	}
	if (status == DIED)
		print_state_aux(philo, "died");
	else if (status == EATING)
		print_state_aux(philo, "is eating");
	else if (status == SLEEPING)
		print_state_aux(philo, "is sleeping");
	else if (status == THINKING)
		print_state_aux(philo, "is thinking");
	else if (status == FORK_1 || status == FORK_2)
		print_state_aux(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->d->print_mtx);
}
