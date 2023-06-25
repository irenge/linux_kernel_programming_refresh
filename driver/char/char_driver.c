#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define scull_major 0
#define scull_minor 0
#define scull_nrdevices 1

int chrdev_open(struct inode* inode, struct file* file);
ssize_t chrdev_read(struct file* file, char* buf, size_t len, loff_t* off);
ssize_t chrdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
int chrdev_release(struct inode* inode, struct file* file);

int result;
dev_t dev;
int scullmajor, scullminor;
const char *name = "newstart";

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = chrdev_open,
	.read = chrdev_read,
	.write = chrdev_write,
	.release = chrdev_release
};

int chrdev_open(struct inode* inode, struct file* file)
{
	pr_info( "Device opened");
	pr_info("Inside the %s function\n", __FUNCTION__);
	return 0;
}
ssize_t chrdev_read(struct file* file, char* __user buf, size_t len, loff_t* off)

{
	pr_info("Inside the %s function\n", __FUNCTION__);


	/*      if(*off >= sizeof(kern_buf))
		return 0;

		if (len > sizeof(kern_buf)){
		printk(KERN_ERR "Length is bigger!");
		len = sizeof(kern_buf);
		}

		copy_to_user(buf, kern_buf, len);

		return len;
		*/
	return 0;
}

ssize_t chrdev_write(struct file* file, const char* __user buf, size_t len, loff_t* off)
{
	pr_info("Inside the %s function\n", __FUNCTION__ );
	/*
	   if(len >= sizeof(kern_buf))
	   return -EIO;
	   memset(kern_buf, 0, sizeof(kern_buf));
	   copy_from_user(kern_buf, buf, len);
	   kern_buf[len]=0;
	   return len;
	   */
	return len;
}

int chrdev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	printk(KERN_INFO "device closed");
	return 0;
}


static int __init chardev_init(void)
{
	if(scull_major) {
		scullmajor = scull_major;
		scullminor = scull_minor;
		dev = MKDEV(scullmajor, scullminor);
		pr_info("Using register_chrdev_region ...");
		result = register_chrdev_region(dev, scull_nrdevices, name);
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nrdevices, name);
		pr_info("Using alloc_chrdev_region ...");
		scullmajor = MAJOR(dev);
		scullminor = MINOR(dev);
		pr_info("%s device allocated, dev [%i, %i]\n", name,
				scullmajor, scullminor);
	}
	if (result <  0) {
		pr_warn("scull: can't get major %d\n", scullmajor);
		return result;
	}
	//register_chrdev(255 // Major Number , "Simple Char Dev" // Name of the driver  , &fops // File Operations );
	pr_info(" Char device loaded");
	
	/* Create a device class first
	 */

	return 0;

}



static void __exit  chardev_exit(void)
{
	//unregister_chrdev(255, "Simple Char Dev");
	unregister_chrdev_region(dev, scull_nrdevices);
	pr_info("Goodbye World!\n");

}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_INFO(intree, "Y");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");


