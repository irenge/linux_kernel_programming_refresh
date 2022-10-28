#include <stdio.h>
int main(int argc, char **argv)
{
	const char array[]="abd...";


	printf("%p\n", array);
	*(int *)0=0;
}

