#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/uaccess.h>

int sdev_open(struct inode* inode, struct file* file);
ssize_t sdev_read(struct file* file, char* buf, size_t len, loff_t* off);
ssize_t sdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
int sdev_release(struct inode* inode, struct file* file);

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = sdev_open,
	.read = sdev_read,
	.write = sdev_write,
	.release = sdev_release
};


static int __init chardev_init(void)
{
	register_chrdev(255 /* Major Number */, "Simple Char Dev" /*Name of the driver */ , &fops/* File Operations */);
	pr_info(" Char dev loaded");
	return 0;
}

int sdev_open(struct inode* inode, struct file* file)
{
	pr_info( "Device opened");
	pr_info("Inside the %s function\n", __FUNCTION__);
	return 0;
}

ssize_t sdev_read(struct file* file, char* __user buf, size_t len, loff_t* off)

{
	pr_info("Inside the %s function\n", __FUNCTION__);


	/*	if(*off >= sizeof(kern_buf))
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

ssize_t sdev_write(struct file* file, const char* __user buf, size_t len, loff_t* off)
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

int sdev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	printk(KERN_INFO "device closed");
	return 0;
}

static void __exit  chardev_exit(void)
{
	unregister_chrdev(255, "Simple Char Dev");
	printk(KERN_INFO "Goodbye World!\n");

}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");

