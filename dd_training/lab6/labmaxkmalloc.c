#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>


static int __init mstart(void) {

	static unsigned long x;
	static char *buff;
	int i;

	for(i = PAGE_SIZE, x = 0; x < MAX_ORDER + 1; x++, i *= 2)
	{
		pr_info("x = %2ld, pages=%6ld, size = %7d\n", x, i/PAGE_SIZE, i);
		buff = (char *) kmalloc((size_t) i, GFP_ATOMIC);
		if (!buff) {
			pr_err("\nMemory allocation failed !\n");
		break;
		}
		pr_info("\nKmalloc ... ok !\n");
	}
	

        
	return 0;
}

static void __exit mexit(void) {
}

module_init(mstart);
module_exit(mexit);

MODULE_DESCRIPTION("Reading about memory demonstration");
MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com");
MODULE_LICENSE("GPL v2");

