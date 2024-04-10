#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	size_t length;
	int fd;
       size_t bytes_written, bytes_read;

	char* buf;
	char* node = "/dev/mycdrv";




	length = 20;

	if (argc > 1)
		node = argv[1];

	if (argc > 2)
		length = atoi(argv[2]);
	buf = malloc(length);

	memset(buf, 'x', length);
	buf[length-1] ='\0';

	fd = open(node, O_RDWR);
	printf("Opened device node, file descriptor = %d\n", fd);

	bytes_written = write(fd, buf, length);
	printf("return code from write = %d\n", fd);
	printf("bytes_written = %d\n", bytes_written);


	memset(buf, 0, length);
	close(fd);
	fd = open(node, O_RDWR);
	printf("Opened device node, file descriptor = %d\n", fd);

	bytes_read = read(fd, buf, length);
	printf("return code for read = %d\n", bytes_read);
	printf("Message is %s\n", buf);

	close(fd);

	exit(EXIT_SUCCESS);
}

