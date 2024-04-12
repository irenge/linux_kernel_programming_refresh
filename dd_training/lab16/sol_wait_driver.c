/*
 * Exercise by LInux Fopundation training 
 *
 *  Using Wait Queues (wait_event(), 1 sleeper wakes up solution)
 *
 *  Generalize the previous character driver to use wait queues,
 *
 *  Have the read() function go to sleep until woken by a write()
 *  function.  (You could also try reversing read and write.)
 *
 *  You may want to open up two windows and read in one window and
 *  then write in the other window.
 *
 *  Try putting more than one process to sleep, i.e., run your test
 *  read program more than once simultaneously before running the
 *  write program to awaken them.  If you keep track of the pid's you
 *  should be able to detect in what order processes are woken.
 *
 *
 *      Using wait_event_interruptible().  You may want to use atomic
 *      functions for any global variables used in the logical
 *      condition.
 *
 *      Using wait_for_completion().
 *
 *
 *      Using exclusive waiting on the many readers solution..  How
 *      many processes wake up?
 *
 *  If you test with cat, echo, or dd, you may see different results
 *  than if you use the supplied simple read/write programs.  Why?
 */



#include <linux/sched.h>
#include "functions.h"

DECLARE_WAIT_QUEUE_HEAD(wq);
static atomic_t data_ready;

//static dev_t first;
//int major_, minor_;

//static unsigned int count = 1;
//static struct cdev *my_cdev;

static int mycdrv_open(struct inode *inode, struct file *file)
{
return generic_open(inode, file);
}

static int mycdrv_release(struct inode *inode, struct file *file)
{

	return generic_release(inode, file);
}

	static ssize_t
mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	pr_info("task %i (%s) going to sleep\n", current->pid, current->comm);
	wait_event_interruptible(wq, atomic_read(&data_ready));
	pr_info("awoken %i (%s) \n", current->pid, current->comm);
	if(signal_pending(current))
		return -ERESTARTSYS;
	atomic_set(&data_ready, 0);

	return generic_read(file, buf, lbuf, ppos);
}

	static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
		loff_t *ppos)
{

	pr_info("task %i (%s) awakening sleepers\n", current->pid, current->comm);
	atomic_set(&data_ready, 1);
	wake_up_interruptible(&wq);
	return generic_write(file, buf, lbuf, ppos);
}

static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = mycdrv_read,
	.write = mycdrv_write,
	.open = mycdrv_open,
	.release = mycdrv_release,
};

static int __init my_init(void)
{

	atomic_set(&data_ready, 0);

	return generic_init();
}


module_init(my_init);
module_exit(generic_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("Task on wait_queue");
MODULE_LICENSE("GPL v2");
