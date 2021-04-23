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
	sem_wait(p->write);
	printf("%lu %d is sleeping\n", timestamp(p), p->id);
	sem_post(p->write);

	zsleep(p->sleep);

	sem_wait(p->write);
	printf("%lu %d is thinking\n", timestamp(p), p->id);
	sem_post(p->write);
}

void p_eat (t_philo *p)
{
	
	sem_wait(p->write);
	//printf("ay\n");
	//write(1, "STARTEAT\n", 9);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d has taken a fork\n", timestamp(p), p->id);
	printf("%lu %d is eating\n", timestamp(p), p->id);
	sem_post(p->write);
	p->last_eaten = get_time();
	sem_post(p->alive);
	zsleep(p->eat);

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

void zsleep(unsigned long micro)
{
	unsigned long target;
	unsigned long start;

	start = get_time();
	target = start + micro;
	while (start < target)
	{
		start = get_time();
		if (start >= target)
			return;
		usleep(((target - start) >> 1) * 1000);
	}
}
