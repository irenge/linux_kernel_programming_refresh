#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */

#define MYDEV_NAME "mycdrv"
#define MYDEV_CLASS "CustomDeviceClass"

//static char *ramdisk;
#define ramdisk_size ((size_t) (16*PAGE_SIZE))

static dev_t first;
int major_, minor_;

char *devname = "mycdrv"; 

static unsigned int count = 1;
//static int my_major = 500, my_minor;
static struct cdev *my_cdev;
static struct class *dev_class;


static int mycdrv_open(struct inode *inode, struct file *file)
{
	static int ntimes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	 if(!kbuf) {
                pr_err("Failed to allocated memory at open\n");
                return -ENOMEM;
        }


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

	static ssize_t
mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	 if(!kbuf) {
                pr_err("Failed to allocated memory at read\n");
                return 0;
        }


	file->private_data = kbuf;

	/*	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("Trying to read %ld bytes \n", lbuf);
		pr_info("trying to read past end of device,"
		"aborting because this is just a stub!\n");
		return 0;
		}
		*/
	nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
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
	if(!kbuf) {
		pr_err("Failed to allocated memory at write\n");
		return 0;
	}

	file->private_data = kbuf;

	/*if ((lbuf + *ppos) > ramdisk_size) {
	  pr_info("trying to write past end of device,"
	  "aborting because this is just a stub!\n");
	  return 0;
	  }
	  */
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
	// dynamic
	if ((alloc_chrdev_region ( &first, 0, count, devname)) < 0) {
		pr_err("failed to allocate character device region\n");
		return -1;
	}
	pr_info("Using dynamic alloc...\n");
	major_ = MAJOR(first);
	minor_ = MINOR(first);
	pr_info("%s device allocated, dev [%i, %i]\n", devname, major_, minor_);

	my_cdev = cdev_alloc();
	if(!my_cdev) {
		pr_err("cdev_alloc failed !");
		unregister_chrdev_region(first, count);
		return -1;
	}
	pr_info("\ndevice registered\n");
	
	// Optional udev thing
	
	/* Create Device class : using udev */
	if ((dev_class = class_create(MYDEV_CLASS)) == NULL) {
                pr_info("Device class can not be created !\n");
                goto ClassError;
        }

	/* Create a device file */
        if (device_create(dev_class, NULL, first, NULL,"%s", devname) == NULL) {
                pr_info("Can not create device file\n");
                goto FileError;
	}
	//

	/* Initialise device */
	cdev_init(my_cdev, &mycdrv_fops);
	
	if(cdev_add(my_cdev, first, count) < 0) {
		pr_err("cdev_add() failed\n");
		goto AddError;
		//cdev_del(my_cdev); /***** investigate ****/ 
               // unregister_chrdev_region(first, count);
	}
	pr_info("\nDevice is live\n");

	//pr_info("\nReturn = %d ==> Succeeded in registering character device %s\n",ret, MYDEV_NAME);
	return 0;

AddError:
        device_destroy(dev_class, first);
FileError:
        class_destroy(dev_class);
ClassError:
        unregister_chrdev_region(first, count);
        return -1;
}

static void __exit my_exit(void)
{
	cdev_del(my_cdev);
	device_destroy(dev_class, first);
        class_destroy(dev_class);
	unregister_chrdev_region(first, count);
	pr_info("\ndevice unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
