#include <stdio.h>

#define __NR_foo 382
__syscall0(long, foo)

int main ()
{
	long stack_size;

	stack_size = foo();
	printf("The kernel  stack size is %ld\n", stack_size);

	return 0;
}
