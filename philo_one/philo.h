/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-feli <ade-feli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:02:01 by ade-feli          #+#    #+#             */
/*   Updated: 2021/04/26 11:04:46 by ade-feli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	unsigned long	last_eaten;
	int				left;
	int				right;
	unsigned long	start_time;
	int				eat;
	int				sleep;
	int				times;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	*m_write;
	pthread_mutex_t	alive;
}				t_philo;

typedef struct s_table
{
	int				num;
	unsigned long	start_time;
	int				die;
	int				eat;
	int				sleep;
	int				times;
	t_philo			**philos;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	m_write;
}				t_table;

int				ft_atoi(const char *str);
unsigned long	get_time(void);
unsigned long	timestamp(t_philo *p);
void			p_sleep(t_philo *p);
void			p_eat (t_philo *p);
void			zsleep(unsigned long micro);
int				genesys(t_table *t);
t_table			*init(int ac, const char **av);
void			*metaphysic(void *philo);

#endif
