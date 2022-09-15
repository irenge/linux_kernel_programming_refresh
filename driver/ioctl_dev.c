
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define IOCTL_DEV "ioctldrv"


static inline long
chr_unlocked_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
        pr_info(" I entered the ioctl with cmd=%d, do what you want!\n", cmd);
        return 0;
}

static const struct file_operations chrdrv_fops = {
        .owner = THIS_MODULE,
        .unlocked_ioctl = chr_unlocked_ioctl,
};

static struct miscdevice my_ioctl_device = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = IOCTL_DEV,
        .fops = &chrdrv_fops,
};

/*
** Module Init function
*/
static int __init chr_driver_init(void)
{
	if (misc_register(&my_ioctl_device)) {
		pr_err("Could not register my device");
		return -EBUSY;
	}

	pr_info("Device registerd :)");
	return 0;
}
/*
** Module exit function
*/
static void __exit chr_driver_exit(void)
{
        misc_deregister(&my_ioctl_device);
        pr_info("Device Driver Remove...Done!!!\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("IOCTL");
MODULE_VERSION("0.1");
