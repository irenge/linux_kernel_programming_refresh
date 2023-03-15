#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

static int __init my_init(void)
{
	static char *kbuf;
	static unsigned long order;
	int size;
	for (size = PAGE_SIZE, order = 0; order < MAX_ORDER + 1; order++, size *= 2) {
		pr_info(" order=%2ld, pages=%5ld, size=%8d ", order, size / PAGE_SIZE, size);
		kbuf = (char *)__get_free_pages(GFP_ATOMIC, order);
		pr_info(" . ");
		if (!kbuf) {
			pr_err("... __get_free_pages failed\n");
			break;
		}
		pr_info("... __get_free_pages OK\n");
		free_pages((unsigned long)kbuf, order);
	}
	for (size = PAGE_SIZE, order = 0; order < MAX_ORDER + 1;
			order++, size *= 2) {
		pr_info(" order=%2ld, pages=%5ld, size=%8d ", order,
				size / PAGE_SIZE, size);
		kbuf = kmalloc((size_t)size, GFP_ATOMIC);
		if (!kbuf) {
			pr_err("... kmalloc failed\n");
			break;
		}
		pr_info("... kmalloc OK\n");
		kfree(kbuf);
	}

	return 0;
}

static void __exit my_exit(void)
{
	pr_info("Module Unloading\n");
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");

