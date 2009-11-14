#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

#define SHMKEY	3301
#define SHMSIZE 4096

static int (*pthread_create_pthread)(pthread_t *thread, 
					const pthread_attr_t *attr,
					void *(*start_routine) (void*),
					void *arg);

struct thread_id {
	pid_t pid;
	pid_t tid;
	int pthreadt;
};

struct create_args {
	void *(*routine)(void*);
	void *arg;
};

//static volatile struct thread_id *head;

static void
libgettid_init(void)
{
	pthread_create_pthread = dlsym(RTLD_NEXT, "pthread_create");

/*	int shmid;
	shmid = shmget(SHMKEY, SHMZ, 0666);
	if (shmid < 0)
		return;
	
	head = shmat(shmid, NULL, 0);
	if ((intptr_t)head < 0)
		head = NULL;
*/		
}

static void *
fake_start_routine(void *arg)
{
	struct create_args *args = (struct create_args*) arg;
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	pthread_t pthreadt = pthread_self();
	void *retval = NULL;

	printf("In fake_start_routine(@%ld): pthread_t %d PID %d TID %d\n", 
								(long int) args->routine,
								(int) pthreadt,
								pid, tid);
	if (!args) {
		fprintf(stderr, "Fake routine called with NULL args\n");
		return NULL;
	}
	printf("Calling args->routine\n");
//	if (args->routine)
//		retval = args->routine(args->arg);
	return retval; 
}

int 
pthread_create(pthread_t *thread,
		const pthread_attr_t *attr,
		void *(*start_routine) (void *),
		void *arg)
{
	int error;
	struct create_args args;

	if (!pthread_create_pthread)
		libgettid_init();

	args.routine = start_routine;
	args.arg = arg;
	error = (*pthread_create_pthread)(thread, attr, fake_start_routine, &args);
	return error;
}

/* vim: set ts=8 noexpandtab shiftwidth=8: */
