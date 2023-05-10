/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:04:37 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/10 13:33:52 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	free_structs(t_fork **forks, t_philosopher **philos,
	unsigned long num_philo)
{
	unsigned long	i;

	i = -1;
	while (++i < num_philo)
		free(philos[i]);
	free(philos);
	i = -1;
	while (++i < num_philo && forks[i])
	{
		pthread_mutex_destroy(&(*forks)[i].mutex);
		free(forks[i]);
	}
	free(forks);
}
