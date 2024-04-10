#include<linux/module.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/oom.h>

#define MYDEV_NAME "mycdrv"


static struct device *my_dev;
static char *ramdisk;
static size_t ramdisk_size = (16 * PAGE_SIZE);

static const struct file_operations mycdrv_fops;

static struct device *my_dev;


static unsigned pg_offset(const void __user *addr) {

	unsigned long mask = PAGE_SIZE - 1;
	return (unsigned int) (unsigned long)addr & mask;

}


static unsigned long pfn_of(const void __user *addr)
{
	return (unsigned long) addr >> PAGE_SHIFT;
}

static int mycdrv_open(struct inode *inode, struct file *file)
{
        static int ntimes;

        char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);

        file->private_data = kbuf;

        (ntimes == 0) ? pr_info("\nDevice %s opened 1st time ", MYDEV_NAME) : pr_info("\nDevice %s opened %d times ", MYDEV_NAME, ntimes + 1);
        ntimes += 1;

        pr_info(" with Major number %d and Minor number %d\n", imajor(inode), iminor(inode));

        pr_info("ref=%d\n", (int)module_refcount(THIS_MODULE));

        return 0;
}

static inline int mycdrv_generic_release(struct inode *inode, struct file *file)
{
        dev_info(my_dev, " closing device: %s:\n\n", MYDEV_NAME);

        return 0;
}


static ssize_t mycdrv_rw(struct file *filp, char __user *buf, size_t lbuf, loff_t *ppos, int rw)
{
	int j, rc, npages;
	struct page **pages;

	char * ktmp, *uaddr;
	size_t nb, cur_len;


	/* get total number of pages of memory needed */
	npages = (pfn_of(buf + lbuf) - pfn_of(buf)) + 1;

	/* allocate space for an array of pages structure */

	dev_info(my_dev, "npages = %d  for %ld bytes\n", npages, (long) lbuf);

	pages = kmalloc_array(npages, sizeof(pages), GFP_KERNEL);

	if(!pages) {
		dev_err(my_dev, "allocating pages failed!\n");
		return -ENOMEM;
	}

	/* get the page structures */

	rc = pin_user_pages_fast((unsigned long)buf, npages, FOLL_WRITE, pages);

	dev_info(my_dev, " I pinned %d user pages\n", rc);

	if (rc < npages)
		goto out;

	ktmp = ramdisk;
	cur_len = lbuf;

	for (j = 0; j < npages; j++) {
		/* remap the page address could also use page_address(page) */ 
		uaddr = kmap(pages[j]);
		nb = PAGE_SIZE;

		if (j == 0) {
			nb -= pg_offset(buf);
			uaddr += pg_offset(buf);
		}
		nb = min(nb, cur_len);

		/* do actual IO operations */
		switch(rw) {
			case 0:
				memcpy(uaddr, ktmp, nb); /* read */
				dev_info(my_dev," kernel sees on a read:\n%s\n", uaddr);
				break;
			case 1:
				memcpy(ktmp, uaddr, nb); /* write */
				dev_info(my_dev," kernel sees on a write:uaddr:\n%s\n", uaddr);
				break;
			default:
				dev_err(my_dev, "Error in rw routine, rw=%d\n", rw);
				break;
		}
		cur_len -= nb;
		ktmp += nb;

		/* release the remap */
		kunmap(pages[j]);
	}

	/* 
	 * releases the pages 
	 * If we read from our "device", we wrote into the user pages 
	 * 
	 */
	unpin_user_pages_dirty_lock(pages, npages, (rw == 0));
	kfree(pages);
	dev_info(my_dev, "Leaving the READ  function, nbytes=%ld\n",
			(long)lbuf);
	return lbuf;
out:
	if (rc > 0)
		unpin_user_pages(pages, rc);
	kfree(pages);
	return 0;
}

static ssize_t
mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
        int rw = 0;

        return mycdrv_rw(file, buf, lbuf, ppos, rw);
}

static ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
             loff_t *ppos)
{
        int rw = 1;

        return mycdrv_rw(file, (char __user *)buf, lbuf, ppos, rw);
}

static const struct file_operations mycdrv_fops = {
        .owner = THIS_MODULE,
        .read = mycdrv_read,
        .write = mycdrv_write,
        .open = mycdrv_open,
        .release = mycdrv_generic_release,
};

static struct miscdevice my_misc_device = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = MYDEV_NAME,
        .fops = &mycdrv_fops,
};

static int __init my_generic_init(void)
{
        ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
        if (misc_register(&my_misc_device)) {
                pr_err("Couldn't register device misc, "
                       "%d.\n", my_misc_device.minor);
                kfree(ramdisk);
                return -EBUSY;
        }
        my_dev = my_misc_device.this_device;
        dev_info(my_dev, "Succeeded in registering device %s\n", MYDEV_NAME);
        return 0;
}

static void __exit my_generic_exit(void)
{
        dev_info(my_dev, "Unregistering Device\n");
        misc_deregister(&my_misc_device);
        kfree(ramdisk);
}


module_init(my_generic_init);
module_exit(my_generic_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("lab_usermap.c");
MODULE_LICENSE("GPL v2");
