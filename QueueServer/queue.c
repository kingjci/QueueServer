#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include "common.h"
#include "queue.h"
#include "globals.h"
#include "debug.h"

int queue_initialize()
{
	queue_list = new(QUEUE_LIST);

	if (!queue_list)
	{
		console((L"初始化队列列表出错...\n"));
		return -1;
	}

	queue_list->total_count = 0;
	queue_list->total_queue = 0;
	queue_list->front = NULL;
	queue_list->rear = NULL;
	queue_list->lock_queue_list = CreateMutex(NULL, FALSE, NULL);

	return 0;
}




PQUEUE_HEAD queue_create(PWCHAR name, int ack)
{
	PQUEUE_HEAD p = new(QUEUE_HEAD);
	if (!p)
	{
		console((L"创建队列出错...\n"));
		return (PQUEUE_HEAD) -1;
	}
	wcscpy_s(p->name, wcslen(name) + 1,name);
	p->front = p->rear = NULL;
	p->subscribe_user = NULL;
	p->next_queue = NULL;
	p->lock_front = CreateMutex(NULL, FALSE, NULL);
	p->lock_rear = CreateMutex(NULL, FALSE, NULL);
	p->event_empty = CreateEvent(NULL, FALSE, FALSE, NULL);
	p->ack = ack;
	p->count = 0;

	WaitForSingleObject(queue_list->lock_queue_list, INFINITE);
	if (queue_list->front == NULL)
	{
		queue_list->front = p;
		queue_list->rear = p;
	}
	else
	{
		queue_list->rear->next_queue = p;
		queue_list->rear = p;
	}
	ReleaseMutex(queue_list->lock_queue_list);

	return p;
}

int queue_produce(PWCHAR data, PQUEUE_HEAD queue)
{
	WCHAR buffer[100];
	PQUEUE_NODE p = new(QUEUE_NODE);
	if (!p)
	{
		console((L"无法分配队列数据...\n"));
		return -1;
	}

	p->next = NULL;
	p->data = NULL;

	PWCHAR q = (PWCHAR)malloc(wcslen(data)*sizeof(WCHAR) + 2);


	if (!q)
	{
		console((L"无法分配队列数据...\n"));
		return -1;
	}
	wcscpy_s(q, wcslen(data)+1, data);
	debug_console((L"%s\n",q));
	p->data = q;

	WaitForSingleObject(queue->lock_rear, INFINITE);

	if (queue->count == 0)
	{
		WaitForSingleObject(queue->lock_front, INFINITE);

		queue->rear = p;
		queue->front = p;		
		(queue->count)++;
		(queue_list->total_count)++;

		SetEvent(queue->event_empty);
		ReleaseMutex(queue->lock_rear);
		ReleaseMutex(queue->lock_front);
	}
	else
	{
		queue->rear->next = p;
		queue->rear = p;
	}


	debug_console((L"队列%ls放入一条消息%ls,队列%ls共有消息:%d,一共有消息:%d\n", \
		queue->name, data, queue->name, queue->count, queue_list->total_count));

	return 0;
}

int queue_consume(PWCHAR data, PQUEUE_HEAD queue)
{

	int status;
	WCHAR data_from_queue[MAXBUFFER];

	
	
	while (true)
	{
		//在队列中只有有一个元素正在入队的情况下，为了避免死锁，存在主动放弃前锁的情况。因为可能放弃前锁，
		//所以这里再次尝试的时候需要重新获得前锁
		WaitForSingleObject(queue->lock_front, INFINITE);
		if (queue->count > 1)
		{
			PQUEUE_NODE p = queue->front;
			wcscpy_s(data_from_queue, wcslen(p->data) + 1, p->data);
			queue->front = queue->front->next;
			free(p->data);
			free(p);
			(queue->count)--;
			(queue_list->total_count)--;
			ReleaseMutex(queue->lock_front);
			break;
		}
		else 
		{
			/*
			在获取在获取到lock_front之后，队列中的元素不会变少，但可能变多，例如：获取了lock_front，
			读取count=0或1，这个时候申请到了lock_rear，但是这个时候produce操作已经使得队列中的元素大于1个
			了，需要重新调用
			*/

			status = WaitForSingleObject(queue->lock_rear, 0);
			if (status == WAIT_ABANDONED)
			{
				/*
					对应于已经获取前锁，但是还没有获取后锁。如果这个时候队列中没有元素，正要入队列，
				后锁已经被produce获得，这样的话如果这里不放弃前锁，会发生死锁。因为到这里操作一定
				需要获取后锁，所以只能尝试获取后锁，如果不成功则可能队列中没有元素，正在produce一个元素
				*/
				ReleaseMutex(queue->lock_front);
				continue;
			}

			if (queue->count == 1)
			{
				wcscpy_s(data_from_queue, wcslen(queue->front->data) + 1, queue->front->data);
				free(queue->front->data);
				free(queue->front);
				queue->front = NULL;
				queue->rear = NULL;

				(queue->count)--;
				(queue_list->total_count)--;

				ReleaseMutex(queue->lock_rear);
				ReleaseMutex(queue->lock_front);
				
				break;
			}
			else if (queue->count == 0)
			{
				ReleaseMutex(queue->lock_rear);
				ReleaseMutex(queue->lock_front);//如果放在最后，在produce等后锁的过程中，这里在等event，而由于produce在等后锁永远不会setevent形成死锁。
				WaitForSingleObject(queue->event_empty, INFINITE);
				
				continue;
			}
			else
			{
				/*
					对应于在获取前锁和后所这个时间间隔中，队尾插入了大于1个的对象
				*/
				ReleaseMutex(queue->lock_rear);
				ReleaseMutex(queue->lock_front);
				continue;
			}
		}


	}



	return 0;
}