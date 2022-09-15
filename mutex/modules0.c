#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>

DEFINE_MUTEX(mutex_var);
EXPORT_SYMBOL(mutex_var);


int GLOBAL_VARIABLE = 1000;

EXPORT_SYMBOL(GLOBAL_VARIABLE);

/*
 * Function to print hello for num times.
 */

void print_hello(int num)
{
	while (num--) {
		printk(KERN_INFO "Hello Friend!!!\n");
	}
}
EXPORT_SYMBOL(print_hello);

/*
 * Function to add two passed number.
 */

void add_two_numbers(int a, int b)
{
	printk(KERN_INFO "Sum of the numbers %d", a + b);
}

EXPORT_SYMBOL(add_two_numbers);



static int __init start_fun(void)
{
	pr_info("%s:The  mutex var in unlocked state in %s \n", __func__, KBUILD_MODNAME);
	return 0;
}
static void __exit end_fun(void)
{
	pr_info("Bye mutex world \n!");
}

module_init(start_fun);
module_exit(end_fun);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");

