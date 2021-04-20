#include "philo.h"

void *metaphysic(void *philo)
{
	t_philo *p;

	p = (t_philo *)philo;
	while (1)
	{
		//check if alive
		//check if left fork is free //check if right fork is free
		pthread_mutex_lock(p->m_ego);
		if (p->forks[p->right] == 0 && p->forks[p->left] == 0)
		{
			//start eating
			p_eat(p);

			//sleep
			p_sleep(p);
		}
		else
			pthread_mutex_unlock(p->m_ego);
		//stop after p.times eaten
	}

}

void generate_philos(t_table *t)
{
	//printf("%d %d %d %d\n", t->num, t->die, t->eat, t->sleep);
	int i;

	t->philos = malloc(sizeof(t_philo *) * t->num);
	i = -1;
	while (++i < t->num)
	{
		t->philos[i] = malloc(sizeof(t_philo));
		t->philos[i]->id = i + 1;
		t->philos[i]->last_eaten = t->start_time;
		t->philos[i]->right = i;
		t->philos[i]->left = i + 1;
		if (i == t->num - 1)
			t->philos[i]->left = 0;
		t->philos[i]->start_time = t->start_time;
		t->philos[i]->eat = t->eat;
		t->philos[i]->sleep = t->sleep;
		t->philos[i]->times = t->times;
		t->philos[i]->m_forks = t->m_forks;
		t->philos[i]->forks = t->forks;
		t->philos[i]->m_ego = &t->m_ego;
		t->philos[i]->m_write = &t->m_write;
	}
}

void create_forks(t_table *t)
{
	int i;

	i = 0;
	t->forks = malloc(sizeof(char) * t->num);
	t->m_forks = malloc(sizeof(pthread_mutex_t) * t->num);
	while (i < t->num)
	{
		t->forks[i] = 0;
		i++;
	}
}

pthread_t *genesys(t_table *t)
{
	int i;
	pthread_t *threads;

	i = 0;
	threads = malloc(sizeof(pthread_t) * t->num);
	while (i < t->num)
	{
		pthread_create(&threads[i], NULL, metaphysic, (void *)t->philos[i]);
		i++;
	}
	return (threads);
}

t_table *init(int ac, const char **av)
{
	t_table *t;

	if (ac < 5 || ac > 6)
		return (0);
	t = malloc(sizeof(t_table));
	t->num = ft_atoi(av[1]);
	t->die = ft_atoi(av[2]);
	t->eat = ft_atoi(av[3]);
	t->sleep = ft_atoi(av[4]);
	if (ac == 6)
		t->times = ft_atoi(av[5]);
	else
		t->times = 0;
	if (t->num <= 0 || t->die < 0 || t->eat < 0 || t->sleep < 0 || t->times < 0)
	{
		free(t);
		return (0);
	}
	t->start_time = get_time();
	create_forks(t);
	generate_philos(t);
	return (t);

}

int starvation(t_table *t)
{
	int i;

	while (1)
	{
		i = 0;
		while (i < t->num)
		{
			if (get_time() - t->philos[i]->last_eaten > t->die)
			{
				printf("%lu %d died.\n", timestamp(t->philos[i]), i + 1);
				return (1);
			}
			i++;
		}
	}
	return(0);
}

int main(int argc, char const *argv[])
{
	t_table *t;
	pthread_t *threads;

	t = init(argc, argv);
	if (t == 0)
	{
		printf("Wrong input.\n");
		return(1);
	}

	//start threads
	threads = genesys(t);

	//check aliveness
	starvation(t);
	//join?
	//pthread_join(threads[0], NULL);
	//free

	return(0);
}
