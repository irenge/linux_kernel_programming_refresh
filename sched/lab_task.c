#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#define DEATH(mess) { perror(mess); exit(errno); }
#define GET_ELAPSED_TIME(tv1, tv2) (\
(double)((tv2.tv_sec - tv1.tv_sec) \
          + .000001 * (tv2.tv_usec - tv1.tv_usec)))

#define JMAX (400*100000)

double do_something(void)
{
        int j;
        double x = 0.0;
        struct timeval tv1, tv2;

        gettimeofday(&tv1, NULL);
        for (j = 0; j < JMAX; j++)
                x += 1.0 / (exp((1 + x * x) / (2 + x * x)));
        gettimeofday(&tv2, NULL);
        return GET_ELAPSED_TIME(tv1, tv2);
}

int main(int argc, char *argv[])
{
        int niceval = 0, nsched;
        /* readings are in nanosecs */
        double scale = 1e9;

        long ticks_cpu, ticks_sleep;
        pid_t pid;
        FILE *fp;
        char fname[256];
        double elapsed_time, timeslice, t_cpu, t_sleep;

        if (argc > 1)
                niceval = atoi(argv[1]);
        pid = getpid();

        /* give a chance for other tasks to queue up */
        sleep(3);

        sprintf(fname, "/proc/%d/schedstat", pid);
        /*    printf ("Fname = %s\n", fname); */

        fp = fopen(fname, "r");
        if (!fp) {
                printf("Failed to open stat file\n");
                exit(EXIT_FAILURE);
        }
        if (nice(niceval) == -1 && niceval != -1) {
                printf("Failed to set nice to %d\n", niceval);
                exit(EXIT_FAILURE);
        }
        elapsed_time = do_something();

        fscanf(fp, "%ld %ld %d", &ticks_cpu, &ticks_sleep, &nsched);
        
	t_cpu = (float)ticks_cpu / scale;
        
	t_sleep = (float)ticks_sleep / scale;
        
	timeslice = t_cpu / (double)nsched;
        
	printf("\nnice=%3d time=%10g secs pid=%6d"
               "  t_cpu=%10g  t_sleep=%10g  nsched=%5d"
               "  avg timeslice = %10g\n",
               niceval, elapsed_time, pid, t_cpu, t_sleep, nsched, timeslice);
        fclose(fp);

        exit(EXIT_SUCCESS);
}

