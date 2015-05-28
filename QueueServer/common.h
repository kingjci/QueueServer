#ifndef _COMMON_H_
#define _COMMON_H_

#define MAXBUFFER 2048
#define SERVER_PORT 5789


#define true 1
#define false 0


#define console(content) WaitForSingleObject(lock_console, INFINITE);\
						 wprintf##content;\
						 ReleaseMutex(lock_console);


/**
	注意！使用下面三个宏的时候需要定义这3个变量
	char receive_buffer[MAXBUFFER];
	WCHAR w_receive_buffer[MAXBUFFER];
	int status;

*/

#define thread_console(content) if (id_current_thread == id_child_thread) \
								{ \
									WaitForSingleObject(lock_console, INFINITE); \
									wprintf##content; \
									ReleaseMutex(lock_console); \
								}\

#define receive(socket, buffer) status = recv(socket, buffer, MAXBUFFER, 0); \
				if (status == 0 || status == SOCKET_ERROR) \
				{ \
					console((L"客户端线程%d接受数据出错，错误码%d \n", id_current_thread, GetLastError())); \
					return -1; \
				} \

#define char_to_wchar(buffer, w_buffer) status = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, w_buffer, 50); \
										if (!status) \
										{ \
											console((L"客户端线程%d字符串转换失败...\n", id_current_thread)); \
										} \

#define send(socket, buffer) status = send(client_socket, receive_buffer, strlen(receive_buffer), 0); \
							 if (status == 0 || status == SOCKET_ERROR) \
							 { \
								console((L"客户端线程%d退出，错误码%d \n", id_current_thread, GetLastError())); \
								return -1; \
							 } \

#endif

