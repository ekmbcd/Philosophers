/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-feli <ade-feli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:06:08 by ade-feli          #+#    #+#             */
/*   Updated: 2021/04/26 11:06:10 by ade-feli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int			res;
	const char	*pointer;

	res = 0;
	pointer = str;
	while (*pointer)
	{
		if (*pointer < '0' || *pointer > '9')
			return (-1);
		if (res > 0 && res * 10 + (*str - '0') < 0)
			return (-1);
		res = (res * 10) + (*pointer - '0');
		pointer++;
	}
	return (res);
}

unsigned long	timestamp(t_philo *p)
{
	unsigned long	now;
	unsigned long	relative;

	now = get_time();
	relative = now - p->start_time;
	return (relative);
}

unsigned long	get_time(void)
{
	unsigned long	time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	zsleep(unsigned long micro)
{
	unsigned long	target;
	unsigned long	start;

	start = get_time();
	target = start + micro;
	while (start < target)
	{
		start = get_time();
		if (start >= target)
			return ;
		usleep(((target - start) >> 1) * 1000);
	}
}
