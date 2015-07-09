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
		console((L"��ʼ�������б����...\n"));
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
		console((L"�������г���...\n"));
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
		console((L"�޷������������...\n"));
		return -1;
	}

	p->next = NULL;
	p->data = NULL;

	PWCHAR q = (PWCHAR)malloc(wcslen(data)*sizeof(WCHAR) + 2);


	if (!q)
	{
		console((L"�޷������������...\n"));
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


	debug_console((L"����%ls����һ����Ϣ%ls,����%ls������Ϣ:%d,һ������Ϣ:%d\n", \
		queue->name, data, queue->name, queue->count, queue_list->total_count));

	return 0;
}

int queue_consume(PWCHAR data, PQUEUE_HEAD queue)
{

	int status;
	WCHAR data_from_queue[MAXBUFFER];

	
	
	while (true)
	{
		//�ڶ�����ֻ����һ��Ԫ��������ӵ�����£�Ϊ�˱���������������������ǰ�����������Ϊ���ܷ���ǰ����
		//���������ٴγ��Ե�ʱ����Ҫ���»��ǰ��
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
			�ڻ�ȡ�ڻ�ȡ��lock_front֮�󣬶����е�Ԫ�ز�����٣������ܱ�࣬���磺��ȡ��lock_front��
			��ȡcount=0��1�����ʱ�����뵽��lock_rear���������ʱ��produce�����Ѿ�ʹ�ö����е�Ԫ�ش���1��
			�ˣ���Ҫ���µ���
			*/

			status = WaitForSingleObject(queue->lock_rear, 0);
			if (status == WAIT_ABANDONED)
			{
				/*
					��Ӧ���Ѿ���ȡǰ�������ǻ�û�л�ȡ������������ʱ�������û��Ԫ�أ���Ҫ����У�
				�����Ѿ���produce��ã������Ļ�������ﲻ����ǰ�����ᷢ����������Ϊ���������һ��
				��Ҫ��ȡ����������ֻ�ܳ��Ի�ȡ������������ɹ�����ܶ�����û��Ԫ�أ�����produceһ��Ԫ��
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
				ReleaseMutex(queue->lock_front);//������������produce�Ⱥ����Ĺ����У������ڵ�event��������produce�ڵȺ�����Զ����setevent�γ�������
				WaitForSingleObject(queue->event_empty, INFINITE);
				
				continue;
			}
			else
			{
				/*
					��Ӧ���ڻ�ȡǰ���ͺ������ʱ�����У���β�����˴���1���Ķ���
				*/
				ReleaseMutex(queue->lock_rear);
				ReleaseMutex(queue->lock_front);
				continue;
			}
		}


	}



	return 0;
}