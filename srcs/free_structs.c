/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:04:37 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/09 12:06:45 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	free_structs(t_fork **forks, t_philosopher **philosophers)
{
	int	i;

	i = -1;
	while (forks[++i])
	{
		pthread_mutex_destroy(&(*forks)[i].mutex);
		free(forks[i]);
	}
	free(forks);
	i = -1;
	while (philosophers[++i])
		free(philosophers[i]);
	free(philosophers);
}