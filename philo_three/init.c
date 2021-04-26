/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-feli <ade-feli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:47:02 by ade-feli          #+#    #+#             */
/*   Updated: 2021/04/26 11:47:17 by ade-feli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	generate_philos(t_table *t)
{
	int	i;

	t->philos = malloc(sizeof(t_philo *) * t->num);
	if (!t->philos)
		return (1);
	i = -1;
	while (++i < t->num)
	{
		t->philos[i] = malloc(sizeof(t_philo));
		if (!t->philos[i])
			return (1);
		t->philos[i]->id = i + 1;
		t->philos[i]->last_eaten = t->start_time;
		t->philos[i]->start_time = t->start_time;
		t->philos[i]->eat = t->eat;
		t->philos[i]->sleep = t->sleep;
		t->philos[i]->die = t->die;
		t->philos[i]->times = t->times;
		t->philos[i]->forks = t->forks;
		t->philos[i]->write = t->write;
		t->philos[i]->tunnel = t->tunnel;
		sem_unlink("/s_alive");
		t->philos[i]->alive = sem_open("/s_alive", O_CREAT, 01411, 1);
	}
	return (0);
}

int	genesys(t_table *t)
{
	int			i;
	pthread_t	thread;

	i = 0;
	while (i < t->num)
	{
		t->philos[i]->pid = fork();
		if (t->philos[i]->pid == 0)
		{
			if (pthread_create(&thread, NULL, (void *)starvation,
					(void *)t->philos[i]))
				return (1);
			pthread_detach(thread);
			metaphysic(t->philos[i]);
			exit(42);
		}
		i++;
	}
	return (0);
}

int	init_semaphores(t_table *t)
{
	sem_unlink("/s_forks");
	sem_unlink("/s_write");
	sem_unlink("/s_tunnel");
	t->forks = sem_open("/s_forks", O_CREAT, 01411, t->num);
	t->write = sem_open("/s_write", O_CREAT, 01411, 1);
	t->tunnel = sem_open("/s_tunnel", O_CREAT, 01411, 1);
	if (!t->forks || !t->write || !t->tunnel)
		return (1);
	return (0);
}

t_table	*init(int ac, const char **av)
{
	t_table	*t;

	if (ac < 5 || ac > 6)
		return (0);
	t = malloc(sizeof(t_table));
	if (!t)
		return (0);
	t->num = ft_atoi(av[1]);
	t->die = ft_atoi(av[2]);
	t->eat = ft_atoi(av[3]);
	t->sleep = ft_atoi(av[4]);
	if (ac == 6)
		t->times = ft_atoi(av[5]);
	else
		t->times = -2;
	if (t->num <= 0 || t->die <= 0 || t->eat <= 0
		|| t->sleep <= 0 || t->times == -1)
	{
		free(t);
		return (0);
	}
	t->start_time = get_time();
	if (init_semaphores(t) || generate_philos(t))
		return (0);
	return (t);
}
