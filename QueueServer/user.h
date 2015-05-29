#ifndef _USER_H_
#define _USER_H_

#include <windows.h>

typedef struct _USER{

	WCHAR name[20];
	WCHAR password[50];
	int   permission[2];
	struct _USER_LIST *next;

} USER, *PUSER;

typedef struct _LOGIN_USER_LIST{

	WCHAR name[20];
	WCHAR session[10];
	int permission[2];
	struct _LOGIN_USER_LIST *next;
} LOGIN_USER_LIST, *PLOGIN_USER_LIST, QUEUE_USER_LIST, *PQUEUE_USER_LIST;










#endif