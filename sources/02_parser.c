/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtestut <vtestut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:04:57 by vtestut           #+#    #+#             */
/*   Updated: 2023/08/26 00:19:06 by vtestut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	digits_only(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi(char *s)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (s[i] && (s[i] >= '0' && s[i] <= '9'))
	{
		nb = nb * 10 + (s[i] - '0');
		i++;
	}
	if (nb > 2147483647)
		return (-1);
	return ((int)nb);
}

bool	check_params(int ac, char **av)
{
	int	i;
	int	nb;

	i = 1;
	while (i < ac)
	{
		if (!digits_only(av[i]))
			return (false);
		nb = ft_atoi(av[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILO))
			return (false);
		if (i != 1 && nb == -1)
			return (false);
		i++;
	}
	return (true);
}

int	error_failure(char *str, char *details, t_datas *d)
{
	if (d != NULL)
		free_all(d);
	return (msg(str, details, 0));
}

void	*error_null(char *str, char *details, t_datas *d)
{
	if (d != NULL)
		free_all(d);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}
