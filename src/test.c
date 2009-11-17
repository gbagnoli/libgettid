#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <libgettid.h>

#define TNUM 20

void *thread_body(void *arg)
{
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(__NR_gettid);
	printf("[thread_body] PID: %d, TID: %d\n", pid, tid);
	sleep(2);
	printf("[thread_body] PID: %d, TID: %d Exiting.\n", pid, tid);
	return NULL;
}

int main (int argc, char *argv[])
{

	pthread_t threads[TNUM];
	int i,res;
	pid_t tid;
	
	for (i=0; i<TNUM; i++)
	{
		pthread_create(&threads[i], NULL, thread_body, NULL);
		sleep(1);
		if ( (res = pthread_get_tid(threads[i], &tid)) != 0)
			fprintf(stderr, "%s\n", gettid_strerror(res));
		else
			printf("[main]: last-created tid: %d\n",tid); 
	}

	for (i=0; i<TNUM; i++)
	{
		pthread_join(threads[i], NULL);
		printf("[main]: joined thread %d\n", i);
	}
	return 0;
}
