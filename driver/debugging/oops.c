#include<linux/init.h>
#include<linux/module.h>

static int hello_init(void)
{
        printk(KERN_ALERT "Hello, world\n");
        *(int *)0 = 0;
        return 0;
}

void hello_exit(void)
{
	pr_info("Bye");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_INFO(intree, "Y");
MODULE_DESCRIPTION("debug show");
MODULE_VERSION("1.0.0");

