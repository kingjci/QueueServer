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
	wchar_t a[100] = L"���";
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
		wprintf(L"��ʼ��Windows Socketʧ��...\n");
		return -1;
	}
	wprintf(L"��ʼ��Windows Socket�ɹ���\n");

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		wprintf(L"�����׽���ʧ��...\n");
		return -2;
	}
	wprintf(L"�����׽��ֳɹ���\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (status)
	{
		wprintf(L"�󶨶˿�ʧ��...\n ����5789�˿��Ƿ��Ѿ���ռ��...\n");
		WSACleanup();
		return -3;
	}
	wprintf(L"�󶨶˿ڳɹ���\n");

	status = listen(server_socket, 20);
	if (status)
	{
		wprintf(L"�����˿�ʧ��...\n");
		return -4;
	}
	wprintf(L"�����˿ڳɹ���\n");

	wprintf(L"���з������ɹ�����!\n\n");

	client_addr_len = sizeof(client_addr);

	while (true)
	{
		client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);
		
		if (client_socket == INVALID_SOCKET)
		{
			wprintf(L"���ӿͻ���ʧ��...\n");
			wprintf(L"������:%d \n", GetLastError());
			status = -5;
		}

		WCHAR w_client_addr[50];
		status = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, inet_ntoa(client_addr.sin_addr), -1, w_client_addr, 50);
		if (!status)
		{
			wprintf(L"���ӵ�ַת��ʧ��...\n");
		}
		wprintf(L"�����ӿͻ���%ls:%d \n", w_client_addr, client_addr.sin_port);


		handle_child_thread = CreateThread(NULL, 0, client_thread, (LPVOID) &client_socket, 0, &child_thread_id);

		if (handle_child_thread == NULL)
		{
			wprintf(L"���̴߳���ʧ��...\n");
			status = -6;
		}
		wprintf(L"�������߳�%d\n\n", child_thread_id);

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
		console((L"Socket�������̴߳���ʧ��...\n"));
		status = -6;
	}
	console((L"����Socket�������߳�%d\n\n", id_server_thread));

	CloseHandle(handle_server_thread);

	Sleep(1000000000);

	return 0;

}