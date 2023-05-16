/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_libft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:06:34 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/16 21:13:58 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	error_exit_thread(void)
{
	ft_putstr_fd("ERROR: Thread init failed\n", 2);
	exit(1);
}

static int	ft_isdigit(int arg)
{
	if (arg >= '0' && arg <= '9')
		return (arg);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	int		i;

	if (s)
	{
		i = -1;
		while (s[++i])
			write(fd, &s[i], 1);
	}
}

unsigned long	ft_atoul(const char *str)
{
	int				sign;
	unsigned long	result;

	if (!str)
		return (0);
	sign = 1;
	while (*str && (*str == 32 || (*str >= 9 && *str <= 13)))
		++str;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			return (0);
		++str;
	}	
	result = 0;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		result = (result * 10) + (*str - '0') * sign;
		++str;
	}
	return (result);
}
