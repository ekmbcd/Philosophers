#include "philo.h"

int metaphysic(void *philo)
{
	t_philo *p;
	int start;

	start = 1;
	p = (t_philo *)philo;

	//printf("%d\n", p->times);

	while (p->times != 0)
	{

		//if(start && p->id % 2 == 0)
		//	usleep((p->eat) * 500);
		sem_wait(p->tunnel);
		sem_wait(p->forks);
		sem_wait(p->forks);
		sem_post(p->tunnel);
		sem_wait(p->alive);
		p_eat(p);
		//printf("yo\n");
		sem_post(p->forks);
		sem_post(p->forks);
		p_sleep(p);
		(p->times)--;
		start = 0;
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
		t->philos[i]->start_time = t->start_time;
		t->philos[i]->eat = t->eat;
		t->philos[i]->sleep = t->sleep;
		t->philos[i]->die = t->die;
		t->philos[i]->times = t->times;
		t->philos[i]->forks = t->forks;
		t->philos[i]->write = t->write;
		t->philos[i]->tunnel = t->tunnel;
		//sem_init(&t->philos[i]->alive, 0, 1);
		sem_unlink("/s_alive");
		t->philos[i]->alive = sem_open("/s_alive", O_CREAT, 01411, 1);

		// t->philos[i]->m_ego = &t->m_ego;
	}
}


void genesys(t_table *t)
{
	int i;
	pthread_t thread;
	//int pid;

	i = 0;
	while (i < t->num)
	{
		t->philos[i]->pid = fork();
		if (t->philos[i]->pid == 0)
		{
			//printf(">created process %d\n", i);
			pthread_create(&thread, NULL, (void *)starvation, (void *)t->philos[i]);
			pthread_detach(thread);
			metaphysic(t->philos[i]);
			exit(42);
		}
		i++;
	}
	//free(threads);
	return ;
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
	sem_unlink("/s_forks");
	sem_unlink("/s_write");
	sem_unlink("/s_tunnel");
	//t->forks = malloc(sizeof(sem_t));
	//t->write = malloc(sizeof(sem_t));
	//t->tunnel = malloc(sizeof(sem_t));
	t->forks = sem_open("/s_forks", O_CREAT, 01411, t->num);
	t->write = sem_open("/s_write", O_CREAT, 01411, 1);
	t->tunnel = sem_open("/s_tunnel", O_CREAT, 01411, 1);
	//sem_init(&t->forks, 0, t->num);
	//sem_init(&t->write, 0, 1);
	return (t);

}

int starvation(t_philo *p)
{
	int i;
	unsigned long now;
	int done;

	unsigned long test;

	//zsleep(p->die >> 1);

	while (1)
	{
		sem_wait(p->alive);
//			now = get_time();
		if ((test = get_time() - p->last_eaten) > p->die)
		{
			sem_wait(p->write);
			//printf("<%lu>\n", now - t->start_time);
			printf("%lu %d died\n", timestamp(p), p->id);
			printf("last eat: %lu die: %d \n", test, p->die);
			//kill(p->pid, SIGINT);
			//all threads should be killed
			//pthread_mutex_unlock(&(t->philos[i]->alive));
			exit(1);
		}
		sem_post(p->alive);
		if (p->times == 0)
		{
			//sem_wait(&t->write);
			//printf("yo\n");
			exit(0);
		}
		usleep(500);
	}
	return(0);
}

int main(int argc, char const *argv[])
{
	t_table *t;
	int i;
	int status;
	int returned;



	t = init(argc, argv);
	if (t == 0)
	{
		printf("Error.\n");
		return(1);
	}
	generate_philos(t);
	//start threads
	//printf("yo\n");
	genesys(t);
	int res = waitpid(-1,&status,0);
	printf("waitpid = %d status %d\n", res);
	if (WIFEXITED(status)){
      returned = WEXITSTATUS(status);
      printf("exited normally with status %d\n",returned);
	}
	i = 0;
	if (returned == 1)
	{

		while (i < t->num)
		{
			//printf("killed %d\n", i);
			kill(t->philos[i]->pid, SIGINT);
			i++;
		}
	}
	//printf("almost\n");
	else
		while (waitpid(-1,0,0) != -1 || errno != ECHILD)
		{
			//printf("waited for child\n");
		}
	printf("done waiting\n");

	//check aliveness
	//starvation(t);
	//usleep(1000000);
	//join?
	//pthread_join(threads[0], NULL);
	//free
	i = 0;
	while (i < t->num)
	{
		//printf("aa\n");
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
	return(0);

}
