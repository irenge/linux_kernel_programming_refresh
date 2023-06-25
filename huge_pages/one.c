#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

int main() 
{
	char *filename = "/tmp/hugetlp.txt";
	
	int fd;
	fd = open(filename, O_CREAT | O_RDWR, 0755);
        if (fd )
		printf("success");

	return 0;
}

