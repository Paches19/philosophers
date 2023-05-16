/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 20:27:36 by adpachec         ###   ########.fr       */
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
   			WIDTH_TIME, current_time, WIDTH_PHILOSOPHER, philos_id, WIDTH_MESSAGE,
			msg, WIDTH_MEALS, actions->philos->num_eat + 1);
		else
			printf("Time: %-*ld philosopher: %-*d %-*s meals: %-*ld\n",
   			WIDTH_TIME, current_time, WIDTH_PHILOSOPHER, philos_id, WIDTH_MESSAGE,
			msg, WIDTH_MEALS, actions->philos->num_eat);
		pthread_mutex_unlock(&(actions->print_mutex));
	}
}

void	eat(t_actions *actions)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	if (actions->stop->stop)
		return ;
	else if (actions->philos->state == TO_EAT)
	{
		actions->philos->state = EATING;
		actions->philos->last_time_eat = current_time;
		actions->philos->init_state = current_time;
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "is eating 🍝");
	}
	else if (actions->philos->state == EATING && current_time - actions->philos->init_state >= actions->args.time_to_eat)
	{
		actions->philos->num_eat++;
		actions->philos->state = TO_SLEEP;
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
	}
}

void	take_forks(t_actions *actions)
{
	if (actions->stop->stop)
		return ;
	else if (actions->philos->id % 2 == 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		usleep(50);
		if (!actions->stop->stop)
		{
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
			pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		}
		usleep(50);
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
	}
	else if (actions->philos->id % 2 != 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		usleep(50);
		if (!actions->stop->stop)
		{
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
			pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		}
		usleep(50);
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
	}
	eat(actions);
}

void	sleep_philo(t_actions *actions)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	if (actions->stop->stop)
		return ;
	else if (actions->philos->state == TO_SLEEP)
	{
		actions->philos->init_state = current_time;
		actions->philos->state = SLEEPING;
		pthread_mutex_lock(&(actions->stop->mutex));
		// if (!actions->stop->stop)
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "is sleeping 💤");
		pthread_mutex_unlock(&(actions->stop->mutex));
	}
	else if (actions->philos->state == SLEEPING && current_time - actions->philos->init_state >= actions->args.time_to_sleep)
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
		// if (!actions->stop->stop)
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "is thinking 💭");
		pthread_mutex_unlock(&(actions->stop->mutex));
	}
	else if (actions->philos->state == THINKING)
		actions->philos->state = TO_EAT;
}

int	is_philo_alive(t_actions *actions)
{
	struct timeval	current_timeval;
	unsigned long	current_time;

	gettimeofday(&current_timeval, NULL);
	current_time = current_timeval.tv_sec * 1000 + current_timeval.tv_usec / 1000;
	pthread_mutex_lock(&(actions->stop->mutex));
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		pthread_mutex_unlock(&(actions->stop->mutex));
		return (0);
	}
	else if ((current_time - actions->philos->last_time_eat)
		> actions->args.time_to_die && actions->philos->state != EATING)
	{
		actions->stop->stop = 1;
		pthread_mutex_unlock(&(actions->stop->mutex));
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "died 💀");
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		return (0);
	}
	pthread_mutex_unlock(&(actions->stop->mutex));
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

void start_eating(t_args args, t_fork **forks, t_philosopher **philosophers)
{
	unsigned long	i;
	t_actions		*actions;
	t_stop			stop_signal;
	pthread_mutex_t	print_mutex;
	struct timeval	current_time;

	(void)forks;
	actions = (t_actions *)ft_calloc(sizeof(t_actions), args.num_philosophers + 1);
	if (!actions)
		exit(EXIT_FAILURE);
	stop_signal.stop = 0;
	pthread_mutex_init(&(stop_signal.mutex), NULL);
	pthread_mutex_init(&(print_mutex), NULL);
	gettimeofday(&current_time, NULL);
	args.time_init_prog = current_time.tv_sec * 1000 +
	current_time.tv_usec / 1000;
	i = -1;
	while (++i < args.num_philosophers)
	{
		actions[i].args = args;
		actions[i].philos = philosophers[i];
		actions[i].stop = &stop_signal;
		actions[i].print_mutex = print_mutex;
		pthread_create(&(philosophers[i]->philo_thread), NULL,
		philosopher_actions, &actions[i]);
	}
	join_philosophers_threads(args.num_philosophers, philosophers);
	free(actions);
	pthread_mutex_destroy(&(stop_signal.mutex));
	pthread_mutex_destroy(&(print_mutex));
}
