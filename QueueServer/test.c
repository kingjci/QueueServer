#include <stdio.h>
#include "debug.h"




#ifndef debug
#define main __run__main__
#endif





int main(int argc, char **argv)
{
	printf("hello");
	return 0;
}