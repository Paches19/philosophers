/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/23 18:17:13 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
		current_time = ((current_timeval.tv_sec * 1000) +
			(current_timeval.tv_usec / 1000)) - time_init;
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

static void	*philosopher_actions(void *arg)
{
	t_actions	*actions;
	int			alive;

	actions = (t_actions *)arg;
	alive = 1;
	while (alive)
	{
		think_philo(actions);
		take_forks(actions);
		eat(actions);
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

void	print_death(t_actions *actions, int philos_id, long time_init, char *msg)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	pthread_mutex_lock(&(actions->stop->mutex));
	actions->stop->stop = 1;
	pthread_mutex_lock(&(actions->print_mutex));
	gettimeofday(&current_timeval, NULL);
	current_time = ((current_timeval.tv_sec * 1000) +
		(current_timeval.tv_usec / 1000)) - time_init;
	printf("Time: %ld philosopher: %d %s meals: %ld\n",
		current_time, philos_id, msg, actions->philos->num_eat);
	pthread_mutex_unlock(&(actions->print_mutex));
	pthread_mutex_unlock(&(actions->stop->mutex));
}

void	is_philo_alive_main(t_actions *actions, int *stop, t_stop *stop_signal)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	pthread_mutex_lock(&(stop_signal->mutex));
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(stop_signal->mutex));
		*stop = 1;
		return ;
	}
	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 +
		current_timeval.tv_usec / 1000;
	if ((current_time - actions->philos->last_time_eat)
		>= actions->args.time_to_die)
	{
		pthread_mutex_unlock(&(stop_signal->mutex));
		print_death(actions, actions->philos->id, actions->args.time_init_prog,
			"died");
	}
	else
		pthread_mutex_unlock(&(stop_signal->mutex));
}

static void	init_args(t_stop *stop_sig, pthread_mutex_t *p_mutex)
{
	stop_sig->stop = 0;
	if (pthread_mutex_init(&(stop_sig->mutex), NULL) < 0)
		error_exit_mutex();
	if (pthread_mutex_init((p_mutex), NULL) < 0)
		error_exit_mutex();
}

t_actions	*init_actions(t_args *args, t_philosopher **philosophers,
	t_stop *stop_signal, pthread_mutex_t *print_mutex)
{
	unsigned long	i;
	struct timeval	current_time;
	t_actions		*actions;
	
	actions = (t_actions *)ft_calloc(sizeof(t_actions), args->num_philosophers + 1);
	if (!actions)
		exit(EXIT_FAILURE);
	gettimeofday(&current_time, NULL);
	args->time_init_prog = current_time.tv_sec * 1000 +
	current_time.tv_usec / 1000;
	i = 0;
	while (i < args->num_philosophers)
	{
		actions[i].args = args[0];
		actions[i].philos = philosophers[i];
		actions[i].philos->last_time_eat = args->time_init_prog;
		actions[i].stop = stop_signal;
		actions[i].print_mutex = *print_mutex;
		++i;
	}
	return (actions);
}

void	check_death_main(t_args args, t_stop *stop_signal, t_actions *actions)
{
	int 			stop;
	unsigned long	i;

	stop = 0;
	while (!stop)
	{
		i = -1;
		while (++i < args.num_philosophers && !stop)
			is_philo_alive_main(&actions[i], &stop, stop_signal);
	}
}

void	start_eating(t_args args, t_philosopher **philosophers)
{
	unsigned long	i;
	t_actions		*actions;
	t_stop			stop_signal;
	pthread_mutex_t	print_mutex;

	init_args(&stop_signal, &print_mutex);
	actions = init_actions(&args, philosophers, &stop_signal, &print_mutex);
	i = -1;
	while (++i < args.num_philosophers)
	{
		if (pthread_create(&(philosophers[i]->philo_thread), NULL,
			philosopher_actions, &actions[i]) < 0)
			error_exit_thread();
	}
	check_death_main(args, &stop_signal, actions);
	join_philosophers_threads(args.num_philosophers, philosophers);
	free(actions);
	pthread_mutex_destroy(&(stop_signal.mutex));
	pthread_mutex_destroy(&(print_mutex));
}
