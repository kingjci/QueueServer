#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <Windows.h>
#include <malloc.h>
#include "user.h"





typedef struct _QUEUE_NODE{

	PWCHAR data;
	struct _QUEUE_NODE *next;

} QUEUE_NODE, *PQUEUE_NODE;


typedef struct _QUEUE_HEAD{

	WCHAR name[100];

	int count;
	int ack;

	PQUEUE_NODE front;
	PQUEUE_NODE rear;

	PUSER subscribe_user;

	HANDLE lock_front;
	HANDLE lock_rear;
	HANDLE event_empty;


	struct _QUEUE_HEAD *next_queue;


} QUEUE_HEAD, *PQUEUE_HEAD;

typedef struct _QUEUE_LIST
{
	int total_count;
	int total_queue;

	PQUEUE_HEAD front;
	PQUEUE_HEAD rear;

	HANDLE lock_queue_list;

} QUEUE_LIST, *PQUEUE_LIST;





int queue_initialize();
PQUEUE_HEAD queue_create(PWCHAR name, int ack);
int queue_produce(PWCHAR data, PQUEUE_HEAD queue);
int queue_consume(PWCHAR data, PQUEUE_HEAD queue);





#endif