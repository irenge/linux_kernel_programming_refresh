
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/version.h>
#include "functions.h"

static DEFINE_MUTEX(mymutex);

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
	pr_info("Process %i (%s) going to sleep\n", current->pid, current->comm);
	if(mutex_lock_interruptible(&mymutex)) {
		pr_info("\nMutex unlocked by signal in %s\n", KBUILD_MODNAME);
		return -EBUSY;
	}
	pr_info("Process  %i (%s) is awakening\n", current->pid, current->comm);


	return generic_read(file, buf, lbuf, ppos);
}

	static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
		loff_t *ppos)
{

	pr_info("Process %i (%s) awakening readers\n", current->pid, current->comm);
	mutex_unlock(&mymutex);
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
	if(mutex_lock_interruptible(&mymutex)) {
		pr_info("\nMutex unlocked by signal in %s\n", KBUILD_MODNAME);
		return -EBUSY;
	}

	return generic_init();
}


module_init(my_init);
module_exit(generic_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("Task on wait_queue");
MODULE_LICENSE("GPL v2");
