#include <windows.h>

#include "common.h"
#include "user.h"
#include "queue.h"







//Process related
HANDLE lock_console;
int id_child_thread;



//User list related

PUSER_LIST user_list;


//Queue list related;

PQUEUE_HEAD queue_list;