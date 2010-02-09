#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int list_add(thread_id_t **head, pid_t tid, pthread_t pth)
{

	thread_id_t *p;
	p = calloc(1, sizeof(thread_id_t));
	if (!p)
		return 1;
	p->tid = tid;
	p->pth = pth;
	p->next = *head;
	*head = p;

	return 0;

}

thread_id_t* list_find(thread_id_t *head, pthread_t pth)
{
	thread_id_t *p;

	p = head;
	while (p && (pthread_equal(p->pth, pth)) == 0)
		p = p->next;
	return p;
}

int list_remove(thread_id_t **head, pthread_t pth)
{
	thread_id_t *p,*q;
	if (pthread_equal((*head)->pth, pth) != 0)
	{
		free(*head);
		*head = NULL;
		return 0;
	}
	q = p = *head;
	while (p && (pthread_equal(p->pth, pth)) == 0)
	{
	    q = p;
	    p = p->next;
	}
	if (!p)
	    return 1;

	q->next = p->next;
	free(p);
	
	return 0;
}


