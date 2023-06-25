#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/uaccess.h>
#include <linux/cdev.h>

int scdev_open(struct inode* inode, struct file* file);
ssize_t scdev_read(struct file* file, char* buf, size_t len, loff_t* off);
ssize_t scdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
int scdev_release(struct inode* inode, struct file* file);

static struct scull_dev{
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	struct semaphore sem;
	unsigned int access_key;
	struct cdev cdev;
};
struct scull_dev *ncdev;

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = scdev_open,
	.read = scdev_read,
	.write = scdev_write,
	.release = scdev_release
};

static void scull_setup_cdev(struct scull_dev *dev, int index) {
        int err, devno = MKDEV(255, 1 + index);

        cdev_init(&dev->cdev, &fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &fops;
	err =  cdev_add(&dev->cdev, devno, 1);
	if(err) {
		pr_info("Error %d adding scull%d", err, index);
	} else {
		 pr_info(" Char dev loaded");
	}
}

static int __init chardev_init(void)
{
//	struct scull_dev *ncdev;
	int index = 2;

	scull_setup_cdev(ncdev, index);

	return 0;
}

int scdev_open(struct inode* inode, struct file* file)
{
	pr_info( "Device opened");
	pr_info("Inside the %s function\n", __FUNCTION__);
	return 0;
}

ssize_t scdev_read(struct file* file, char* __user buf, size_t len, loff_t* off)

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

ssize_t scdev_write(struct file* file, const char* __user buf, size_t len, loff_t* off)
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

int scdev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	printk(KERN_INFO "device closed");
	return 0;
}

static void __exit  chardev_exit(void)
{
	//unregister_chrdev(255, "Simple Char Dev");
	cdev_del(&ncdev->cdev);

	pr_info("Goodbye World!\n");

}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");

