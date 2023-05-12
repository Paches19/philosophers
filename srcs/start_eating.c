/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:11:14 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/12 11:03:28 by adpachec         ###   ########.fr       */
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
	pthread_mutex_lock(&(actions->print_mutex));
	printf("Time: %ld \tphilosopher: %d %s\n",
		current_time, philos_id, msg);
	pthread_mutex_unlock(&(actions->print_mutex));
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
		// pthread_mutex_lock(&(actions->print_mutex));
		// printf("philo: %d init_state EAT: %ld\n", actions->philos->id, actions->philos->init_state);
		// pthread_mutex_unlock(&(actions->print_mutex));
		actions->philos->num_eat++;
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "is eating 🍝");
	}
	else if (actions->philos->state == EATING && current_time - actions->philos->init_state >= actions->args.time_to_eat)
	{
		// pthread_mutex_lock(&(actions->print_mutex));
		// printf("philo: %d c_t finsih EAT: %ld\n", actions->philos->id, current_time);
		// printf("Time eating: %ld\n", actions->philos->init_state - current_time);
		// pthread_mutex_unlock(&(actions->print_mutex));
		actions->philos->state = TO_SLEEP;
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
	}
	// else if (actions->philos->state == EATING)
	// 	usleep(50);
}

void	take_forks(t_actions *actions)
{
	if (actions->stop->stop)
		return ;
	else if (actions->philos->id % 2 == 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		if (!actions->stop->stop)
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
		if (!actions->stop->stop)
			pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		if (!actions->stop->stop)
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
	}
	else if (actions->philos->id % 2 != 0 && actions->philos->state == TO_EAT)
	{
		pthread_mutex_lock(&(actions->philos->left_fork->mutex));
		if (!actions->stop->stop)
			print_log(actions, actions->philos->id, actions->args.time_init_prog, "has taken a fork 🍴");
		if (!actions->stop->stop)
			pthread_mutex_lock(&(actions->philos->right_fork->mutex));
		if (!actions->stop->stop)
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
		// pthread_mutex_lock(&(actions->print_mutex));
		// printf("philo: %d init_state SLEEP: %ld\n", actions->philos->id, actions->philos->init_state);
		// pthread_mutex_unlock(&(actions->print_mutex));
		actions->philos->state = SLEEPING;
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "is sleeping 💤");
	}
	else if (actions->philos->state == SLEEPING && current_time - actions->philos->init_state >= actions->args.time_to_sleep)
	{
		// pthread_mutex_lock(&(actions->print_mutex));
		// printf("philo: %d c_t finsih SLEEP: %ld\n", actions->philos->id, current_time);
		// printf("Time sleeping: %ld\n", actions->philos->init_state - current_time);
		// pthread_mutex_unlock(&(actions->print_mutex));
		actions->philos->state = TO_THINK;
	}
	// else if (actions->philos->state == SLEEPING)
	// 	usleep(50);
}

void	think_philo(t_actions *actions)
{
	if (actions->stop->stop)
		return ;
	else if (actions->philos->state == TO_THINK)
	{
		actions->philos->state = THINKING;
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "is thinking 💭");
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
	if (actions->stop->stop)
	{
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
		return (0);	
	}
	else if ((current_time - actions->philos->last_time_eat)
		> actions->args.time_to_die && actions->philos->state != EATING)
	{
		print_log(actions, actions->philos->id, actions->args.time_init_prog, "died 💀");
		pthread_mutex_lock(&(actions->stop->mutex));
		actions->stop->stop = 1;
		pthread_mutex_unlock(&(actions->stop->mutex));
		pthread_mutex_unlock(&(actions->philos->left_fork->mutex));
		pthread_mutex_unlock(&(actions->philos->right_fork->mutex));
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

void	print_state(int id, int state)
{
	if (state == 0)
		printf("philo: %d state: TO_EAT\n", id);
	else if (state == 1)
		printf("philo: %d state: EATING\n", id);
	else if (state == 2)
		printf("philo: %d state: TO_SLEEP\n", id);
	else if (state == 3)
		printf("philo: %d state: SLEEPING\n", id);
	else if (state == 4)
		printf("philo: %d state: TO_THINK\n", id);
	else if (state == 5)
		printf("philo: %d state: THINKING\n", id);
}

static void	*philosopher_actions(void *arg)
{
	t_actions	*actions;

	actions = (t_actions *)arg;
	while (1)
	{
		// pthread_mutex_lock(&(actions->print_mutex));
		// print_state(actions->philos->id, actions->philos->state);
		// pthread_mutex_unlock(&(actions->print_mutex));
		if (actions->stop->stop)
			return (NULL);
		take_forks(actions);
		sleep_philo(actions);
		think_philo(actions);
		if (!is_philo_alive(actions))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}

void start_eating(t_args args, t_fork **forks, t_philosopher **philosophers)
{
	unsigned long	i;
	t_actions		*actions;
	t_stop			stop_signal;
	pthread_mutex_t	print_mutex;

	(void)forks;
	actions = (t_actions *)ft_calloc(sizeof(t_actions), args.num_philosophers + 1);
	if (!actions)
		exit(EXIT_FAILURE);
	stop_signal.stop = 0;
	pthread_mutex_init(&(stop_signal.mutex), NULL);
	pthread_mutex_init(&(print_mutex), NULL);
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
