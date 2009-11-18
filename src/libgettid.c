#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <libgettid.h>
#include "libgettid_private.h"

static struct thread_id *threads_list;
pthread_mutex_t list_mtx = PTHREAD_MUTEX_INITIALIZER;

static void
libgettid_init(void)
{
	if (!real_pthread_create)
		real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");
}

static void *
fake_start_routine(void *arg)
{
	struct create_args *args = (struct create_args*) arg;
	void *retval = NULL;
	pid_t tid;
	int res;
	pthread_t self;

	if (!args || !args->routine) {
		fprintf(stderr, "Fake routine called with NULL args\n");
		return NULL;
	}

	tid = syscall(__NR_gettid);
	self = pthread_self();
	pthread_mutex_lock(&list_mtx);
	res = list_add(&threads_list, tid, self);
	pthread_mutex_unlock(&list_mtx);
	retval = args->routine(args->arg);

	if (res == 0)
	{
		pthread_mutex_lock(&list_mtx);
		list_remove(&threads_list, self);
		pthread_mutex_unlock(&list_mtx);
	}
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

// API functions
int pthread_get_tid(pthread_t thread, pid_t *tid)
{
	thread_id_t *info;
	if (threads_list == NULL) {
		return GETTID_E_NOTHREADS;	
	}
	pthread_mutex_lock(&list_mtx);
	info = list_find(threads_list, thread);
	pthread_mutex_unlock(&list_mtx);
	if (!info) {
		return GETTID_E_THREADNOTFOUND;
	}		
	*tid = info->tid;
	return 0;
}

static const char *gettid_errors[] = {
	"Operation successful",
	"Method not implemented",
	"Thread not found",
	"Not found, threads list is empty"
};
static const char *gettid_unknown_error = "Unknown error";

const char *gettid_strerror(int errno)
{
	if ((errno < 0) || (errno > (int) (sizeof(gettid_errors) / sizeof (*gettid_errors))) ) {
		return gettid_unknown_error;
	}
	return gettid_errors[errno];
}

/* vim: set ts=8 noexpandtab shiftwidth=8: */
