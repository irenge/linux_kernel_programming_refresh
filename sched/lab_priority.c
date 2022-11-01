#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {

	int prio, newprio, rc;

	pid_t pid;
	
	if( argc > 1)
		pid = atoi(argv[1]);
	else
		pid = getpid();

	prio = getpriority(PRIO_PROCESS, (id_t) pid);

	printf("\npid = %d --Priority of %d", pid, prio);

	for (int i = -19; i < 20; i += 3) {
		rc = setpriority(PRIO_PROCESS, (id_t)pid, i);
		if (rc)
			fprintf(stderr, "setpriority failed");

		// clear error b4 calling getpriority

		errno = 0;

		newprio = getpriority(PRIO_PROCESS, (int) pid);

		printf("\npid = %d--Priority of %d", pid, newprio);
	}

	exit(EXIT_SUCCESS);
}


