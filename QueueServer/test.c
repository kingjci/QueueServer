#include <stdio.h>
#include <locale.h>
#include "debug.h"
#include "queue.h"
#include "globals.h"
#include "common.h"

#ifndef test
#define main __run__main__
#endif

DWORD WINAPI procude_thread(LPVOID IPpARAMETER)
{
	PQUEUE_HEAD queue = (PQUEUE_HEAD)IPpARAMETER;

	while (true)
	{
		queue_produce(L"这是第一条消息", queue);
	}
	



	return 0;
}


DWORD WINAPI consume_thread(LPVOID IPpARAMETER)
{
	WCHAR data[100];
	PQUEUE_HEAD queue = (PQUEUE_HEAD)IPpARAMETER;
	
	while (true)
	{
		queue_consume(data, queue);
	}

	

	return 0;
}




int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	int a = sizeof(PWCHAR);
	a = sizeof(WCHAR);

	WCHAR data[100];
	int status = queue_initialize();
	
	PQUEUE_HEAD queue;
	queue = queue_create(L"hello", 0);
	

	status = queue_produce(L"this is a message", queue);
	status = queue_consume(data, queue);
	


	/*
	HANDLE handle_produce;
	HANDLE handle_consume;
	int int_produce;
	int int_consume;

	handle_produce = CreateThread(NULL, 0, procude_thread, queue, 0, &int_produce);
	CloseHandle(handle_produce);

	handle_consume = CreateThread(NULL, 0, consume_thread, queue, 0, &int_consume);
	CloseHandle(handle_consume);
	*/

	Sleep(1000000000);
	return 0;
}