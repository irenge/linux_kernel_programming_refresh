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
	printf("\nINFO: This code is running under follower process %d.\n", getpid());

	int i;

	for ( i = 0 ; i <= 10 ; i++ ) {
		k = i + 6;
		sleep(1);
		printf("\nchild thread running with pid %d : i = %d, k = %d secs\n",gettid(), i, k);

	}

	printf("\nReaching end of life of the child process\n");
	return 0;
}

int  main(int argc, char *argv[])
{
	int i, pagesize, stacksize, tid;
	void *stack;
	printf("Starting Leader thread, pid = %d\n", getpid());
	pagesize = getpagesize();
	stacksize = 4 * pagesize;

	if (posix_memalign(&stack, pagesize, stacksize)) {
		perror("posix_memalign failed");
		exit(EXIT_FAILURE);
	}
	printf
		("Setting off a clone thread with stack (0x%p) stacksize = %d....",
		 stack, stacksize);

for(int l = 1; l < 5; l++) {
	tid = clone(follower, (char *) stack + stacksize, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM |
			CLONE_SIGHAND | CLONE_THREAD | CLONE_VFORK | CLONE_PTRACE, NULL);

	printf("with tid = %d\n", tid);

	if ( tid < 0 ) {
		printf("ERROR: Unable to create the child process.\nClone failed!");
		exit(EXIT_FAILURE);
	}


	for ( i = 0 ; i <= 10 ; i++ ) {
		k = i ;
		sleep(1);
		printf("\nparent thread with thread id %d running: i = %d, k = %d secs\n",gettid(), i, k);

	}

	printf("parent killitself\n");
}

	exit(EXIT_SUCCESS);
}
