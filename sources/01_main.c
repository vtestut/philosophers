/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:02:16 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/27 16:57:04 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*free_all(t_datas *d)
{
	unsigned int	i;

	if (!d)
		return (NULL);
	if (d->forks_mtx != NULL)
		free(d->forks_mtx);
	if (d->philos != NULL)
	{
		i = 0;
		while (i < d->n)
		{
			if (d->philos[i] != NULL)
				free(d->philos[i]);
			i++;
		}
		free(d->philos);
	}
	free(d);
	return (NULL);
}

void	destroy_mutexes(t_datas *d)
{
	unsigned int	i;

	i = 0;
	while (i < d->n)
	{
		pthread_mutex_destroy(&d->forks_mtx[i]);
		pthread_mutex_destroy(&d->philos[i]->meal_time_mtx);
		i++;
	}
	pthread_mutex_destroy(&d->print_mtx);
	pthread_mutex_destroy(&d->stop_threads_mtx);
}

static void	exit_free(t_datas	*d)
{
	unsigned int	i;

	i = 0;
	while (i < d->n)
	{
		pthread_join(d->philos[i]->thread, NULL);
		i++;
	}
	if (d->n > 1)
		pthread_join(d->checker, NULL);
	destroy_mutexes(d);
	free_all(d);
}

static bool	start_threads(t_datas *d)
{
	unsigned int	i;

	d->start_time = get_time() + (d->n * 2 * 10);
	i = 0;
	while (i < d->n)
	{
		if (pthread_create(&d->philos[i]->thread, NULL,
				&philosopher, d->philos[i]) != 0)
			return (error_failure("couldn't create thread\n", NULL, d));
		i++;
	}
	if (d->n > 1)
	{
		if (pthread_create(&d->checker, NULL,
				&checker, d) != 0)
			return (error_failure("couldn't create thread\n", NULL, d));
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_datas	*d;

	d = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (printf("wrong input\n"), EXIT_FAILURE);
	if (!check_params(ac, av))
		return (printf("wrong input\n"), EXIT_FAILURE);
	d = init_datas(ac, av, 1);
	if (!d)
		return (EXIT_FAILURE);
	if (!start_threads(d))
		return (EXIT_FAILURE);
	exit_free(d);
	return (EXIT_SUCCESS);
}
