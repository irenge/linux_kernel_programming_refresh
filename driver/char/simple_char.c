#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");

//static char kern_buf[200];
static int dev_open(struct inode* inode, struct file* file);
static ssize_t dev_read(struct file* file, char* buf, size_t len, loff_t* off);
static ssize_t dev_write(struct file* file, const char* buf, size_t len, loff_t* off);
static int dev_release(struct inode* inode, struct file* file);

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release
};


static int __init chardev_init(void)
{
	register_chrdev(255, "medev" , &fops);
	
	/*if(t<0) {
		pr_info( "Problem registering character device \n");
		return EIO;
	}
	*/
	pr_info(" Char dev loaded");
	return 0;
}

static int dev_open(struct inode* inode, struct file* file)
{
	pr_info( "Device opened");
	pr_info("Inside the %s function\n", __FUNCTION__);
	return 0;
}

static ssize_t dev_read(struct file* file, char* buf, size_t len, loff_t* off)

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
	return len;
}

static ssize_t dev_write(struct file* file, const char* buf, size_t len, loff_t* off)
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

static int dev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	printk(KERN_INFO "device closed");
	return 0;
}

static void __exit  chardev_exit(void)
{
unregister_chrdev(255, "medev");
printk(KERN_INFO "Goodbye World!\n");

}

module_init(chardev_init);
module_exit(chardev_exit);
