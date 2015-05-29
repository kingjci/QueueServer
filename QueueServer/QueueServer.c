#include <stdio.h>
#include <locale.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#include "common.h"
#include "thread.h"
#include "globals.h"
#include "debug.h"


#ifdef test
#define main __run__test__
#endif


int main(int argc, char **argv)
{


	/*
	setlocale(LC_ALL, "");
	wchar_t a[100] = L"金成";
	wprintf(a);
	*/

	/*
	int status;
	int client_addr_len;
	WSADATA Ws;
	SOCKET server_socket,client_socket;
	struct sockaddr_in server_addr, client_addr;

	HANDLE handle_child_thread;
	int child_thread_id;

	setlocale(LC_ALL, "");


	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
    {
		wprintf(L"初始化Windows Socket失败...\n");
		return -1;
	}
	wprintf(L"初始化Windows Socket成功！\n");

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		wprintf(L"创建套接字失败...\n");
		return -2;
	}
	wprintf(L"创建套接字成功！\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (status)
	{
		wprintf(L"绑定端口失败...\n 请检查5789端口是否已经被占用...\n");
		WSACleanup();
		return -3;
	}
	wprintf(L"绑定端口成功！\n");

	status = listen(server_socket, 20);
	if (status)
	{
		wprintf(L"监听端口失败...\n");
		return -4;
	}
	wprintf(L"监听端口成功！\n");

	wprintf(L"队列服务器成功启动!\n\n");

	client_addr_len = sizeof(client_addr);

	while (true)
	{
		client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);
		
		if (client_socket == INVALID_SOCKET)
		{
			wprintf(L"连接客户端失败...\n");
			wprintf(L"错误码:%d \n", GetLastError());
			status = -5;
		}

		WCHAR w_client_addr[50];
		status = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, inet_ntoa(client_addr.sin_addr), -1, w_client_addr, 50);
		if (!status)
		{
			wprintf(L"连接地址转换失败...\n");
		}
		wprintf(L"已连接客户端%ls:%d \n", w_client_addr, client_addr.sin_port);


		handle_child_thread = CreateThread(NULL, 0, client_thread, (LPVOID) &client_socket, 0, &child_thread_id);

		if (handle_child_thread == NULL)
		{
			wprintf(L"子线程创建失败...\n");
			status = -6;
		}
		wprintf(L"创建子线程%d\n\n", child_thread_id);

		CloseHandle(handle_child_thread);

	}

	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();

	return 0;
	*/
	setlocale(LC_ALL, "");
	

	int status;

	HANDLE handle_server_thread;
	int id_server_thread;

	setlocale(LC_ALL, "");

	lock_console = CreateMutex(NULL, FALSE, NULL);

	handle_server_thread = CreateThread(NULL, 0, server_thread, NULL, 0, &id_server_thread);

	if (handle_server_thread == NULL)
	{
		console((L"Socket服务器线程创建失败...\n"));
		status = -6;
	}
	console((L"创建Socket服务器线程%d\n\n", id_server_thread));

	CloseHandle(handle_server_thread);

	Sleep(1000000000);

	return 0;

}