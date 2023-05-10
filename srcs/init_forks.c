/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:41:38 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 13:26:31 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static t_fork	*init_fork(int id)
{
	t_fork	*fork;

	fork = (t_fork *)malloc(sizeof(t_fork));
	if (!fork)
		error_exit_malloc();
	fork->id = id;
	pthread_mutex_init(&fork->mutex, NULL);
	return (fork);
}

t_fork	**init_forks(int num_forks)
{
	int		i;
	t_fork	**forks;

	forks = (t_fork **)malloc(sizeof(t_fork *) * num_forks);
	if (!forks)
		error_exit_malloc();
	i = -1;
	while (++i < num_forks)
		forks[i] = init_fork(i);
	return (forks);
}
