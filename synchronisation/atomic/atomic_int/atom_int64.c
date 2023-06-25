#include <linux/types.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	atomic64_t v = ATOMIC64_INIT(1);

	pr_info("Atomic64 variable v is %lld", atomic64_read(&v));

	atomic64_set(&v, 4);

	 pr_info("Set v to 4, atomic64 variable v is %lld", atomic64_read(&v));

	atomic64_add(5, &v);

	 pr_info("Add 5, atomic64 variable v is %lld", atomic64_read(&v));

	atomic64_inc(&v);

	pr_info("Increment 1: atomic64 variable v is %lld", atomic64_read(&v));


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

