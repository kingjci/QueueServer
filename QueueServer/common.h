#ifndef _COMMON_H_
#define _COMMON_H_

#include <windows.h>
#include "debug.h"

#define MAXBUFFER 2048
#define SERVER_PORT 5789


#define true 1
#define false 0


#define console(content) WaitForSingleObject(lock_console, INFINITE);\
						 wprintf##content;\
						 ReleaseMutex(lock_console);



#ifdef debug
#define debug_console(content)	WaitForSingleObject(lock_console, INFINITE);\
								wprintf##content;\
								ReleaseMutex(lock_console);
#else
#define debug_console(content)	;
#endif



#define thread_console(content) if (id_current_thread == id_child_thread) \
																{ \
									WaitForSingleObject(lock_console, INFINITE); \
									wprintf##content; \
									ReleaseMutex(lock_console); \
																}\







/**
	注意！使用下面三个宏的时候需要定义这3个变量
	char receive_buffer[MAXBUFFER];
	WCHAR w_receive_buffer[MAXBUFFER];
	int status;


	需要检测函数的返回值  int status = char_to_wchar(buffer, w_buffer);
*/

#define char_to_wchar(buffer, w_buffer)  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, w_buffer, 50)
										






#define new(type) (type *)malloc(sizeof(type))

#define new_wstring(p_wstring) (PWCHAR)malloc(wcslen(p_wstring)*sizeof(WCHAR) + 2)

#define wstring_copy(destination, source) wcscpy_s(destination, wcslen(source)+1, source)











#endif

