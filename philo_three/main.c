/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-feli <ade-feli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:40:20 by ade-feli          #+#    #+#             */
/*   Updated: 2021/04/26 13:20:41 by ade-feli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	metaphysic(void *philo)
{
	t_philo	*p;
	int		start;

	start = 1;
	p = (t_philo *)philo;
	while (p->times != 0)
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

int	starvation(t_philo *p)
{
	while (1)
	{
		sem_wait(p->alive);
		if (get_time() - p->last_eaten > (unsigned long)p->die)
		{
			sem_wait(p->write);
			printf("%lu %d died\n", timestamp(p), p->id);
			exit(1);
		}
		sem_post(p->alive);
		if (p->times == 0)
			exit(0);
		usleep(500);
	}
	return (0);
}

void	p_eat(t_philo *p)
{
	sem_wait(p->write);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d is eating\n", timestamp(p), p->id);
	sem_post(p->write);
	p->last_eaten = get_time();
	sem_post(p->alive);
	zsleep(p->eat);
}

void	end(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->num)
	{
		sem_close(t->philos[i]->alive);
		sem_unlink("/s_alive");
		free(t->philos[i++]);
	}

	sem_close(t->write);
	sem_close(t->forks);
	sem_close(t->tunnel);
	sem_unlink("/s_forks");
	sem_unlink("/s_write");
	sem_unlink("/s_tunnel");
	free(t->philos);
	free(t);
}

int	main(int argc, char const *argv[])
{
	t_table	*t;
	int		i;
	int		status;
	int		returned;

	t = init(argc, argv);
	if (t == 0 || genesys(t))
	{
		printf("Error.\n");
		return (1);
	}
	waitpid(-1, &status, 0);
	returned = 0;
	if (WIFEXITED(status))
		returned = WEXITSTATUS(status);
	i = 0;
	if (returned == 1)
		while (i < t->num)
			kill(t->philos[i++]->pid, SIGINT);
	else
		while (waitpid(-1, 0, 0) != -1 || errno != ECHILD)
			;
	end(t);
	return (0);
}
