#include <linux/module.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/errno.h>

extern struct mutex my
extern void print_hello(int);
extern void add_two_numbers(int, int);
extern int MUTEX_VAR;

static int __init my_init(void)
{
	pr_info("Hello: module loaded at 0x%p\n", my_init);
#if 0
        /* this branch should hang if not available */
        if (mutex_lock_interruptible(&my_mutex)) {
                pr_info("mutex unlocked by signal in %s\n", KBUILD_MODNAME);
                return -EBUSY;
        }
#else
        if(!mutex_trylock(&var_mutex)) {
		pr_info("mutex_trylock failed in %s\n", KBUILD_MODNAME);
		return -EBUSY;
	}
#endif

	pr_info("\n%s mutex grabbed mutex\n", KBUILD_MODNAME);

	print_hello(7);
	
	add_two_numbers(5, 2);
	
	pr_info("Value of MUTEX_VAR: %d", MUTEX_VAR);

	return 0;
}
static void __exit my_exit(void)
{
	mutex_unlock(&var_mutex);
	pr_info("Bye: module unloaded from 0x%p: unlocking %s\n", my_exit, KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");
