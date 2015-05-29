#include <stdio.h>
#include <locale.h>
#include "debug.h"
#include "queue.h"
#include "globals.h"
#include "common.h"

#ifndef test
#define main __run__main__
#endif





int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");

	debug_console((L"½ð³É°¡"));

	int status = queue_initialize();
	PQUEUE_HEAD queue;
	PQUEUE_HEAD queue1 = queue_create(L"hello", 0);
	PQUEUE_HEAD queue2 = queue_create(L"world", 0);
	PQUEUE_HEAD queue3 = queue_create(L"ok", 0);


	return 0;
}