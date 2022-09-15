#include <linux/module.h>
#include <linux/init.h>

unsigned long exported_symbol;

static int __init my_init(void)
{
	pr_info("Module loaded\n");
	return 0;
}
static void __exit my_exit(void)
{
	pr_info("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");
EXPORT_SYMBOL(exported_symbol);

