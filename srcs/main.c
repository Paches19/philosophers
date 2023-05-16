/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:46:48 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 20:52:14 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int argc, char **argv)
{
	t_args			args;
	t_fork			**forks;
	t_philosopher	**philosophers;

	if (argc < 5 || argc > 6)
		return (not_enough_args());
	args = initialize_args(argc, argv);
	if (!check_args(args))
		return (exit_bad_args());
	else if (args.num_philosophers == 1)
	{
		printf("Time: 0 \tphilosopher: 1 has taken a fork 🍴\n");
		printf("Time: %ld \tphilosopher: 1 died 💀\n", args.time_to_die);
		return (0);
	}
	else
	{
		forks = init_forks(args.num_philosophers);
		philosophers = init_philosophers(args.num_philosophers, forks);
		start_eating(args, philosophers);
		free_structs(forks, philosophers, args.num_philosophers);
	}
	return (0);
}
