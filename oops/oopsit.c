#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	int *i;

	i = 0;
	pr_info("Hello: init_module loaded at address 0x%p\n", init_module);
	pr_info("i=%d\n", *i);
	return 0;
}

static void __exit  my_exit(void)
{
	pr_info("Hello: cleanup_module loaded at address 0x%p\n", cleanup_module);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");

