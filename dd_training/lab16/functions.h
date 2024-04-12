#ifndef _FUN_CHAR_H
#define _FUN_CHAR_H

#include <linux/module.h>       /* for modules */
#include <linux/fs.h>           /* file_operations */
#include <linux/uaccess.h>      /* copy_(to,from)_user */
#include <linux/init.h>         /* module_init, module_exit */
#include <linux/slab.h>         /* kmalloc */
#include <linux/cdev.h>         /* cdev utilities */

#define MYDEV_NAME "mycdrv"
#define ramdisk_size ((size_t) (16*PAGE_SIZE))

static inline size_t generic_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos);
static inline int generic_open(struct inode *inode, struct file *file);
static inline ssize_t generic_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos);
static inline int generic_open(struct inode *inode, struct file *file);
static inline int generic_release(struct inode *inode, struct file *file);
static inline int __init generic_init(void);
static void __exit generic_exit(void);



static dev_t first;
int major_, minor_;

static unsigned int count = 1;
//static int my_major = 500, my_minor;
static struct cdev *my_cdev;

static const struct file_operations mycdrv_fops;


static inline size_t generic_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
	int nbytes;
	//maxbytes, bytes_to_do;

//	maxbytes = ramdisk_size - *ppos;
//	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
//	if (bytes_to_do == 0)
//	pr_warn("\nReached end of the device on a read");

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	if (!kbuf)
	return -ENOMEM;

	file->private_data = kbuf;
	nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
	*ppos += nbytes;
	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;

}

static inline ssize_t generic_write(struct file *file, const char __user *buf, size_t lbuf,
		loff_t *ppos) {

	int nbytes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	    if (!kbuf)
        return -ENOMEM;

	file->private_data = kbuf;

	nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
	*ppos += nbytes;
	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);

	return nbytes;
}

static inline int generic_open(struct inode *inode, struct file *file) {
	static int ntimes;

	char  *kbuf = kmalloc(ramdisk_size, GFP_KERNEL);
	    if (!kbuf)
        return -ENOMEM;

	file->private_data = kbuf;

	(ntimes == 0) ? pr_info("\nDevice %s opened 1st time ", MYDEV_NAME) : pr_info("\nDevice %s opened %d times ", MYDEV_NAME, ntimes + 1);
	ntimes += 1;

	pr_info(" with Major number %d and Minor number %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));

	pr_info("ref=%d\n", (int)module_refcount(THIS_MODULE));
	return 0;

}
static inline int generic_release(struct inode *inode, struct file *file) {
	char  *cbuf = file->private_data;
	if (cbuf == NULL) 
	pr_info("\nNULL: testing\n");
	pr_info(" CLOSING device: %s\n\n", MYDEV_NAME);
	kfree(cbuf);

	return 0;
}

static inline int __init generic_init(void) {
	int ret;
	ret = alloc_chrdev_region ( &first, 0, count, MYDEV_NAME);
	pr_info("Using dynamic alloc...\n");
	major_ = MAJOR(first);
	minor_ = MINOR(first);
	pr_info("%s device allocated, dev [%i, %i]\n", MYDEV_NAME, major_, minor_);

	my_cdev = cdev_alloc();
	    if (!my_cdev) {
                pr_err("cdev_alloc() failed\n");
                unregister_chrdev_region(first, count);
                return -1;
        }
	cdev_init(my_cdev, &mycdrv_fops);
	cdev_add(my_cdev, first, count);
	pr_info("\nReturn = %d ==> Succeeded in registering character device %s\n",ret, MYDEV_NAME);

	return 0;
}

static void __exit generic_exit(void)
{
        cdev_del(my_cdev);
        unregister_chrdev_region(first, count);
        pr_info("\ndevice unregistered\n");
}


#endif

