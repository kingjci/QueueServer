#ifndef _QUEUE_SERVER_
#define _QUEUE_SERVER_

#include "common.h"


/**
ע�⣡ʹ�������������ʱ����Ҫ������3������
char receive_buffer[MAXBUFFER];
WCHAR w_receive_buffer[MAXBUFFER];
int status;

*/



#define receive(socket, buffer) status = recv(socket, buffer, MAXBUFFER, 0); \
				if (status == 0 || status == SOCKET_ERROR) \
								{ \
					console((L"�ͻ����߳�%d�������ݳ���������%d \n", id_current_thread, GetLastError())); \
					return -1; \
								} \



#define send(socket, buffer) status = send(client_socket, receive_buffer, strlen(receive_buffer), 0); \
							 if (status == 0 || status == SOCKET_ERROR) \
							 							 { \
								console((L"�ͻ����߳�%d�˳���������%d \n", id_current_thread, GetLastError())); \
								return -1; \
							 							 } \



#endif