#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>

/*
 * This module shows how to create a int type fifo.
 */

/* fifo size in elements (ints) */
#define FIFO_SIZE	32

/* name of the proc entry */
#define	PROC_FIFO	"int-fifo"

/* lock for procfs read access */
static DEFINE_MUTEX(read_access);

/* lock for procfs write access */
static DEFINE_MUTEX(write_access);

/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */
#if 0
#define DYNAMIC
#endif

#ifdef DYNAMIC
static DECLARE_KFIFO_PTR(test, int);
#else
static DEFINE_KFIFO(test, int, FIFO_SIZE);
#endif

static const int expected_result[FIFO_SIZE] = {
	 3,  4,  5,  6,  7,  8,  9,  0,
	 1, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42,
};

static int __init my_init(void)
{


int		buf[6];
	int		i, j;
	unsigned int	ret;

	printk(KERN_INFO "int fifo test start\n");

	/* put values into the fifo */
	for (i = 0; i != 10; i++)
		kfifo_put(&test, i);

	/* show the number of used elements */
	printk(KERN_INFO "fifo len: %u\n", kfifo_len(&test));

	/* get max of 2 elements from the fifo */
	ret = kfifo_out(&test, buf, 2);
	printk(KERN_INFO "ret: %d\n", ret);
	/* and put it back to the end of the fifo */
	ret = kfifo_in(&test, buf, ret);
	printk(KERN_INFO "ret: %d\n", ret);

	/* skip first element of the fifo */
	printk(KERN_INFO "skip 1st element\n");
	kfifo_skip(&test);

	/* put values into the fifo until is full */
	for (i = 20; kfifo_put(&test, i); i++)
		;

	printk(KERN_INFO "queue len: %u\n", kfifo_len(&test));

	/* show the first value without removing from the fifo */
	if (kfifo_peek(&test, &i))
		printk(KERN_INFO "%d\n", i);

	/* check the correctness of all values in the fifo */
	j = 0;
	while (kfifo_get(&test, &i)) {
		printk(KERN_INFO "item = %d\n", i);
		if (i != expected_result[j++]) {
			printk(KERN_WARNING "value mismatch: test failed\n");
			return -EIO;
		}
	}
	if (j != ARRAY_SIZE(expected_result)) {
		printk(KERN_WARNING "size mismatch: test failed\n");
		return -EIO;
	}
	printk(KERN_INFO "test passed\n");
	return  0;
}

static void __exit my_exit(void)
{
	//pr_info("Bye: module unloaded from 0x%p\n", queue_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

