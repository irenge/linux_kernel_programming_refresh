#include <linux/module.h>
#include <linux/init.h>

void mod1fun(void);
extern int foo;

static int __init my_init(void)
{
        pr_info("Hello world from mod2: %d\n", foo);
        mod1fun();
        return 0;
}

static void __exit my_exit(void)
{
        pr_info("Goodbye world from mod2\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD430:6.4 s_05/lab_module2.c");
MODULE_LICENSE("GPL v2");
                         
