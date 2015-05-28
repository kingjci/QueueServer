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
	ע�⣡ʹ�������������ʱ����Ҫ������3������
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
					console((L"�ͻ����߳�%d�������ݳ���������%d \n", id_current_thread, GetLastError())); \
					return -1; \
				} \

#define char_to_wchar(buffer, w_buffer) status = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, w_buffer, 50); \
										if (!status) \
										{ \
											console((L"�ͻ����߳�%d�ַ���ת��ʧ��...\n", id_current_thread)); \
										} \

#define send(socket, buffer) status = send(client_socket, receive_buffer, strlen(receive_buffer), 0); \
							 if (status == 0 || status == SOCKET_ERROR) \
							 { \
								console((L"�ͻ����߳�%d�˳���������%d \n", id_current_thread, GetLastError())); \
								return -1; \
							 } \

#endif

