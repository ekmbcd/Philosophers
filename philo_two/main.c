/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-feli <ade-feli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:23:36 by ade-feli          #+#    #+#             */
/*   Updated: 2021/04/26 12:56:57 by ade-feli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*metaphysic(void *philo)
{
	t_philo	*p;
	int		start;

	start = 1;
	p = (t_philo *)philo;
	while (p->times != 0 && p->id != 0)
	{
		sem_wait(p->tunnel);
		sem_wait(p->forks);
		sem_wait(p->forks);
		sem_post(p->tunnel);
		sem_wait(p->alive);
		p_eat(p);
		sem_post(p->forks);
		sem_post(p->forks);
		p_sleep(p);
		(p->times)--;
		start = 0;
	}
	return (0);
}

void	kill_all(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->num)
	{
		t->philos[i]->id = 0;
		i++;
	}
}

int	starvation(t_table *t)
{
	int				i;
	int				done;

	while (1)
	{
		i = -1;
		done = 0;
		while (++i < t->num)
		{
			if (t->philos[i]->times == 0)
				continue ;
			done++;
			sem_wait(t->philos[i]->alive);
			if (get_time() - t->philos[i]->last_eaten > (unsigned long)t->die)
			{
				sem_wait(t->write);
				printf("%lu %d died\n", timestamp(t->philos[i]), i + 1);
				return (1);
			}
			sem_post(t->philos[i]->alive);
		}
		if (done == 0)
			return (0);
	}
	return (0);
}

void	end(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->num)
	{
		sem_close(t->philos[i++]->alive);
		sem_unlink("/s_alive");
	}
	while (--i)
		sem_post(t->forks);
	sem_close(t->forks);
	sem_close(t->tunnel);
	sem_close(t->write);
	sem_unlink("/s_forks");
	sem_unlink("/s_write");
	sem_unlink("/s_tunnel");
	while (i < t->num)
		free(t->philos[i++]);
	free(t->philos);
	free(t);
}

int	main(int argc, char const *argv[])
{
	t_table	*t;
	int		i;

	t = init(argc, argv);
	if (t == 0 || genesys(t))
	{
		printf("Error.\n");
		return (1);
	}
	if (starvation(t))
	{
		kill_all(t);
		i = -1;
		while (++i < t->num)
		{
			sem_post(t->philos[i]->alive);
			sem_post(t->forks);
			sem_post(t->forks);
			sem_post(t->write);
			sem_post(t->tunnel);
		}
	}
	end(t);
	return (0);
}
