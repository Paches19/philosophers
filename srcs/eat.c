/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:57:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/17 11:16:37 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_actions *actions)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 +
		current_timeval.tv_usec / 1000;
	pthread_mutex_lock(&(actions->stop->mutex));
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		return ;
	}
	else if (actions->philos->state == TO_EAT)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->state = EATING;
		actions->philos->last_time_eat = current_time;
		actions->philos->init_state = current_time;
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"is eating 🍝");
	}
	else if (actions->philos->state == EATING && current_time -
		actions->philos->init_state >= actions->args.time_to_eat)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->num_eat++;
		actions->philos->state = TO_SLEEP;
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		actions->philos->l_fork_taken = 0;
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		actions->philos->r_fork_taken = 0;
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}

static void	even_fork_lock(t_actions *actions)
{
	pthread_mutex_lock(&(actions->philos->right_fork->mutex));
	actions->philos->r_fork_taken = 1;
	usleep(50);
	pthread_mutex_lock(&(actions->stop->mutex));
	if (!actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"has taken a fork 🍴");
		pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		actions->philos->l_fork_taken = 1;
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
	usleep(50);
	print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"has taken a fork 🍴");
}

static void	odd_fork_lock(t_actions *actions)
{
	pthread_mutex_lock(&(actions->philos->left_fork->mutex));
	actions->philos->l_fork_taken = 1;
	usleep(50);
	pthread_mutex_lock(&(actions->stop->mutex));
	if (!actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"has taken a fork 🍴");
		pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		actions->philos->r_fork_taken = 1;
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
	usleep(50);
	print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"has taken a fork 🍴");
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
