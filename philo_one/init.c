#include "philo.h"

int generate_philos(t_table *t)
{
	int i;

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
		t->philos[i]->right = i;
		t->philos[i]->left = i + 1;
		if (i == t->num - 1)
			t->philos[i]->left = 0;
		t->philos[i]->start_time = t->start_time;
		t->philos[i]->eat = t->eat;
		t->philos[i]->sleep = t->sleep;
		t->philos[i]->times = t->times;
		t->philos[i]->m_forks = t->m_forks;
		t->philos[i]->m_write = &t->m_write;
		pthread_mutex_init(&t->philos[i]->alive, NULL);
	}
	return (0);
}

int create_forks(t_table *t)
{
	int i;

	i = 0;
	t->m_forks = malloc(sizeof(pthread_mutex_t) * t->num);
	if (!t->m_forks)
		return (1);
	pthread_mutex_init(&t->m_write, NULL);
	while (i < t->num)
	{
		pthread_mutex_init(&t->m_forks[i], NULL);
		i++;
	}
	return(0);
}

int genesys(t_table *t)
{
	int i;
	pthread_t *threads;

	i = 0;
	threads = malloc(sizeof(pthread_t) * t->num);
	if (!threads)
		return (1);
	while (i < t->num)
	{
		if (pthread_create(&threads[i], NULL, metaphysic, (void *)t->philos[i]))
			return (1);
		pthread_detach(threads[i]);
		i++;
	}
	free(threads);
	return (0);
}

t_table *init(int ac, const char **av)
{
	t_table *t;

	if (ac < 5 || ac > 6)
		return (0);
	t = malloc(sizeof(t_table));
	if (!t)
		return(0);
	t->num = ft_atoi(av[1]);
	t->die = ft_atoi(av[2]);
	t->eat = ft_atoi(av[3]);
	t->sleep = ft_atoi(av[4]);
	if (ac == 6)
		t->times = ft_atoi(av[5]);
	else
		t->times = -2;
	if (t->num <= 0 || t->die <= 0 || t->eat <= 0 || t->sleep <= 0 || t->times == -1)
	{
		free(t);
		return (0);
	}
	t->start_time = get_time();
	if (create_forks(t) || generate_philos(t))
		return (0);
	return (t);
}