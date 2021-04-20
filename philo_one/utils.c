#include "philo.h"

int ft_atoi(const char *str)
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

unsigned long timestamp(t_philo *p)
{
	unsigned long now;
	unsigned long relative;

	now = get_time();
	relative = now - p->start_time;
	//printf("%lu ", relative);
	return (relative);
}

void p_sleep(t_philo *p)
{
	pthread_mutex_lock(p->m_write);
	printf("%lu %d is sleeping\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);

	usleep(p->sleep * 1000);

	pthread_mutex_lock(p->m_write);
	printf("%lu %d is thinking\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);
}

void p_eat (t_philo *p)
{
	p->forks[p->right] = 1;
	p->forks[p->left] = 1;
	// pthread_mutex_lock(&p->m_forks[p->right]);
	// pthread_mutex_lock(&p->m_forks[p->left]);

	pthread_mutex_unlock(p->m_ego);

	pthread_mutex_lock(p->m_write);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d is eating\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);
	p->last_eaten = get_time();
	usleep(p->eat * 1000);
	pthread_mutex_lock(p->m_ego);
	p->forks[p->right] = 0;
	p->forks[p->left] = 0;
	// pthread_mutex_unlock(&p->m_forks[p->right]);
	// pthread_mutex_unlock(&p->m_forks[p->left]);
	pthread_mutex_unlock(p->m_ego);
}

unsigned long get_time(void)
{
	time_t seconds;
	suseconds_t micros;
	unsigned long time;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	//printf("time = %lu\n", time);
	return(time);
}
