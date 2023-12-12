/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 22:05:30 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 23:27:39 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_stop_warning(t_datas *d, bool state)
{
	pthread_mutex_lock(&d->stop_threads_mtx);
		d->stop_warning = state;
	pthread_mutex_unlock(&d->stop_threads_mtx);
}

bool	activity_has_stopped(t_datas *d)
{
	bool	b;

	b = false;
	pthread_mutex_lock(&d->stop_threads_mtx);
	if (d->stop_warning == true)
		b = true;
	pthread_mutex_unlock(&d->stop_threads_mtx);
	return (b);
}

static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time();
	if ((time - philo->last_meal) >= philo->d->tt_die)
	{
		set_stop_warning(philo->d, true);
		print_state(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_mtx);
		return (true);
	}
	return (false);
}

static bool	stop_conditions(t_datas *d)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < d->n)
	{
		pthread_mutex_lock(&d->philos[i]->meal_time_mtx);
		if (kill_philo(d->philos[i]))
			return (true);
		if (d->must_eat != -1)
			if (d->philos[i]->eat_count
				< (unsigned int)d->must_eat)
				all_ate_enough = false;
		pthread_mutex_unlock(&d->philos[i]->meal_time_mtx);
		i++;
	}
	if (d->must_eat != -1 && all_ate_enough == true)
	{
		set_stop_warning(d, true);
		return (true);
	}
	return (false);
}

void	*checker(void *datas)
{
	t_datas			*d;

	d = (t_datas *)datas;
	if (d->must_eat == 0)
		return (NULL);
	set_stop_warning(d, false);
	delay_start(d->start_time);
	while (true)
	{
		if (stop_conditions(d) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
