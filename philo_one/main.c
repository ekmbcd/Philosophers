#include "philo.h"

void *metaphysic(void *philo)
{
	t_philo *p;
	int start;

	start = 1;
	p = (t_philo *)philo;
	while (p->times != 0)
	{
		//printf("1\n");
		//check if alive
		//check if left fork is free //check if right fork is free
		if(p->id % 2)
		{

			pthread_mutex_lock(&(p->m_forks[p->right]));
			pthread_mutex_lock(&(p->m_forks[p->left]));
		}
		else
		{
			if (start)
				usleep((p->eat) * 500);
			pthread_mutex_lock(&(p->m_forks[p->left]));
			pthread_mutex_lock(&(p->m_forks[p->right]));
		}
		//printf("2\n");
		pthread_mutex_lock(&(p->alive));
//		pthread_mutex_lock(p->m_ego);
		// if (p->forks[p->right] == 0 && p->forks[p->left] == 0)
		// {
			//start eating
		p_eat(p);
		// pthread_mutex_lock(p->m_write);
		// if (p->id % 2)
		// 	write(1, "Disp\n", 5);
		// else
		// 	write(1, "pari\n", 5);

		// pthread_mutex_unlock(p->m_write);
//		if(p->id % 2)
//		{
			//pthread_mutex_lock(p->m_write);
			pthread_mutex_unlock(&(p->m_forks[p->right]));
			//printf("%lu %d FORKETTA DESTRA\n", timestamp(p), p->id);
			pthread_mutex_unlock(&(p->m_forks[p->left]));
			//printf("%lu %d FORKETTA SINISTRA\n", timestamp(p), p->id);
			//pthread_mutex_unlock(p->m_write);

		// }
		// else
		// {
		// 	//pthread_mutex_lock(p->m_write);
		// 	pthread_mutex_unlock(&(p->m_forks[p->right]));
		// 	//printf("%lu %d FORKETTA DESTRA\n", timestamp(p), p->id);
		// 	pthread_mutex_unlock(&(p->m_forks[p->left]));
		// 	//printf("%lu %d FORKETTA SINISTRA\n", timestamp(p), p->id);
		// 	//pthread_mutex_unlock(p->m_write);


		// }

		//sleep
		p_sleep(p);
		(p->times)--;
		start = 0;
		// }
// 		else
// 		{
// 			printf("yooooo\n");
// //			pthread_mutex_unlock(p->m_ego);
// 			pthread_mutex_unlock(&(p->m_forks[p->right]));
// 			pthread_mutex_unlock(&(p->m_forks[p->left]));
// 			pthread_mutex_unlock(&(p->alive));
// 		}

		//stop after p.times eaten
	}
	//printf(">>DONE\n");
	return(0);
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
		// t->philos[i]->forks = t->forks;
		// t->philos[i]->m_ego = &t->m_ego;
		t->philos[i]->m_write = &t->m_write;
		pthread_mutex_init(&t->philos[i]->alive, NULL);
	}
}

void create_forks(t_table *t)
{
	int i;

	i = 0;
	//t->forks = malloc(sizeof(char) * t->num);
	t->m_forks = calloc(t->num + 1, sizeof(pthread_mutex_t));
	pthread_mutex_init(&t->m_write, NULL);
	while (i < t->num)
	{
		pthread_mutex_init(&t->m_forks[i], NULL);
		i++;
	}


	// while (i < t->num)
	// {
	// 	t->forks[i] = 0;
	// 	i++;
	// }
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
		pthread_detach(threads[i]);
		i++;
	}
	//free(threads);
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
		t->times = -2;
	if (t->num <= 0 || t->die <= 0 || t->eat <= 0 || t->sleep <= 0 || t->times == -1)
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
	unsigned long now;
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
//			now = get_time();

			if (get_time() - t->philos[i]->last_eaten > t->die)
			{
				pthread_mutex_lock(&(t->m_write));
				//printf("<%lu>\n", now - t->start_time);
				printf("%lu %d died\n", timestamp(t->philos[i]), i + 1);
				//all threads should be killed
				//pthread_mutex_unlock(&(t->philos[i]->alive));
				return (1);
			}
			pthread_mutex_unlock(&(t->philos[i]->alive));
		}
		if (done == 0)
		{
			pthread_mutex_lock(&(t->m_write));
			//printf("yo\n");
			return (0);
		}

	}
	return(0);
}

int main(int argc, char const *argv[])
{
	t_table *t;
	int i;
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
	i = 0;
	while (i < t->num)
	{
		pthread_cancel(threads[i]);
		pthread_mutex_destroy(&t->philos[i]->alive);
		free(t->philos[i]);
		pthread_mutex_destroy(&t->m_forks[i++]);
	}
	pthread_mutex_destroy(&t->m_write);
	free(t->philos);
	free(t->m_forks);
	free(t);
	free(threads);
	return(0);

}
