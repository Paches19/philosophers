/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_think.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:59:00 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/17 10:46:27 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	sleep_philo(t_actions *actions)
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
	else if (actions->philos->state == TO_SLEEP)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->init_state = current_time;
		actions->philos->state = SLEEPING;
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"is sleeping 💤");
	}
	else if (actions->philos->state == SLEEPING && current_time -
		actions->philos->init_state >= actions->args.time_to_sleep)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->state = TO_THINK;
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}

void	think_philo(t_actions *actions)
{
	pthread_mutex_lock(&(actions->stop->mutex));
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		return ;
	}
	else if (actions->philos->state == TO_THINK)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->state = THINKING;
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"is thinking 💭");
	}
	else if (actions->philos->state == THINKING)
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		actions->philos->state = TO_EAT;
	}
	else
		pthread_mutex_unlock(&(actions->stop->mutex));
}
