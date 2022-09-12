#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define __NR_get_max 451


void main(void)
{
	int p1, p2;
	
	p1 = 3; p2 = 11;

	printf("get_max(%d, %d) returns :%d\n", p1, p2, syscall(__NR_get_max, p1, p2));

	p1 = 20; p2 = 9;
        printf("get_max(%d, %d) returns :%d\n", p1, p2, syscall(__NR_get_max, p1, p2));

}

