/*
 * <PD> Module 1 for demonstration of circular dependancy </PD>
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/semaphore.h>

extern struct semaphore  sem_var;
extern void print_hello(int);
extern void add_two_numbers(int, int);
extern int GLOBAL_VARIABLE;


static int __init my_init(void)
{

	pr_info("Trying to load %s\n", KBUILD_MODNAME);
	pr_info("semaphore_count=%u\n", sem_var.count);

#if 0
	if (down_trylock(&sem_var) {
			pr_info("Unable to load. down_trylock failed:  %s\n", KBUILD_NAME);
			return -EBUSY;
#else
			if (down_interruptible (&sem_var)) {
			pr_info("Not loading  %s, interrupted by signal\n", KBUILD_MODNAME);
			return -EBUSY;
			}
#endif
			pr_info("\n%s grabbed semaphore x\n", KBUILD_MODNAME);
			print_hello(10);
			add_two_numbers(5, 6);
			pr_info("Value of GLOBAL_VARIABLE %d", GLOBAL_VARIABLE);


			return 0;
			}
			static void __exit my_exit(void) {
			
			pr_info("Bye from Export Symbol 1 module.");
			up(&sem_var);
			pr_info("Exiting semaphores \n");
			}

			module_init(my_init);
			module_exit(my_exit);

			MODULE_DESCRIPTION("Module");
			MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com>");
			MODULE_LICENSE("GPL v2");
