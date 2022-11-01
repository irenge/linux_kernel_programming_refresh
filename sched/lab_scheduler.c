#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>

#define DEATH(mess) { perror(mess); exit(errno);}

void printpolicy(int policy);

int main(int argc, char *argv[]) {

	//int prio, newprio, rc;

	int policy;

	struct sched_param p;

	pid_t pid;

	pid = getpid();

	policy = sched_getscheduler(pid);


	printf("\npid = %d\n===============\n", pid);

	printpolicy(policy);

	//policy = SCHED_FIFO;
	//printpolicy(policy);
	for (int i = 10; i < 51; i += 3) {
		if (i % 2) {
			policy = SCHED_FIFO;
		} else {
			policy = SCHED_RR;
		}



		p.sched_priority = i;
		if (sched_setscheduler(pid, policy, &p))
			DEATH("sched_setscheduler:");
		printf("p.sched_priority = %d\n", p.sched_priority);
		printpolicy(policy);
	}

	/*	for (int i = -19; i < 20; i += 3) {
		rc = setpriority(PRIO_PROCESS, (id_t)pid, i);
		if (rc)
		fprintf(stderr, "setpriority failed");

	// clear error b4 calling getpriority

	errno = 0;

	newprio = getpriority(PRIO_PROCESS, (int) pid);

	printf("\npid = %d--Priority of %d", pid, newprio);
	}
	*/

	exit(EXIT_SUCCESS);
}

void printpolicy(int policy)
{
	/* SCHED_NORMAL = SCHED_OTHER in user-space */
	if (policy == SCHED_OTHER)
		printf("policy = SCHED_OTHER = %d\n", policy);
	if (policy == SCHED_FIFO)
		printf("policy = SCHED_FIFO = %d\n", policy);
	if (policy == SCHED_RR)
		printf("policy = SCHED_RR = %d\n", policy);
}
