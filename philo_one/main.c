#include "philo.h"

void p_sleep(t_philo *p)
{
	pthread_mutex_lock(p->m_write);
	printf("%lu %d is sleeping\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);
	zsleep(p->sleep);
	pthread_mutex_lock(p->m_write);
	printf("%lu %d is thinking\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);
}

void p_eat (t_philo *p)
{
	pthread_mutex_lock(p->m_write);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d is eating\n", timestamp(p), p->id);
	pthread_mutex_unlock(p->m_write);
	p->last_eaten = get_time();
	pthread_mutex_unlock(&(p->alive));
	zsleep(p->eat);
}

void *metaphysic(void *philo)
{
	t_philo *p;
	int start;

	start = 1;
	p = (t_philo *)philo;
	while (p->times != 0)
	{
		if(start && p->id % 2 == 0)
			usleep((p->eat) * 500);
		pthread_mutex_lock(&(p->m_forks[p->left]));
		pthread_mutex_lock(&(p->m_forks[p->right]));
		pthread_mutex_lock(&(p->alive));
		p_eat(p);
		pthread_mutex_unlock(&(p->m_forks[p->left]));
		pthread_mutex_unlock(&(p->m_forks[p->right]));
		p_sleep(p);
		(p->times)--;
		start = 0;
	}
	return(0);
}


int starvation(t_table *t)
{
	int i;
	int done;

	while (1)
	{
		i = -1;
		done = 0;
		while (++i < t->num)
		{
			if (t->philos[i]->times == 0)
				continue;
			done++;
			pthread_mutex_lock(&(t->philos[i]->alive));
			if (get_time() - t->philos[i]->last_eaten > (unsigned long)t->die)
			{
				pthread_mutex_lock(&(t->m_write));
				printf("%lu %d died\n", timestamp(t->philos[i]), i + 1);
				return (1);
			}
			pthread_mutex_unlock(&(t->philos[i]->alive));
		}
		if (done == 0)
			return (0);
	}
}

int main(int argc, char const *argv[])
{
	t_table *t;
	int i;

	t = init(argc, argv);
	if (t == 0 || genesys(t))
	{
		printf("Error.\n");
		return(1);
	}
	starvation(t);
	i = 0;
	while (i < t->num)
	{
		pthread_mutex_destroy(&t->philos[i]->alive);
		free(t->philos[i]);
		pthread_mutex_destroy(&t->m_forks[i++]);
	}
	pthread_mutex_destroy(&t->m_write);
	free(t->philos);
	free(t->m_forks);
	free(t);
	return(0);
}
