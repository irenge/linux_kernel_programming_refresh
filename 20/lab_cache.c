#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

static struct kmem_cache *my_cache;
static int size = 41 * PAGE_SIZE;
static char *kbuf;
int sz;

module_param(size, int, 0444);

static int __init my_init(void)
{
	if (size > (1024 * PAGE_SIZE) ) {
		pr_err("size=%d is too large, you can not have more than 1024 pages!\n", size);
		return -1;
	}

	my_cache = kmem_cache_create("mycache", size, 0,  SLAB_HWCACHE_ALIGN, NULL);
	if (!my_cache) {
		pr_err("failed to create a cache object\n");
		return -ENOMEM;
	}
	pr_info("allocated memory cache correctly\n");

	kbuf = kmem_cache_alloc(my_cache, GFP_ATOMIC);
	if(!kbuf) {
		pr_err("kmem_cache_create failed\n");
		return -ENOMEM;
	}
	pr_info(" successfully created a cache object\n");
	
	pr_info("  size = %d\n", size);

	sz = kmem_cache_shrink(my_cache);
	pr_info(" Shrinked by %d : size = %d\n",sz, size);
	sz = kmem_cache_shrink(my_cache);
        pr_info(" Shrinked by %d : size = %d\n",sz, size);

	
	return 0;
}
static void __exit my_exit(void)
{
	/* destroy a memory cache object */
	kmem_cache_free(my_cache, kbuf);
	pr_info("destroyed a memory cache object\n");

	/* destroy the memory cache */
	(void)kmem_cache_destroy(my_cache);
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

