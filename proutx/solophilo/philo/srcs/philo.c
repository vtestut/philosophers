/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:04:18 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 04:59:50 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_sleep(t_datas *d, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time() + sleep_time;
	while (get_time() < wake_up)
	{
		if (activity_has_stopped(d))
			break ;
		usleep(100);
	}
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->d->forks_mtx[philo->fork[0]]);
	print_state(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->d->forks_mtx[philo->fork[1]]);
	print_state(philo, false, GOT_FORK_2);
	print_state(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_mtx);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_time_mtx);
	ft_sleep(philo->d, philo->d->tt_eat);
	if (activity_has_stopped(philo->d) == false)
	{
		pthread_mutex_lock(&philo->meal_time_mtx);
		philo->eat_count += 1;
		pthread_mutex_unlock(&philo->meal_time_mtx);
	}
	print_state(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->d->forks_mtx[philo->fork[1]]);
	pthread_mutex_unlock(&philo->d->forks_mtx[philo->fork[0]]);
	ft_sleep(philo->d, philo->d->tt_sleep);
}

static void	think(t_philo *philo, bool silent)
{
	time_t	tt_think;

	pthread_mutex_lock(&philo->meal_time_mtx);
	tt_think = (philo->d->tt_die - (get_time() - philo->last_meal) - philo->d->tt_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_mtx);
	if (tt_think < 0) // cela signifie que le temps écoulé depuis le dernier repas dépasse le temps de mort (tt_die) et le philosophe a faim. Dans ce cas, il ne doit pas penser, donc la valeur tt_think est mise à 0.
		tt_think = 0;
	if (tt_think == 0 && silent == true)
		tt_think = 1;
	if (tt_think > 600) // valeur arbitraire choisie ici, elle est réduite à 200 pour éviter de prendre trop de temps pour réfléchir.
		tt_think = 200;
	if (silent == false)
		print_state(philo, false, THINKING);
	ft_sleep(philo->d, tt_think);
}

static void	*one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->d->forks_mtx[philo->fork[0]]);
	print_state(philo, false, GOT_FORK_1);
	ft_sleep(philo->d, philo->d->tt_die);
	print_state(philo, false, DIED);
	pthread_mutex_unlock(&philo->d->forks_mtx[philo->fork[0]]);
	return (NULL);
}

void	*philosopher(void *d)
{
	t_philo	*philo;

	philo = (t_philo *)d;
	if (philo->d->must_eat == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_mtx);
	philo->last_meal = philo->d->start_time;
	pthread_mutex_unlock(&philo->meal_time_mtx);
	delay_start(philo->d->start_time);
	if (philo->d->tt_die == 0)
		return (NULL);
	if (philo->d->n == 1)
		return (one_philo(philo));
	else if (philo->id % 2) // si impaire
		think(philo, true);
	while (activity_has_stopped(philo->d) == false)
	{
		eat(philo);
		think(philo, false);
	}
	return (NULL);
}
