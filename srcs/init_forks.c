/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:41:38 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/11 14:21:21 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static t_fork	*init_fork(int id)
{
	t_fork	*fork;

	fork = (t_fork *)ft_calloc(sizeof(t_fork), 1);
	if (!fork)
		error_exit_malloc();
	fork->id = id;
	pthread_mutex_init(&fork->mutex, NULL);
	return (fork);
}

t_fork	**init_forks(unsigned long num_forks)
{
	unsigned long	i;
	t_fork			**forks;

	forks = (t_fork **)ft_calloc(sizeof(t_fork *), num_forks + 1);
	if (!forks)
		error_exit_malloc();
	i = -1;
	while (++i < num_forks)
		forks[i] = init_fork(i);
	return (forks);
}
