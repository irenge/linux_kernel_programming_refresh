/*
 * <PD> Module 2 for exporting symbol demostration </PD>
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/errno.h>

extern struct  mutex mutex_var;

extern void print_hello(int);
extern void add_two_numbers(int, int);
extern int GLOBAL_VARIABLE;

/*
 * The function has been written just to call the functions which are in other module. This way you can also write modules which does provide some functionality to the other modules.
 */
static int __init my_init(void)
{
	pr_info("Trying to load %s\n",__func__, KBUILD_MODNAME);
#if 0
	if (mutex_lock_interruptible(&mutex_var) {
			pr_info("mutex unlocked by signal in %s\n", KBUILD_MODNAME);
			return -EBUSY;
#else
			if (!mutex_trylock(&mutex_var)) {
			pr_info("mutex_trylock failed in %s\n", KBUILD_MODNAME);
			return -EBUSY;
			}
#endif

			pr_info("%s: Mutex grabbed\n", KBUILD_MODNAME);
			print_hello(10);
			add_two_numbers(5, 6);
			pr_info("Value of GLOBAL_VARIABLE %d", GLOBAL_VARIABLE);
			return 0;
			}

			static void __exit my_exit(void)
			{
			mutex_unlock(&mutex_var);
			pr_info("Bye from Hello Module");
			}

			module_init(my_init);
			module_exit(my_exit);

			MODULE_DESCRIPTION("Module to demonstrate the EXPORT_SYMBOL functionality");
			MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com>");
			MODULE_LICENSE("GPL v2");
