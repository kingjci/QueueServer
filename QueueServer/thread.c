#include <stdio.h>
#include <locale.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#include "common.h"
#include "globals.h"


extern HANDLE lock_console;

DWORD WINAPI client_thread(LPVOID IPpARAMETER)
{
	SOCKET client_socket = (SOCKET)IPpARAMETER;
	char receive_buffer[MAXBUFFER];
	WCHAR w_receive_buffer[MAXBUFFER];
	int status;

	int id_current_thread = GetCurrentThreadId();

	/***
	while (true)
	{
		
		memset(receive_buffer, 0, sizeof(receive_buffer));
		receive(client_socket, receive_buffer);
		char_to_wchar(receive_buffer, w_receive_buffer);

		console((L"%ls\n", w_receive_buffer));

		send(client_socket, receive_buffer);
		


	}
	*/








	Sleep(10000000);
	return 0;
}

DWORD WINAPI server_thread(LPVOID IPpARAMETER)
{
	int status;
	int client_addr_len;
	WSADATA Ws;
	SOCKET server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;

	HANDLE handle_client_thread;
	int id_client_thread;



	int id_current_thread = GetCurrentThreadId();


	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		console((L"服务器:初始化Windows Socket失败...\n"));
		return -1;
	}
	console((L"服务器:初始化Windows Socket成功！\n"));

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		console((L"服务器:创建套接字失败...\n"));
		return -2;
	}
	console((L"服务器:创建套接字成功！\n"));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (status)
	{
		console((L"服务器:绑定端口失败...\n 请检查5789端口是否已经被占用...\n"));
		WSACleanup();
		return -3;
	}
	console((L"服务器:绑定端口成功！\n"));

	status = listen(server_socket, 20);
	if (status)
	{
		console((L"服务器:监听端口失败...\n"));
		return -4;
	}
	console((L"服务器:监听端口%d成功！\n", SERVER_PORT));

	console((L"服务器:队列服务器成功启动!\n\n"));

	client_addr_len = sizeof(client_addr);

	while (true)
	{
		client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);

		if (client_socket == INVALID_SOCKET)
		{
			console((L"服务器:连接客户端失败...\n"));
			console((L"服务器:错误码%d \n", GetLastError()));
			status = -5;
		}

		WCHAR w_client_addr[50];
		status = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, inet_ntoa(client_addr.sin_addr), -1, w_client_addr, 50);
		if (!status)
		{
			console((L"服务器:连接地址转换失败...\n"));
		}
		console((L"服务器:已连接客户端%ls  %d \n", w_client_addr, client_addr.sin_port));


		handle_client_thread = CreateThread(NULL, 0, client_thread, (LPVOID)client_socket, 0, &id_client_thread);

		if (id_client_thread == 0)
		{
			console((L"服务器:客户端线程创建失败...\n"));
			status = -6;
		}
		console((L"服务器:创建客户端线程%d\n\n", id_client_thread));

		CloseHandle(handle_client_thread);

	}

	
	closesocket(server_socket);
	WSACleanup();

	return 0;
}