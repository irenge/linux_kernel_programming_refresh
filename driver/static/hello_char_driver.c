#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
//#include<linux/fs.h>
//#include<linux/string.h>

#define MAJOR_T 255
#define MINOR_T 0
#define COUNT_T 2

/*static char scull_name[100];
static int scull_open(struct inode* inode, struct file* filp);
static int scull_release(struct inode* inode, struct file* filp);

static struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.open = scull_open,
	.release = scull_release
};



static int scull_open(struct inode* inode, struct file* filp) {

	pr_info(" Device opened ");
	return 0;
}


static int scull_release(struct inode* inode, struct file* filp) {

	pr_info("Device closed");
	return 0;
}
*/
static int __init scull_init(void) 
{
	dev_t x_t;
	int ret;
	int y = 0;

	int scull_minor = MINOR_T;
	int scull_major = MAJOR_T;
	int count = COUNT_T;

	if (!y) {
		pr_info("\nStatic stuff ...");
		x_t = MKDEV(MAJOR_T, MINOR_T);
		ret = register_chrdev_region(x_t, count, "scull_dev");
		//ret = register_chrdev(scull_major, "schull_dev", &scull_fops);

	} else {
		pr_info("\nDynamic allocation stuff");
		ret = alloc_chrdev_region(&x_t, scull_minor, count, "scull_dev");

	}

	if(ret < 0) {
		pr_warn("scull: can't get major %d\n", scull_major);
		return ret;
	}


	return 0;
}

static void __exit scull_exit(void)
{
	dev_t x_t;
	int scull_minor = MINOR_T;
	int scull_major = MAJOR_T;
	int count = COUNT_T;

	x_t = MKDEV(scull_major, scull_minor);



	unregister_chrdev_region(x_t, count);
}


module_init(scull_init);
module_exit(scull_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("My first char device");
MODULE_VERSION("2.0.0");

