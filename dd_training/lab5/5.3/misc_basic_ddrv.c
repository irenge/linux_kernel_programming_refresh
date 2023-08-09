/*
 * Take your basic character driver and adapt it to use
 * dynamic loading with request_module().
 *
 * Construct a trivial second module and have it dynamically loaded
 * during the character driver's open entry point. (Make sure the name
 * of the file that is requested is the same as the name of your
 * file.)
 *
 * Add a small function to your character driver and have it
 * referenced by the second module.
 *
 * Make sure you place your modules in a place where
 * modprobe can find them, (Installing with the target
 * modules_install will take care of this for you.)
 *
 * You can use either cat or the main program from the character
 * driver lab to exercise your module.  What happens if you try to
 * request loading more than once?
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/sched.h>

/*
   int scdev_open(struct inode* inode, struct file* file);
   ssize_t scdev_read(struct file* file, char* buf, size_t len, loff_t* off);
   ssize_t scdev_write(struct file* file, const char* buf, size_t len, loff_t* off);
   int scdev_release(struct inode* inode,  file* file);
   */

#define DEV_NAME "basicdriver"

static struct device *mdev;

#define space_size ((size_t) (32*PAGE_SIZE))
/*
   void mod_fun(void);
   void mod_fun(void)
   {
   pr_info("\nVOILA! I got into %s\n\n", __func__);
   }
   EXPORT_SYMBOL(mod_fun);
   */
/*
   static struct module *find_insert_module(const char *name)
   {
   struct module *list_mod = NULL;
// traverse the module list and find corresponding module
list_for_each_entry(list_mod, THIS_MODULE->list.prev, list)
{
if (strcmp(list_mod->name, name) == 0)
{
return list_mod;
}
}
// printk("[%s] module %s not found\n", THIS_MODULE->name, name);
//printk("[%s] module %s not found\n", THIS_MODULE->name, name);
return NULL;
}
*/
static inline int scdev_open(struct inode* inode, struct file* file)
{

	//	const char *name;
	//struct module *xmodule;

	static int counter;

	dev_info(mdev, "Tryig to open device %s:\n", DEV_NAME);
	dev_info(mdev, " MAJOR number = %d, MINOR number = %d\n",
			imajor(inode), iminor(inode));

	counter++;

	//pr_info( "\nDevice opened\n");
	pr_info("\nInside the %s function\n", __FUNCTION__);
	dev_info(mdev, " successfully open  device: %s:\n\n", DEV_NAME);
	dev_info(mdev, "I have been opened  %d times since being loaded\n", counter);
	dev_info(mdev, "ref=%d\n", (int)module_refcount(THIS_MODULE));


	//	pr_info("%d and %d is %d\n", 2, 6, avgx(2,6));
	/*
	   name = "trivial";

	   xmodule = find_insert_module(name);
	   if (!xmodule) {
	   ret = request_module(name);
	   if (!ret) {
	   pr_info("trivial module is loaded\n");
	   } else {
	   pr_info("\n trivial does not exist\n");
	   }
	   }
	   */
	return 0;
}

static int mmb_open(struct inode* inode, struct file* file) {

	dev_info(mdev, "rc from requesting module mod_fun is: %d\n", request_module("%s", "lab_module2"));

	return scdev_open(inode, file);
}

static ssize_t scdev_read(struct file* file, char* __user buf, size_t len, loff_t* off) {
	pr_info("\nInside the %s function\n", __FUNCTION__);

	return 0;
}

static ssize_t scdev_write(struct file* file, const char* __user buf, size_t len, loff_t* off)
{
	pr_info("\nInside the %s function\n", __FUNCTION__ );

	return len;
}

static int scdev_release(struct inode* inode, struct file* file)
{
	//pr_info("Inside the %s function\n", __FUNCTION__);
	//printd();
	//pr_info("device closed");
	dev_info(mdev, " closing device: %s:\n\n", DEV_NAME);

	return 0;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = mmb_open,
	.read = scdev_read,
	.write = scdev_write,
	.release = scdev_release
};

static struct miscdevice mdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &fops,
};


static int __init chardev_init(void)
{

	//spacex = kmalloc(space_size, GFP_KERNEL);
	//guess = MKDEV(major, minor);

	/* registering the device */

	if (misc_register(&mdevice)) {
		return -EBUSY;
	}

	// initialize the device 
	mdev = mdevice.this_device;

	dev_info(mdev, "Succeeded in registering device %s\n", DEV_NAME);

	return 0;
}


static void __exit  chardev_exit(void)
{
	misc_deregister(&mdevice);
	pr_info("\nGoodbye World!\n");
	//kfree(spacex);

}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("1.0.0");

