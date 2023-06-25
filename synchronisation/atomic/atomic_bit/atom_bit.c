#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	unsigned long word = 0;

	pr_info("intial atomic word is %u", word);

	set_bit(0, &word);

	pr_info("After setting bit 0, Atomic word is %u", word);

	set_bit(1, &word);

	pr_info("After setting bit 1, atomic variable  word is %u", word);

	clear_bit(1, &word);

	pr_info("After setting bit 1, atomic variable  word is %u", word);

	change_bit(0, &word);

	pr_info("After changing bit 0, atomic variable  word is %u", word);




	/*
	   atomic_add(5, &v);

	   pr_info("Add 5, atomic variable v is %d", atomic_read(&v));

	   atomic_inc(&v);

	   pr_info("Increment 1: atomic variable v is %d", atomic_read(&v));
	   */

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

