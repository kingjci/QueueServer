#ifndef _QUEUESERVER_
#define _QUEUESERVER_

#define MAXBUFFER 2048
#define SERVER_PORT 5789


#define true 1
#define false 0


#define console(content) WaitForSingleObject(lock_console, INFINITE);\
						 wprintf##content;\
						 ReleaseMutex(lock_console);

#define thread_console(content) if (id_current_thread == id_child_thread) \
								{ \
									WaitForSingleObject(lock_console, INFINITE); \
									wprintf##content; \
									ReleaseMutex(lock_console); \
								}\







#endif

