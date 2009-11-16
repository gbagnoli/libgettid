#ifndef __LIST_H__
#define __LIST_H__
#include <pthread.h>
#include "libgettid_private.h"

thread_id_t* list_find(thread_id_t *head, pthread_t pth);
int list_remove(thread_id_t **head, pthread_t pth);
int list_add(thread_id_t **head, pid_t tid, pthread_t pth);

#endif
