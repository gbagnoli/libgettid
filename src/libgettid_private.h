#ifndef __LIBGETTID_PRIVATE_H__
#define __LIBGETTID_PRIVATE_H__
#include <pthread.h>

typedef int (*real_pthread_create_fn)(pthread_t *thread, 
					const pthread_attr_t *attr,
					void *(*start_routine) (void*),
				  	void *arg);

static real_pthread_create_fn real_pthread_create = NULL;

struct thread_id {
	pid_t 		tid;
	pthread_t 	pth;
    struct thread_id *next;
};
typedef struct thread_id thread_id_t;

struct create_args {
	void *(*routine)(void*);
	void *arg;
};


#endif
