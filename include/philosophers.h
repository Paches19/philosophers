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

# define RED_ERROR "\033[1;31m"
# define LIGHT_RED "\033[1;91m"
# define BLUE "\033[1;94m"
# define LIGHT_BLUE "\033[1;96m"
# define COLOR_RESET "\033[0m"

typedef enum e_state
{
	TO_EAT,
	EATING,
	TO_SLEEP,
	SLEEPING,
	TO_THINK,
	THINKING
}	t_state;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
}					t_fork;

typedef struct s_philosopher
{
	int				id;
	long			num_eat;
	unsigned long	last_time_eat;
	unsigned long	init_state;
	pthread_t		philo_thread;
	t_fork			*left_fork;
	int				l_fork_taken;
	t_fork			*right_fork;
	int				r_fork_taken;
	t_state			state;
}					t_philosopher;

typedef struct s_stop
{
	int				stop;
	int				end_eat;
	pthread_mutex_t	mutex;
}					t_stop;

typedef struct s_args
{
	unsigned long	num_philosophers;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	long			num_eat;
	unsigned long	time_init_prog;
}					t_args;

typedef struct s_actions
{
	t_philosopher	*philos;
	t_args			args;
	t_stop			*stop;
	pthread_mutex_t	print_mutex;
}					t_actions;

// ******************************* check_args **********************************
t_args			initialize_args(int argc, char **argv);
int				not_enough_args(void);
int				check_args(t_args args);
int				exit_bad_args(void);
// ******************************* utils ***************************************
void			error_exit_malloc(void);
void			error_exit_mutex(void);
int				ft_strcmp(const char *s1, const char *s2);
void			*ft_calloc(size_t count, size_t size);
// ******************************* not_libft ***********************************
unsigned long	ft_atoul(const char *str);
void			ft_putstr_fd(char *s, int fd);
void			error_exit_thread(void);
void			ft_putnbr_fd(unsigned long n, int fd);
// ******************************* free_structs ********************************
void			free_structs(t_fork **forks, t_philosopher **philos,
					unsigned long num_philo);
// ******************************* init_forks **********************************
t_fork			**init_forks(unsigned long num_forks);
// ******************************* init_philos *********************************
t_philosopher	**init_philosophers(unsigned long num_philosophers,
					t_fork **forks);
// ******************************* start_eating ********************************
void			start_eating(t_args args, t_philosopher **philosophers);
void			print_log(t_actions *actions, int philos_id, long time_init,
					char *msg);
// ******************************* check_death *********************************
void			join_philosophers_threads(int num_philosophers,
					t_philosopher **philosophers);
int				end_eat_times(t_actions *actions);
int				is_philo_alive(t_actions *actions);
void			print_death(t_actions *actions, int philos_id, long time_init,
					char *msg);
void			is_philo_alive_main(t_actions *actions, int *stop,
					t_stop *stop_signal);
// ******************************* eat *****************************************
void			take_forks(t_actions *actions);
void			eat(t_actions *actions);
// ******************************* sleep_think *********************************
void			think_philo(t_actions *actions);
void			sleep_philo(t_actions *actions);

#endif
