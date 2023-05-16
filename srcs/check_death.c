/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:55:05 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 20:56:08 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	join_philosophers_threads(int num_philosophers,
	t_philosopher **philosophers)
{
	int	i;

	i = -1;
	while (++i < num_philosophers)
		pthread_join(philosophers[i]->philo_thread, NULL);
}

int	end_eat_times(t_actions *actions)
{
	if (actions->args.num_eat == -1)
		return (0);
	else if (actions->philos->num_eat >= actions->args.num_eat)
	{
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		return (1);
	}
	return (0);
}

static int	end_philo(t_actions *actions)
{
	pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
	pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
	pthread_mutex_unlock(&(actions->stop->mutex));
	return (0);
}

int	is_philo_alive(t_actions *actions)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 +
		current_timeval.tv_usec / 1000;
	pthread_mutex_lock(&(actions->stop->mutex));
	if (actions->stop->stop)
		return (end_philo(actions));
	else if ((current_time - actions->philos->last_time_eat)
		> actions->args.time_to_die && actions->philos->state != EATING)
	{
		actions->stop->stop = 1;
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog,
			"died 💀");
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		return (0);
	}
	pthread_mutex_unlock(&(actions->stop->mutex));
	return (1);
}
