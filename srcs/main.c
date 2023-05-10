/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:46:48 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/10 13:22:58 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_args	initialize_args(int argc, char **argv) 
{
	t_args	args;
	struct timeval	current_timeval;

	args.num_philosophers = ft_atoul(argv[1]);
	args.time_to_die = ft_atoul(argv[2]);
	args.time_to_eat = ft_atoul(argv[3]);
	args.time_to_sleep = ft_atoul(argv[4]);
	gettimeofday(&current_timeval, NULL);
	args.time_init_prog = current_timeval.tv_sec * 1000 +
		current_timeval.tv_usec / 1000;
	args.num_eat = -1; 
	if (argc == 6)
		args.num_eat = ft_atoul(argv[5]);
	else
		args.num_eat = -1;
	return args;
}

int	main(int argc, char **argv)
{
	t_args			args;
	t_fork			**forks;
	t_philosopher	**philosophers;

	args = initialize_args(argc, argv);
	forks = init_forks(args.num_philosophers);
	philosophers = init_philosophers(args.num_philosophers, forks);
	start_eating(args, forks, philosophers);
	free_structs(forks, philosophers, args.num_philosophers);
	return (0);
	
}
