/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 13:31:05 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_log(int philos_id, char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	printf("Time: %ld	philosopher: %d %s",
		current_time, philos_id, msg);
}

void	eat(t_philosopher *philos, t_args args)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	print_log(philos->id, "is eating 🍝");
	philos->last_time_eat = current_time;
	philos->num_eat++;
	usleep(args.time_to_eat);
}

void	take_forks(t_philosopher *philos, t_args args)
{
	if (philos->id % 2 == 0)
	{
		pthread_mutex_lock(&(philos->right_fork->mutex));
		print_log(philos->id, "has taken a fork 🍴");
		pthread_mutex_lock(&(philos->left_fork->mutex));
		print_log(philos->id, "has taken a fork 🍴");
	}
	else
	{
		pthread_mutex_lock(&(philos->left_fork->mutex));
		print_log(philos->id, "has taken a fork 🍴");
		pthread_mutex_lock(&(philos->right_fork->mutex));
		print_log(philos->id, "has taken a fork 🍴");
	}
	eat(philos, args);
	pthread_mutex_unlock(&(philos->left_fork->mutex));
	pthread_mutex_unlock(&(philos->right_fork->mutex));
}

void	sleep_philo(t_philosopher *philos, t_args args)
{
	print_log(philos->id, "is sleeping 💤");
	usleep(args.time_to_sleep);
}

void	think_philo(t_philosopher *philos, t_args args)
{
	print_log(philos->id, "is thinking 💭");
	usleep(args.time_to_sleep);
}

int	is_philo_alive(t_philosopher *philos, t_args args)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	if ((current_time - philos->last_time_eat) > args.time_to_die)
	{
		print_log(philos->id, "died 💀");
		return (0);
	}
	return (1);
}

static void	philosopher_actions(t_philosopher *philos, t_args args)
{
	while (1)
	{
		take_forks(philos, args);
		sleep_philo(philos, args);
		think_philo(philos, args);
		if (!is_philo_alive(philos, args))
			break ;
	}
}

void start_eating(t_args args, t_fork **forks, t_philosopher **philosophers)
{
	unsigned long	i;
	int				num_finished;

	i = -1;
	while (++i < args.num_philosophers)
	{
		pthread_create(&(philosophers[i]->philo_thread), NULL,
			&philosopher_actions, philosophers[i]);
		usleep(10);
	}
	num_finished = 0;
	while (num_finished < args.num_philosophers) 
	{
		i = -1;
		while (++i < args.num_philosophers) 
		{
			if (philosophers[i]->num_eat >= args.num_eat && args.num_eat != -1)
				num_finished++;
		}

		if (num_finished == args.num_philosophers)
			break;
	}
	join_philosophers_threads(args.num_philosophers, philosophers);
}
