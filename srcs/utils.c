/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:22:49 by adpachec          #+#    #+#             */
/*   Updated: 2023/05/11 14:19:46 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	error_exit_malloc(void)
{
	ft_putstr_fd("ERROR: Memory allocation failed\n", 2);
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

static void	error_exit_ulong(void)
{
	ft_putstr_fd("Arg out of range\n", 2);
	ft_putstr_fd("You MUST use unsigned long as any argument\n", 2);
	exit(1);
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
			sign = (-1);
		++str;
	}
	if (sign == -1)
		return (0);
	result = 0;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		result = (result * 10) + (*str - '0') * sign;
		if (result > ULONG_MAX)
			error_exit_ulong();
		++str;
	}
	return (result);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *) s;
	i = -1;
	while (++i < n && n > 0)
		ptr[i] = 0;
}

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	const size_t	len = count * size;

	if (!count)
		return (malloc(0));
	if (len / count != size)
		return (NULL);
	ptr = (void *) malloc(len);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, len);
	return (ptr);
}
