/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 21:15:24 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_log(t_actions *actions, int philos_id, long time_init, char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = ((current_timeval.tv_sec * 1000) +
		(current_timeval.tv_usec / 1000)) - time_init;
	if (!actions->stop->stop || !ft_strcmp("died", msg))
	{
		pthread_mutex_lock(&(actions->print_mutex));
		if (!ft_strcmp("is eating", msg))
			printf("Time: %-*ld philosopher: %-*d %-*s meals: %-*ld\n",
   			WIDTH_TIME, current_time, WIDTH_PHILOSOPHER, philos_id, WIDTH_MSG,
			msg, WIDTH_MEALS, actions->philos->num_eat + 1);
		else
			printf("Time: %-*ld philosopher: %-*d %-*s meals: %-*ld\n",
   			WIDTH_TIME, current_time, WIDTH_PHILOSOPHER, philos_id, WIDTH_MSG,
			msg, WIDTH_MEALS, actions->philos->num_eat);
		pthread_mutex_unlock(&(actions->print_mutex));
	}
}

static void	*philosopher_actions(void *arg)
{
	t_actions	*actions;
	int			alive;

	actions = (t_actions *)arg;
	actions->philos->last_time_eat = actions->args.time_init_prog;
	while (1)
	{
		think_philo(actions);
		take_forks(actions);
		if (end_eat_times(actions))
			return (NULL);
		sleep_philo(actions);
		alive = is_philo_alive(actions);
		if (!alive)
			return (NULL);
		usleep(200);
	}
	return (NULL);
}

static void	init_args(t_stop *stop_sig, pthread_mutex_t *p_mutex, t_args *args,
	struct timeval *current_time)
{
	stop_sig->stop = 0;
	if (pthread_mutex_init(&(stop_sig->mutex), NULL) < 0)
		error_exit_mutex();
	if (pthread_mutex_init((p_mutex), NULL) < 0)
		error_exit_mutex();
	gettimeofday(current_time, NULL);
	args->time_init_prog = current_time->tv_sec * 1000 +
	current_time->tv_usec / 1000;
}

void	start_eating(t_args args, t_philosopher **philosophers)
{
	unsigned long	i;
	t_actions		*actions;
	t_stop			stop_signal;
	pthread_mutex_t	print_mutex;
	struct timeval	current_time;

	actions = (t_actions *)ft_calloc(sizeof(t_actions), args.num_philosophers + 1);
	if (!actions)
		exit(EXIT_FAILURE);
	init_args(&stop_signal, &print_mutex, &args, &current_time);
	i = -1;
	while (++i < args.num_philosophers)
	{
		actions[i].args = args;
		actions[i].philos = philosophers[i];
		actions[i].stop = &stop_signal;
		actions[i].print_mutex = print_mutex;
		if (pthread_create(&(philosophers[i]->philo_thread), NULL,
			philosopher_actions, &actions[i]) < 0)
			error_exit_thread();
	}
	join_philosophers_threads(args.num_philosophers, philosophers);
	free(actions);
	pthread_mutex_destroy(&(stop_signal.mutex));
	pthread_mutex_destroy(&(print_mutex));
}
