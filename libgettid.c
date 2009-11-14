#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <libgettid.h>

#define SHMKEY	3301
#define SHMSIZE 4096

typedef int (*real_pthread_create_fn)(pthread_t *thread, 
					const pthread_attr_t *attr,
					void *(*start_routine) (void*),
				  	void *arg);

static real_pthread_create_fn real_pthread_create = NULL;

struct thread_id {
	pid_t 		pid;
	pid_t 		tid;
	pthread_t 	pth;
};

struct create_args {
	void *(*routine)(void*);
	void *arg;
};


static void
libgettid_init(void)
{
	if (!real_pthread_create)
		real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");

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
	struct thread_id TID;
	TID.pid = getpid();
	TID.tid = syscall(__NR_gettid);
	TID.pth = pthread_self();
	void *retval = NULL;

	if (!args || !args->routine) {
		fprintf(stderr, "Fake routine called with NULL args\n");
		return NULL;
	}
	fprintf(stderr, "In fake_start_routine(@%p): pthread_t %d PID %d TID %d\n", 
			args->routine,
			(int) TID.pth,
			TID.pid, TID.tid);
	retval = args->routine(args->arg);

	free(arg);

	return retval; 
}

int 
pthread_create(pthread_t *thread,
		const pthread_attr_t *attr,
		void *(*start_routine) (void *),
		void *arg)
{
	int error = -1; /* FIXME: specific error */
	struct create_args *args = calloc(1, sizeof(struct create_args));

	libgettid_init();

	args->routine 	= start_routine;
	args->arg 	= arg;
	error = real_pthread_create(thread, attr, fake_start_routine, args);
	return error;
}

int gettid(pthread_t thread)
{
	return GETTID_E_UNIMPLEMENTED;
}

char *gettid_strerror(int errno)
{
	return "Uhm";
}

/* vim: set ts=8 noexpandtab shiftwidth=8: */
