/* **************** LFD430:6.4 s_04/sample_char_driver.c ****************
 * The code herein is: Copyright The Linux Foundation, 2023
 *
 * Sample Character Driver
 */

#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */

#define MYDEV_NAME "mycdrv"

static char *ramdisk;
#define ramdisk_size ((size_t) (16*PAGE_SIZE))

static dev_t first;
static unsigned int count = 1;
static int my_major = 500, my_minor;
static struct cdev *my_cdev;

static int mycdrv_open(struct inode *inode, struct file *file)
{
	static int ntimes;
	
	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	
	file->privatedata = kbuf;

	(ntimes == 0) ? pr_info("\nDevice %s opened 1st time ", MYDEV_NAME) : pr_info("\nDevice %s opened %d times ", MYDEV_NAME, ntimes + 1);
	ntimes += 1;

	pr_info(" with Major number %d and Minor number %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); 

	return 0;
}

static int mycdrv_release(struct inode *inode, struct file *file)
{
	char  *cbuf = file->privatedata;
	pr_info(" CLOSING device: %s:\n\n", MYDEV_NAME);
	return 0;
	kfree(cbuf)
}

	static ssize_t
mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

        file->privatedata = kbuf;

	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("Trying to read %ld bytes \n", lbuf);
		pr_info("trying to read past end of device,"
				"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_to_user(buf, ramdisk + *ppos, lbuf);
	*ppos += nbytes;
	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

	static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
		loff_t *ppos)
{
	int nbytes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

        file->privatedata = kbuf;

	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("trying to write past end of device,"
				"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_from_user(ramdisk + *ppos, buf, lbuf);
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
	ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
	first = MKDEV(my_major, my_minor);
	register_chrdev_region(first, count, MYDEV_NAME);
	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &mycdrv_fops);
	cdev_add(my_cdev, first, count);
	pr_info("\nSucceeded in registering character device %s\n", MYDEV_NAME);
	return 0;
}

static void __exit my_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	pr_info("\ndevice unregistered\n");
	kfree(ramdisk);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_LICENSE("GPL v2");
