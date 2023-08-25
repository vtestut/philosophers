/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:03:24 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 04:50:59 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*init_forks(t_datas *d)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * d->n);
	if (!forks)
		return (error_null("malloc error\n", NULL, 0));
	i = 0;
	while (i < d->n)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_null("couldn't create thread\n", NULL, 0));
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->d->n;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->d->n;
		philo->fork[1] = philo->id;
	}
}

static t_philo	**init_philos(t_datas *d)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * d->n);
	if (!philos)
		return (error_null("malloc error\n", NULL, 0));
	i = 0;
	while (i < d->n)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null("malloc error\n", NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_mtx, 0) != 0)
			return (error_null("couldn't create thread\n", NULL, 0));
		philos[i]->d = d;
		philos[i]->id = i;
		philos[i]->eat_count = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_mtx(t_datas *d)
{
	d->forks_mtx = init_forks(d);
	if (!d->forks_mtx)
		return (false);
	if (pthread_mutex_init(&d->stop_threads_mtx, 0) != 0)
		return (error_failure("couldn't create thread\n", NULL, d));
	if (pthread_mutex_init(&d->printer_mtx, 0) != 0)
		return (error_failure("couldn't create thread\n", NULL, d));
	return (true);
}

t_datas	*init_datas(int ac, char **av, int i)
{
	t_datas	*d;

	d = malloc(sizeof(t_datas) * 1);
	if (!d)
		return (error_null("malloc error\n", NULL, 0));
	d->n = ft_atoi(av[i++]);
	d->tt_die = ft_atoi(av[i++]);
	d->tt_eat = ft_atoi(av[i++]);
	d->tt_sleep = ft_atoi(av[i++]);
	d->must_eat = -1;
	if (ac - 1 == 5)
		d->must_eat = ft_atoi(av[i]);
	d->philos = init_philos(d);
	if (!d->philos)
		return (NULL);
	if (!init_mtx(d))
		return (NULL);
	d->stop_warning = false;
	return (d);
}
