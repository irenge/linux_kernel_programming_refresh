#include<module.h>


static unsigned pg_offset(const void __user *addr) {



}


static unsigned long pfn_of(const void __user *addr)
{
	return (unsigned long) addr >> PAGE_SHIFT;
}

