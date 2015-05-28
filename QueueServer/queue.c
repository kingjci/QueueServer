#include <windows.h>
#include "common.h"
#include "queue.h"
#include "globals.h"

PQUEUE_HEAD queue_initialize(PWCHAR name)
{
	PQUEUE_HEAD p = (PQUEUE_HEAD)malloc(sizeof(QUEUE_HEAD));
	if (p)
	{
		console(("创建队列出错...\n"));
		return (PQUEUE_HEAD)-1;
	}

	wcscpy_s(p->name, wcslen(name), name);
	p->next = p->front = p->rear = NULL;
	p->lock_front = CreateMutex(NULL, FALSE, NULL);
	p->lock_rear = CreateMutex(NULL, FALSE, NULL);
	p->lock_count = CreateMutex(NULL, FALSE, NULL);

	return p;
}

int queue_in(PWCHAR data, PQUEUE_HEAD queue)
{

	WaitForSingleObject(queue->lock_rear, INFINITE);
	
	return 0;
}
