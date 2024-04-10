#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */

#define MYDEV_NAME "mycdrv"

#define ramdisk_size ((size_t) (16*PAGE_SIZE))

static dev_t first;

int major_, minor_;

static unsigned int count = 1;

static struct device *my_dev;

static struct cdev *my_cdev;

static int mycdrv_open(struct inode *inode, struct file *file)
{
	static int ntimes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

	file->private_data = kbuf;

	(ntimes == 0) ? pr_info("\nDevice %s opened 1st time ", MYDEV_NAME) : pr_info("\nDevice %s opened %d times ", MYDEV_NAME, ntimes + 1);
	ntimes += 1;

	pr_info(" with Major number %d and Minor number %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); 

	pr_info("ref=%d\n", (int)module_refcount(THIS_MODULE));

	return 0;
}

static int mycdrv_release(struct inode *inode, struct file *file)
{
	char  *cbuf = file->private_data;
	pr_info(" CLOSING device: %s:\n\n", MYDEV_NAME);
	return 0;
	kfree(cbuf);
}

static ssize_t mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
	int nbytes = 0, maxbytes, bytes_to_do;

	static size_t kbuf_size = (32 * PAGE_SIZE);

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

	char* tmp = kbuf + *ppos;

	maxbytes = kbuf_size - *ppos;

	bytes_to_do = maxbytes > lbuf  ? lbuf : maxbytes;

	if (bytes_to_do == 0)
		dev_warn(my_dev, "\nreached the end\n");


	//file->private_data = kbuf;
	// nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);

	while((nbytes < bytes_to_do) && !put_user(*tmp, (buf + nbytes))) {
		nbytes++;
		tmp++;
	}
	*ppos += nbytes;

	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

	static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = 0, maxbytes, bytes_to_do;

	static size_t kbuf_size = (32 * PAGE_SIZE);

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

	char* tmp = kbuf + *ppos;

	maxbytes = kbuf_size - *ppos;

	bytes_to_do = maxbytes > lbuf  ? lbuf : maxbytes;

	if (bytes_to_do == 0)
		dev_warn(my_dev, "\nreached the end\n");


	//file->private_data = kbuf;
	// nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);

	while((nbytes < bytes_to_do) && !get_user(*tmp, (buf + nbytes))) {
		nbytes++;
		tmp++;
	}
	nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
	*ppos += nbytes;
	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
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
	//ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
	//	first = MKDEV(my_major, my_minor);
	//	register_chrdev_region(first, count, MYDEV_NAME);
	int ret;
	ret = alloc_chrdev_region ( &first, 0, count, MYDEV_NAME);
	pr_info("Using dynamic alloc...\n");
	major_ = MAJOR(first);
	minor_ = MINOR(first);
	pr_info("%s device allocated, dev [%i, %i]\n", MYDEV_NAME, major_, minor_);

	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &mycdrv_fops);
	cdev_add(my_cdev, first, count);
	pr_info("\nReturn = %d ==> Succeeded in registering character device %s\n",ret, MYDEV_NAME);
	return 0;
}

static void __exit my_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	pr_info("\ndevice unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
