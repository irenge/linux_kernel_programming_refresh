#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include <linux/cdev.h>

#define scull_major 0
#define scull_minor 0
#define scull_nrdevices 1
#define DEV_CLASS "CustomDeviceClass"
#define BUFFER_LENGTH 1024

int chrdev_open(struct inode* inode, struct file* file);
ssize_t chrdev_read(struct file* file, char* buf, size_t len, loff_t* off);
ssize_t chrdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
int chrdev_release(struct inode* inode, struct file* file);

/* Variables for device and device class */

int result;
dev_t dev;
int scullmajor, scullminor;
static struct class *dev_class;
static struct cdev my_device;

const char *name = "newstart";
static char cust_dev_buffer[BUFFER_LENGTH];
static size_t cust_dev_buffer_index = 0;

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
	int to_copy, not_copied, delta;

	pr_info("Inside the %s function\n", __FUNCTION__);


	to_copy = min(len, cust_dev_buffer_index);

	/* Copy the internal data from the internal buffer to the user. */
	not_copied = copy_to_user(buf, cust_dev_buffer, to_copy);


	/* Determine the actual number of copied bytes */
	delta = to_copy - not_copied;

	pr_info("User requested to read %ld bytes from the device: actually %d bytes have been read\n", len, delta);

	return delta;
}


/*      if(*off >= sizeof(kern_buf))
	return 0;

	if (len > sizeof(kern_buf)){
	printk(KERN_ERR "Length is bigger!");
	len = sizeof(kern_buf);
	}

	copy_to_user(buf, kern_buf, len);

	return len;
	*/
//return delta;
//}

ssize_t chrdev_write(struct file* file, const char* __user user_buf, size_t count, loff_t* off)
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
	/* This time, the user buffer is a source of data, so it is not an internal variable: it contains fixed, given data */

	int to_copy, not_copied, delta;
	pr_info("Inside the %s function\n", __FUNCTION__ );


	/* Determine the amount of data to be written into the buffer. If `count' exceeds the size of the buffer,
	 * write only sizeof(cust_dev_buffer) characters. This is a security precaution similar to the one for driver_read,
	 * as regards unauthorized user writes in the kernel-space. */
	to_copy = min(count, sizeof(cust_dev_buffer));

	/* Write into the internal buffer the data provided by the user. If cust_dev_buffer_index was non-zero, that is if the
	 * cust_dev_buffer was non-empty, this overwrites it starting from its beginning. */
	not_copied = copy_from_user(cust_dev_buffer, user_buf, to_copy);

	/* Determine the actual number of written bytes */
	delta = to_copy - not_copied;

	/* The new actual amount of data inside the buffer */
	cust_dev_buffer_index = delta;

	pr_info("User requested to write %ld bytes into the device internal buffer: actually %d bytes have been written\n", count, delta);

	/* Terminate the string with NULL into the internal buffer */
	cust_dev_buffer[delta] = 0;

	pr_info("The device internal buffer has the following contents: %s\n", cust_dev_buffer);

	return delta;
}

int chrdev_release(struct inode* inode, struct file* file)
{
	pr_info("Inside the %s function\n", __FUNCTION__);
	pr_info("device closed");
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
	pr_info("%s - Device number (with Major: %i, Minor: %i) was registered\n", name, scullmajor, scullminor);	
	/* Create a device class first */
	if ((dev_class = class_create(THIS_MODULE, DEV_CLASS)) == NULL) {
		pr_info("Device class can not be created !\n");
		goto ClassError;
	}

	/* Create a device file */
	if (device_create(dev_class, NULL, dev, NULL, name) == NULL) { 
		pr_info("Can not create device file\n");
		goto FileError;
	}

	/* Initialze device */
	cdev_init(&my_device, &fops);

	/* Register device to kernel */
	if (cdev_add(&my_device, dev, 1) == -1) {
		pr_info("registering of device to kernel failed\n");
		goto AddError;
	}

	return 0;
AddError:
	device_destroy(dev_class, dev);
FileError:
	class_destroy(dev_class);
ClassError:
	unregister_chrdev_region(dev, scull_nrdevices);
	return -1;

}



static void __exit  chardev_exit(void)
{
	cdev_del(&my_device);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
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


