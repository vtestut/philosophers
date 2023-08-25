/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 15:18:43 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/25 04:18:31 by vtestut          ###   ########.fr       */
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
	pthread_mutex_destroy(&d->printer_mtx);
	pthread_mutex_destroy(&d->stop_threads_mtx);
}

/* error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.
*/
int	error_failure(char *str, char *details, t_datas *d)
{
	if (d != NULL)
		free_all(d);
	return (msg(str, details, 0));
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_datas *d)
{
	if (d != NULL)
		free_all(d);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}
