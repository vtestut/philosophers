/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:02:16 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 04:22:00 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
