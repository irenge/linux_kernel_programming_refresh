#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	pr_info("PAGESIZE = %d\n", PAGE_SIZE);
	return 0;
}
static void __exit my_exit(void)
{
	pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");
