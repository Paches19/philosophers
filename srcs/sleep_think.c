/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_think.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:59:00 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 20:59:09 by adpachec         ###   ########.fr       */
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
	if (actions->stop->stop)
		return ;
	else if (actions->philos->state == TO_SLEEP)
	{
		actions->philos->init_state = current_time;
		actions->philos->state = SLEEPING;
		pthread_mutex_lock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
		"is sleeping 💤");
		pthread_mutex_unlock(&(actions->stop->mutex));
	}
	else if (actions->philos->state == SLEEPING && current_time -
		actions->philos->init_state >= actions->args.time_to_sleep)
		actions->philos->state = TO_THINK;
}

void	think_philo(t_actions *actions)
{
	if (actions->stop->stop)
		return ;
	else if (actions->philos->state == TO_THINK)
	{
		actions->philos->state = THINKING;
		pthread_mutex_lock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"is thinking 💭");
		pthread_mutex_unlock(&(actions->stop->mutex));
	}
	else if (actions->philos->state == THINKING)
		actions->philos->state = TO_EAT;
}