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

static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time() + (table->nb_philos * 2 * 10);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return (error_failure("couldn't create thread\n", NULL, table));
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&grim_reaper, table) != 0)
			return (error_failure("couldn't create thread\n", NULL, table));
	}
	return (true);
}

static void	stop_simulation(t_table	*table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	destroy_mutexes(table);
	free_table(table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (printf("wrong input\n"), EXIT_FAILURE);
	if (!is_valid_input(ac, av))
		return (printf("wrong input\n"), EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
