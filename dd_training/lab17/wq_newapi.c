#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */
#include <linux/fs.h>
#include <linux/workqueue.h> 

#define MYDEV_NAME "mycdrv"

static struct device *my_dev;
static char *ramdisk;
static size_t ramdisk_size = (16 * PAGE_SIZE);
static dev_t first;
static unsigned int count = 1;  /* number of dev_t needed */
static struct cdev *my_cdev;
static struct class *foo_class;
static struct workqueue_struct *my_workq;

/* workqueue  section */
// declare a struct variable my_data

static struct my_dat {
	atomic_t count;
	struct work_struct work;
} my_data;

static void wq_fun(struct work_struct *w_arg) 
{
	struct my_dat *data = container_of (w_arg, struct my_dat, work);

	dev_info(my_dev, "\nIn %s, Jiffies = %ld: Task pid %d\n", __func__, jiffies, (int) current->pid);
	atomic_inc(&data->count);
	dev_info(my_dev, "\ncounter: %d\n", atomic_read(&data->count));

}

static int mycdrv_open(struct inode *inode, struct file *file)
{
	static int counter;

	dev_info(my_dev, " OPENING device: %s:\n\n", MYDEV_NAME);
	dev_info(my_dev, " MAJOR number = %d, MINOR number = %d\n",
			imajor(inode), iminor(inode));
	counter++;

	dev_info(my_dev, " successfully open  device: %s:\n\n", MYDEV_NAME);
	dev_info(my_dev, "I have been opened  %d times since being loaded\n",
			counter);
	dev_info(my_dev, "ref=%d\n", (int)module_refcount(THIS_MODULE));

	return 0;
}

static int mycdrv_release(struct inode *inode, struct file *file)
{
	dev_info(my_dev, " CLOSING device: %s:\n\n", MYDEV_NAME);
	return 0;
}

static ssize_t mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{

	int nbytes, maxbytes, bytes_to_do;

	maxbytes = ramdisk_size - *ppos;
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		dev_warn(my_dev, "Reached end of the device on a read");

	nbytes = bytes_to_do - copy_to_user(buf, ramdisk + *ppos, bytes_to_do);
	*ppos += nbytes;
	dev_info(my_dev, "Leaving the   READ function, nbytes=%d, pos=%d\n",
			nbytes, (int)*ppos);
	return nbytes;

}

	static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
		loff_t *ppos)
{
	struct my_dat *data = (struct my_dat *)&my_data;

	int nbytes, maxbytes, bytes_to_do;

	dev_info(my_dev, " Entering the WRITE function\n");
	dev_info(my_dev, " my current task pid is %d\n", (int)current->pid);
	dev_info(my_dev, "about to schedule workqueue,  jiffies=%ld\n",
			jiffies);
	//schedule_work(&data->work);
	queue_work(my_workq, &data->work);
	/*     */

	dev_info(my_dev, " i queued the task, jiffies=%ld\n", jiffies);
	atomic_add(1, &data->count);

	maxbytes = ramdisk_size - *ppos;
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		dev_warn(my_dev, "Reached end of the device on a write");
	nbytes =
		bytes_to_do - copy_from_user(ramdisk + *ppos, buf, bytes_to_do);
	*ppos += nbytes;
	dev_info(my_dev, "Leaving the   WRITE function, nbytes=%d, pos=%d\n",
			nbytes, (int)*ppos);
	return nbytes;

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
	struct my_dat *data = (struct my_dat *) &my_data;
	//data->counter = atomic_init(0);
	my_workq = alloc_workqueue("my_workq", WQ_UNBOUND | WQ_FREEZABLE, 1);
	atomic_set(&data->count, 1);

	INIT_WORK(&data->work, wq_fun);

	if (alloc_chrdev_region(&first, 0, count, MYDEV_NAME) < 0) {
		pr_err("failed to allocate character device region\n");
		return -1;
	} 
	//atomic_set(&data->len, 100);

	my_cdev = cdev_alloc();
	if (!my_cdev) {
		pr_err("cdev_alloc() failed\n");
		unregister_chrdev_region(first, count);
		return -1;
	}
	my_cdev->ops = &mycdrv_fops;

	ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);

	if (cdev_add(my_cdev, first, count) < 0) {
		pr_err("cdev_add() failed\n");
		cdev_del(my_cdev);
		unregister_chrdev_region(first, count);
		kfree(ramdisk);ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);

		return -1;
	}

	foo_class = class_create("my_class");
	my_dev = device_create(foo_class, NULL, first, NULL, "%s", "mycdrv");
	dev_info(my_dev, "\nSucceeded in registering character device %s\n",
			MYDEV_NAME);
	dev_info(my_dev, "Major number = %d, Minor number = %d\n", MAJOR(first),
			MINOR(first));

	return 0;

}

static void __exit my_exit(void)
{
	/*	cdev_del(my_cdev);
		unregister_chrdev_region(first, count);
		pr_info("\ndevice unregistered\n");
		kfree(ramdisk); 
		*/
	destroy_workqueue(my_workq);

	dev_info(my_dev, "Uregistering Device\n");
	device_destroy(foo_class, first);
	class_destroy(foo_class);

	if (my_cdev)
		cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	kfree(ramdisk);

}


module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("J");
MODULE_LICENSE("GPL v2");
