#include <linux/module.h>
#include <linux/init.h>
#include <linux/vmalloc.h>

#define MB (1024*1024)

static long mem = 100000L;
module_param(mem, long, 0444);

static int  __init mstart(void)
{
	static char *vbuff;
	long size;

	for (size = MB; size <= mem * MB; size += 50) {
		pr_info("... pages=%6ld, size=%8ld MB", size / PAGE_SIZE, size / MB);
		vbuff = vmalloc(size);
		if(!vbuff) {
			pr_err("... vmalloc failed");
			break;
		}
		pr_info("vmalloc succeed...\n");
		vfree(vbuff);
	}
	return 0;
}

static void __exit mexit(void)
{
	pr_info("Bye mens\n");
}

module_init(mstart);
module_exit(mexit);


MODULE_AUTHOR("Jules");
MODULE_DESCRIPTION("vmalloc exerise");
MODULE_LICENSE("GPL v2");
