#include <linux/types.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	atomic_t v = ATOMIC_INIT(1);

	pr_info("Atomic variable v is %d", atomic_read(&v));

	atomic_set(&v, 4);

	 pr_info("Set v to 4, atomic variable v is %d", atomic_read(&v));

	atomic_add(5, &v);

	 pr_info("Add 5, atomic variable v is %d", atomic_read(&v));

	atomic_inc(&v);

	 pr_info("Increment 1: atomic variable v is %d", atomic_read(&v));


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

