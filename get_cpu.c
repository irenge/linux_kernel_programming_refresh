#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(void)
{
	printf("My cpu is %d\n", sched_getcpu());
	exit(EXIT_SUCCESS);
}
