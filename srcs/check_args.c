/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:52:01 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 20:52:30 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_args	initialize_args(int argc, char **argv) 
{
	t_args	args;

	args.num_philosophers = ft_atoul(argv[1]);
	args.time_to_die = ft_atoul(argv[2]);
	args.time_to_eat = ft_atoul(argv[3]);
	args.time_to_sleep = ft_atoul(argv[4]);
	if (argc == 6)
		args.num_eat = (long) ft_atoul(argv[5]);
	else
		args.num_eat = -1;
	return args;
}

int	not_enough_args(void)
{
	ft_putstr_fd(RED_ERROR, 2);
	ft_putstr_fd("ERROR.", 2);
	ft_putstr_fd(LIGHT_RED, 2);
	ft_putstr_fd("Can't execute program. You need", 2);
	ft_putstr_fd(" to give at least 5 and as max 6 arguments to", 2);
	ft_putstr_fd(" start the program\n", 2);
	ft_putstr_fd(LIGHT_BLUE, 2);
	ft_putstr_fd("Arguments to give:\n", 2);
	ft_putstr_fd("./philo number_of_philosophers | time_to_die | time_to_eat |", 2);
	ft_putstr_fd(" time_to_sleep | [number_of_times_each_philosopher_must_eat]\n", 2);
	ft_putstr_fd(COLOR_RESET, 2);
	return (0);
}

int	check_args(t_args args)
{
	if ((long) args.num_eat < -1 || args.num_eat == 0)
		return (0);
	else if (args.num_philosophers == 0)
		return (0);
	else if (args.time_to_die == 0)
		return (0);
	else if (args.time_to_eat == 0)
		return (0);
	else if (args.time_to_sleep == 0)
		return (0);
	else
		return (1);
}

int	exit_bad_args(void)
{
	ft_putstr_fd(RED_ERROR, 2);
	ft_putstr_fd("ERROR. ", 2);
	ft_putstr_fd(LIGHT_RED, 2);
	ft_putstr_fd("Can't execute program. You need", 2);
	ft_putstr_fd(" to give positive int arguments.\n", 2);
	ft_putstr_fd(" You can't start the program with negative arguments\n", 2);
	ft_putstr_fd(COLOR_RESET, 2);
	return (1);
}
