#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/uaccess.h>

#define BUFFER_LENGTH 1024


int sdev_open(struct inode* inode, struct file* file);
ssize_t sdev_read(struct file* file, char* buf, size_t len, loff_t* off);
ssize_t sdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
int sdev_release(struct inode* inode, struct file* file);

const char *name = "newstart";
static char kern_buf[BUFFER_LENGTH];
static size_t kern_buf_index = 0;

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

ssize_t sdev_read(struct file* file, char* __user user_buf, size_t len, loff_t* off)

{
	int to_copy, not_copied, delta;

	pr_info("Inside the %s function\n", __FUNCTION__);

	to_copy = min(len,  kern_buf_index);


	/* Copy the internal data from the internal buffer to the user. */
        not_copied = copy_to_user(user_buf, kern_buf, to_copy);

	/* Determine the actual number of copied bytes */
        delta = to_copy - not_copied;

        pr_info("User requested to read %ld bytes from the device: actually %d bytes have been read\n", len, delta);

        return delta;



	/*	if(*off >= sizeof(kern_buf))
		return 0;

		if (len > sizeof(kern_buf)){
		printk(KERN_ERR "Length is bigger!");
		len = sizeof(kern_buf);
		}

		copy_to_user(buf, kern_buf, len);

		return len;
		*/
	//return 0;
}

ssize_t sdev_write(struct file* file, const char* __user user_buf, size_t len, loff_t* off)
{
//	pr_info("Inside the %s function\n", __FUNCTION__ );
	/*
	   if(len >= sizeof(kern_buf))
	   return -EIO;
	   memset(kern_buf, 0, sizeof(kern_buf));
	   copy_from_user(kern_buf, buf, len);
	   kern_buf[len]=0;
	   return len;
	   */
	//return len;
	     int to_copy, not_copied, delta;
        pr_info("Inside the %s function\n", __FUNCTION__ );


        /* Determine the amount of data to be written into the buffer. If `count' exceeds the size of the buffer,
         * write only sizeof(kern_buf) characters. This is a security precaution similar to the one for driver_read,
         * as regards unauthorized user writes in the kernel-space. */
        to_copy = min(len, sizeof(kern_buf));

        /* Write into the internal buffer the data provided by the user. If kern_buf_index was non-zero, that is if the
         * kern_buf was non-empty, this overwrites it starting from its beginning. */
        not_copied = copy_from_user(kern_buf, user_buf, to_copy);

        /* Determine the actual number of written bytes */
        delta = to_copy - not_copied;

        /* The new actual amount of data inside the buffer */
        kern_buf_index = delta;

        pr_info("User requested to write %ld bytes into the device internal buffer: actually %d bytes have been written\n", len, delta);

        /* Terminate the string with NULL into the internal buffer */
        kern_buf[delta] = 0;

        pr_info("The device internal buffer has the following contents: %s\n", kern_buf);

        return delta;

}

int sdev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	pr_info("device closed");
	return 0;
}

static void __exit  chardev_exit(void)
{
	unregister_chrdev(255, "Simple Char Dev");
	pr_info("Goodbye World!\n");

}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");

