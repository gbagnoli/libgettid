#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <libgettid.h>

#define TNUM 10
pthread_mutex_t end_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t start_m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_end_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t client_start_cond = PTHREAD_COND_INITIALIZER;

void *thread_body(void *arg)
{
	pid_t pid;
	pid_t tid;
	tid = syscall(__NR_gettid);
	int *idx = (int*) arg;
	printf("[thread_body:%d] Started with TID: %d\n", *idx, tid);
	// signal main thread, child started
	// wait the main to signal us we can finish
	printf("[thread_body:%d] Signal start and then waiting for main to signal us\n", *idx);
	pthread_cond_signal(&client_start_cond);
	pthread_mutex_lock(&end_m);
	pthread_cond_wait(&client_end_cond, &end_m);
	pthread_mutex_unlock(&end_m);
	pthread_exit(0);
}

int main (int argc, char *argv[])
{
	pthread_t threads[TNUM];
	int i,res;
	pid_t tid;
	int *idx;
	
	for (i=0; i<TNUM; i++)
	{
		printf("[main]: Starting thread %d\n", i);
		idx = malloc(sizeof(int));
		*idx = i;
		pthread_create(&threads[i], NULL, thread_body, idx);
		printf("[main]:\twaiting for thread %d to start\n",i);
		pthread_mutex_lock(&start_m);
		pthread_cond_wait(&client_start_cond, &start_m);
		pthread_mutex_unlock(&start_m);
		if ( (res = pthread_get_tid(threads[i], &tid)) != 0)
			fprintf(stderr, "%s\n", gettid_strerror(res));
		else 
			printf("[main]:\tthread %d tid: %d\n",i, tid); 
		
	}
	printf("[main]: wake up all threads\n");
	pthread_cond_broadcast(&client_end_cond);
}
