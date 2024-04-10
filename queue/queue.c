#include <linux/kfifo.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>


/* fifo size in elements (ints) */
#define FIFO_SIZE       32

/* name of the proc entry */
#define PROC_FIFO       "int-fifo"

/* lock for procfs read access */
static DEFINE_MUTEX(read_access);

/* lock for procfs write access */
static DEFINE_MUTEX(write_access);


static DEFINE_KFIFO(qring, int, FIFO_SIZE);

static int __init my_init(void)
{

	int i,j, sz;

	// populate the queue

	for (i = 0; kfifo_put(&qring, i); i++);

	pr_info("===================================================");
	pr_info ("Item\tlength after item removal");
	
	// read the queue 
	
	while(kfifo_get(&qring, &j))
		pr_info("%d\t%u",j, kfifo_len(&qring));


	/* show the first value without removing from the fifo */
	if (kfifo_peek(&qring, &j)) {
		pr_info(" first value of the queue  is %d\n", j);
	} else {
		pr_info("No item in the queue");
	}


	sz = kfifo_size(&qring);

	pr_info("Size of the queue is %d", sz);

	return 0;
}

static void __exit my_exit(void)
{
	pr_info("Bye: queue module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

