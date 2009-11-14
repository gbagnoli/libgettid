#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#define TNUM 2

void *thread_body(void *arg)
{
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(__NR_gettid);
	printf("[thread_body] PID: %d, TID: %d\n", pid, tid);
	return NULL;
}

int main (int argc, char *argv[])
{

	pthread_t threads[TNUM];
	int i;
	
	for (i=0; i<TNUM; i++)
	{
		pthread_create(&threads[i], NULL, thread_body, NULL);
//		printf("[main][%d]: pthread_t as index: %d\n",i, (int) threads[i]); 
	}
	
	for (i=0; i<TNUM; i++)
	{
		pthread_join(threads[i], NULL);
		printf("[main]: joined thread %d\n", i);
	}
	return 0;
}
