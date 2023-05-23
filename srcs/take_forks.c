/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 18:19:11 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/23 17:31:17 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static void	even_fork_lock(t_actions *actions)
{
	pthread_mutex_lock(&(actions->philos->right_fork->mutex));
	actions->philos->r_fork_taken = 1;
	pthread_mutex_lock(&(actions->stop->mutex));
	if (!actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"has taken a fork");
		pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		actions->philos->l_fork_taken = 1;
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"has taken a fork");
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}

static void	odd_fork_lock(t_actions *actions)
{
	pthread_mutex_lock(&(actions->philos->left_fork->mutex));
	actions->philos->l_fork_taken = 1;
	pthread_mutex_lock(&(actions->stop->mutex));
	if (!actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"has taken a fork");
		pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		actions->philos->r_fork_taken = 1;
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"has taken a fork");
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}

void	take_forks(t_actions *actions)
{
	pthread_mutex_lock(&(actions->stop->mutex));
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		return ;
	}
	else if (actions->philos->id % 2 == 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		even_fork_lock(actions);
	}
	else if (actions->philos->id % 2 != 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		odd_fork_lock(actions);
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}
