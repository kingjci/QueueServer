#ifndef _QUEUE_SERVER_
#define _QUEUE_SERVER_

#include "common.h"


/**
注意！使用下面三个宏的时候需要定义这3个变量
char receive_buffer[MAXBUFFER];
WCHAR w_receive_buffer[MAXBUFFER];
int status;

*/



#define receive(socket, buffer) status = recv(socket, buffer, MAXBUFFER, 0); \
				if (status == 0 || status == SOCKET_ERROR) \
								{ \
					console((L"客户端线程%d接受数据出错，错误码%d \n", id_current_thread, GetLastError())); \
					return -1; \
								} \



#define send(socket, buffer) status = send(client_socket, receive_buffer, strlen(receive_buffer), 0); \
							 if (status == 0 || status == SOCKET_ERROR) \
							 							 { \
								console((L"客户端线程%d退出，错误码%d \n", id_current_thread, GetLastError())); \
								return -1; \
							 							 } \



#endif