/***************************************************************************//**
 *  \file       test_app.c
 *
 *  \details    Userspace application to test the Device driver
 *
 *  \author     EmbeTronicX
 *
 *  \Tested with Linux raspberrypi 5.10.27-v7l-embetronicx-custom+
 *
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>


int main(int argc, char *argv[])
{
	int fd, j;
	char * device = "/dev/ioctldrv";

	if (argc > 1)
		device = argv[1];
	
	fd = open(device, O_RDWR);
	
	for (j = 10; j < 110; j++)
		printf("ioctl(%d) = %d\n", j, ioctl(fd, j, NULL));
	exit(EXIT_SUCCESS);
}
