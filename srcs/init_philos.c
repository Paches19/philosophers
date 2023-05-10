/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:41:36 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 13:21:03 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static t_philosopher	*init_philosopher(int id, t_fork *left_fork, t_fork *right_fork)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)malloc(sizeof(t_philosopher));
	if (!philosopher)
		error_exit_malloc();
	philosopher->id = id;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = right_fork;
	philosopher->num_eat = 0;
	philosopher->last_time_eat = 0;
	return (philosopher);
}

t_philosopher	**init_philosophers(int num_philosophers, t_fork **forks)
{
	int				i;
	t_philosopher	**philosophers;

	philosophers = (t_philosopher **)malloc(sizeof(t_philosopher *) * num_philosophers);
	if (!philosophers)
		error_exit_malloc();
	i = -1;
	while (++i < num_philosophers)
		philosophers[i] = init_philosopher(i + 1, forks[i], forks[(i + 1) % num_philosophers]);
	return (philosophers);
}
