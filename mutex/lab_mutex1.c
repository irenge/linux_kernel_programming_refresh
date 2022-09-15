#include <linux/module.h>
#include <linux/init.h>

DEFINE_MUTEX(var_mutex);
EXPORT_SYMBOL(var_mutex);

int MUTEX_VAR = 10;

//char *modname = __stringify(KBUILD_MODNAME);

EXPORT_SYMBOL(MUTEX_VAR);

/*
 *
 */

void print_hello(int num)
{
	while (num--) {
		pr_info("Hello Friend!!!\n");
	}
}

EXPORT_SYMBOL(print_hello);
/*
 * 
 */

void add_two_numbers (int a, int b)
{
	pr_info("Sum of the numbers %d", a + b);
}

EXPORT_SYMBOL(add_two_numbers);

static int __init my_init(void)
{

	pr_info("Trying to load module %s with  %s\n", KBUILD_MODNAME, __func__);
#if 0
	/* this branch should hang if not available */
	if (mutex_lock_interruptible(&my_mutex)) {
		pr_info("mutex unlocked by signal in %s\n", KBUILD_MODNAME);
		return -EBUSY;
	}
#else 
	if(!mutex_trylock(&var_mutex))wq
	
	return 0;
}


static void __exit my_exit(void)
{
	pr_info("Bye: from Export Symbol 1 module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");
