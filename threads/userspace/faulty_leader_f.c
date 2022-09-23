#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>

// global Parameter k

int k;


int follower(void *data)
{
	printf("\nINFO: This code is running under child process %d.\n", getpid());

	int i;
	
	for ( i = 0 ; i <= 10 ; i++ ) {
		k = i + 6;
		sleep(1);
		printf("\nChild thread running: i = %d, k = %d\n", i, k);
		
	}

	printf("\nReaching end of life of the child process\n");
	return 0;
}

void main(int argc, char *argv[])
{
	printf("Hello, World!\n");

	void *pchild_stack = malloc(1024 * 1024*1024);
	if ( pchild_stack == NULL ) {
		printf("ERROR: Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	int tid = clone(follower, pchild_stack + (1024 * 1024), CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM |
                  CLONE_SIGHAND | CLONE_THREAD, NULL);

	printf("with tid = %d\n", tid);

	if ( tid < 0 ) {
		printf("ERROR: Unable to create the child process.\nClone failed!");
		exit(EXIT_FAILURE);
	}

	//wait(NULL);

	 for (int j = 0; j < 6; j++) {
                k  = j;
                sleep(1);
                printf("\nparent thread running: j=%d, param=%d secs\n", j,
                       k);
        }
        printf("parent killitself\n");


	free(pchild_stack);

	//printf("INFO: Child process terminated.\n");
}
