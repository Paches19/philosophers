/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/10 13:34:29 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_log(int philos_id, unsigned long time_init, char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = ((current_timeval.tv_sec * 1000) +
		(current_timeval.tv_usec / 1000)) - time_init;
	printf("Time: %ld \tphilosopher: %d %s\n",
		current_time, philos_id, msg);
}

void	eat(t_philosopher *philos, t_args args)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	print_log(philos->id, args.time_init_prog, "is eating 🍝");
	philos->last_time_eat = current_time;
	philos->num_eat++;
	usleep(args.time_to_eat * 1000);
}

void	take_forks(t_philosopher *philos, t_args args)
{
	if (philos->id % 2 == 0)
	{
		pthread_mutex_lock(&(philos->right_fork->mutex));
		print_log(philos->id, args.time_init_prog, "has taken a fork 🍴");
		pthread_mutex_lock(&(philos->left_fork->mutex));
		print_log(philos->id, args.time_init_prog, "has taken a fork 🍴");
	}
	else
	{
		pthread_mutex_lock(&(philos->left_fork->mutex));
		print_log(philos->id, args.time_init_prog, "has taken a fork 🍴");
		pthread_mutex_lock(&(philos->right_fork->mutex));
		print_log(philos->id, args.time_init_prog, "has taken a fork 🍴");
	}
	eat(philos, args);
	pthread_mutex_unlock(&(philos->left_fork->mutex));
	pthread_mutex_unlock(&(philos->right_fork->mutex));
}

void	sleep_philo(t_philosopher *philos, t_args args)
{
	print_log(philos->id, args.time_init_prog, "is sleeping 💤");
	usleep(args.time_to_sleep * 1000);
}

void	think_philo(t_philosopher *philos, t_args args)
{
	print_log(philos->id, args.time_init_prog, "is thinking 💭");
	usleep(args.time_to_sleep * 1000);
}

int	is_philo_alive(t_philosopher *philos, t_args args)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	if ((current_time - philos->last_time_eat) > args.time_to_die)
	{
		print_log(philos->id, args.time_init_prog, "died 💀");
		return (0);
	}
	return (1);
}

void	join_philosophers_threads(int num_philosophers,
	t_philosopher **philosophers)
{
	int	i;

	i = -1;
	while (++i < num_philosophers)
		pthread_join(philosophers[i]->philo_thread, NULL);
}

static void	*philosopher_actions(void *arg)
{
	t_actions	*actions;
	int			i;

	actions = (t_actions *)arg;
	i = 0;
	while (1)
	{
		take_forks(&actions->philos, actions->args);
		sleep_philo(&actions->philos, actions->args);
		think_philo(&actions->philos, actions->args);
		if (!is_philo_alive(&actions->philos, actions->args))
			break ;
		++i;
	}
	return (NULL);
}

void start_eating(t_args args, t_fork **forks, t_philosopher **philosophers)
{
	unsigned long	i;
	t_actions		*actions;

	(void)forks;
	actions = (t_actions *)ft_calloc(sizeof(t_actions), args.num_philosophers + 1);
	if (!actions)
		exit(EXIT_FAILURE);
	i = -1;
	while (++i < args.num_philosophers)
	{
		actions[i].args = args;
		actions[i].philos = *philosophers[i];
		pthread_create(&(philosophers[i]->philo_thread), NULL,
			philosopher_actions, &actions[i]);
	}
	join_philosophers_threads(args.num_philosophers, philosophers);
	free(actions);
}
