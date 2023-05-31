/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 12:04:03 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/31 12:10:25 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	error_exit_thread(void)
{
	ft_putstr_fd("ERROR: Thread init failed\n", 2);
	exit(1);
}

void	print_log(t_actions *actions, int philos_id, long time_init, char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	pthread_mutex_lock(&(actions->stop->mutex));
	pthread_mutex_lock(&(actions->print_mutex));
	if (!actions->stop->stop || !ft_strcmp("died", msg))
	{
		pthread_mutex_unlock(&(actions->stop->mutex));
		gettimeofday(&current_timeval, NULL);
		current_time = ((current_timeval.tv_sec * 1000)
				+ (current_timeval.tv_usec / 1000)) - time_init;
		if (!ft_strcmp("is eating", msg))
			printf("Time: %ld philosopher: %d %s meals: %ld\n",
				current_time, philos_id, msg, actions->philos->num_eat + 1);
		else
			printf("Time: %ld philosopher: %d %s meals: %ld\n",
				current_time, philos_id, msg, actions->philos->num_eat);
		pthread_mutex_unlock(&(actions->print_mutex));
	}
	else
	{
		pthread_mutex_unlock(&(actions->print_mutex));
		pthread_mutex_unlock(&(actions->stop->mutex));
	}
}

void	print_death(t_actions *actions, int philos_id, long time_init,
	char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	pthread_mutex_lock(&(actions->stop->mutex));
	actions->stop->stop = 1;
	pthread_mutex_lock(&(actions->print_mutex));
	gettimeofday(&current_timeval, NULL);
	current_time = ((current_timeval.tv_sec * 1000)
			+ (current_timeval.tv_usec / 1000)) - time_init;
	printf("Time: %ld philosopher: %d %s meals: %ld\n",
		current_time, philos_id, msg, actions->philos->num_eat);
	pthread_mutex_unlock(&(actions->print_mutex));
	pthread_mutex_unlock(&(actions->stop->mutex));
}
