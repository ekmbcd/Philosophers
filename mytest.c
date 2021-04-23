#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <sys/stat.h>

sem_t *sem;
sem_t *tun;

void thr(void *nu)
{
	int n = *((int *)nu);
	int i = 5;
	 while (i--)
	 {
		/* code */
		//sem_wait(tun);
		sem_wait(sem);
		printf("%d\n", n);
		//sem_post(tun);
		usleep(100000);
		
		sem_post(sem);
		usleep(1000);
		
	 }
}
int main()
{
	pthread_t threads[5];

	int nums[] = {1, 2, 3, 4, 5};
	sem_unlink("/sem");
	sem = malloc(sizeof(sem_t));
	sem = sem_open("/sem", O_CREAT, 01411, 2);
	sem_unlink("/tun");
	tun = malloc(sizeof(sem_t));
	tun = sem_open("/tun", O_CREAT, 01411, 1);
	int i = 0;
	while (i < 5)
	{
		//usleep(1000);
		pthread_create(&threads[i], NULL, (void *)thr, (void *)&nums[i]);
		i++;
	}
	pthread_join(threads[0], 0);
	usleep(10000);
}