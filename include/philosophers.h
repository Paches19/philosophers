/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:46:45 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 11:16:31 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

typedef struct	s_fork 
{
	int				id;
	pthread_mutex_t	mutex;
}					t_fork;

typedef struct	s_philosopher 
{
	int				id;
	t_fork			*left_fork;
	t_fork			*right_fork;
	int				num_eat;
	pthread_t		philo_thread;
	unsigned long	last_time_eat;
}				t_philosopher;

typedef struct	s_args
{
	unsigned long	num_philosophers;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	num_eat;
}		t_args;

// ******************************* utils **************************************
unsigned long	ft_atoul(const char *str);
void			ft_putstr_fd(char *s, int fd);
void			error_exit_malloc(void);
// ******************************* free_structs *******************************
void			free_structs(t_fork **forks, t_philosopher **philosophers);
// ******************************* init_forks *********************************
t_fork			**init_forks(int num_forks);
// ******************************* init_philos ********************************
t_philosopher	**init_philosophers(int num_philosophers, t_fork **forks);
// ******************************* start_eating ********************************
void			start_eating(t_args args, t_fork **forks,
	t_philosopher **philosophers);

#endif
