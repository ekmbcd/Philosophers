#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct s_philo
{
	int	id;
	int pid;
	unsigned long last_eaten;
	//int alive;
	unsigned long start_time;
	int eat;
	int sleep;
	int die;
	int times;
	sem_t *forks;
	sem_t *write;
	sem_t *tunnel;
	sem_t *alive;
}	t_philo;

typedef struct s_table
{
	int num;
	unsigned long start_time;
	int die;
	int eat;
	int sleep;
	int times;
	t_philo **philos;
	sem_t *forks;
	sem_t *write;
	sem_t *tunnel;
}	t_table;


int ft_atoi(const char *str);
unsigned long get_time(void);
unsigned long timestamp(t_philo *p);
void p_sleep(t_philo *p);
void p_eat (t_philo *p);
void zsleep(unsigned long micro);
int starvation(t_philo *p);

#endif
