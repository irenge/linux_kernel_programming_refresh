#include<linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>

static struct device *misc_dev;

#define MISC_DEV_NAME "sample_misc"

static size_t kbuf_size = (16 * PAGE_SIZE);
char *kbuf;

// functionality
static int misc_open(struct inode *inode, struct file *file)
{
	static int counter;

	dev_info(misc_dev, "Trying to open device %s ", MISC_DEV_NAME);

	dev_info(misc_dev, " Major number = %d and Minor number = %d\n", imajor(inode), iminor(inode));
	counter++;

	dev_info(misc_dev, "\nDevice %s opened %d times ", MISC_DEV_NAME, counter);

	dev_info(misc_dev, "ref=%d\n", (int)module_refcount(THIS_MODULE));

	return 0;

}
static int misc_release(struct inode *inode, struct file *file)
{
	dev_info(misc_dev, " misc device closed: %s\n", MISC_DEV_NAME);
	return 0;
}
static ssize_t misc_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int nbytes, maxbytes, bytes_to_read;

	maxbytes = kbuf_size - *f_pos;

	bytes_to_read = maxbytes > count ? count : maxbytes;

	if (bytes_to_read == 0)
		dev_warn(misc_dev, "\nYou have reached the end of the device on a read");

	nbytes = count - copy_to_user(buf, kbuf + *f_pos, bytes_to_read);

	*f_pos += nbytes;

	dev_info(misc_dev,"Leaving the READ function, nbytes=%d, pos=%d\n", nbytes, (int)*f_pos);

	return nbytes;
}
static ssize_t misc_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	int nbytes, maxbytes, bytes_to_write;

	maxbytes = kbuf_size - *ppos;

	bytes_to_write = maxbytes > len ? len : maxbytes;

	if (bytes_to_write == 0)
		dev_warn(misc_dev, "\nYou have reached the end of the device on a write");

	nbytes = len - copy_from_user( kbuf + *ppos, buf, bytes_to_write);

	*ppos += nbytes;

	dev_info(misc_dev,"Leaving the READ function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);

	dev_info(misc_dev, "\nmisc device write\n");
	return nbytes;
}
// file operation structure

static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = misc_read,
	.write = misc_write,
	.open = misc_open,
	.release = misc_release,
};

// Device structure
struct miscdevice sample_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_DEV_NAME,
	.fops = &mycdrv_fops,
};

// Misc init function 
static int __init misc_init(void)
{
	int err;

	kbuf = kmalloc(kbuf_size, GFP_KERNEL);

	err = misc_register(&sample_misc_device);
	pr_info("\nregistering %d \n",err);
	if (err != 0) {

		pr_err("\nsample misc registration of %d failed !!\n", sample_misc_device.minor);
		return err;
	}

	misc_dev = sample_misc_device.this_device;
	dev_info(misc_dev,"\nsample misc registered!!\n");

	return 0;
}

static void __exit misc_exit(void)
{
	misc_deregister(&sample_misc_device);
	pr_info("\nSample misc device unregistered: exit !\n");
	kfree(kbuf);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - Misc Driver");
MODULE_VERSION("1.0");
