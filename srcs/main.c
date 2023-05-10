/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:46:48 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 12:28:24 by adpachec         ###   ########.fr       */
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
	args.num_eat = -1; 
	if (argc == 6)
		args.num_eat = ft_atoul(argv[5]);
	else
		args.num_eat = -1;
	// printf("num_philo: %d\n", args.num_philosophers);
	// printf("time_to_die: %d\n", args.time_to_die);
	// printf("time_to_eat: %d\n", args.time_to_eat);
	// printf("time_to_sleep: %d\n", args.time_to_sleep);
	// if (argc == 6)
	// 	printf("num_eat: %d\n", args.num_eat);
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
	free_structs(forks, philosophers);
	return (0);
	
}
