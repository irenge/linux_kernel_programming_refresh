#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

static struct dentry *parent, *filen;

static u32 val = (u32) 888;
static u32 val32 = (u32) 777;
#define MAXSTRING 32 

static ssize_t my_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
        int nbytes; 
        ssize_t rc;
        char *kstring;

        kstring = kmalloc(MAXSTRING, GFP_KERNEL);  
        if (IS_ERR(kstring)) 
                return PTR_ERR(kstring);

        nbytes = sprintf(kstring, "%d\n", val); 
        rc = simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
        kfree(kstring);
    
        pr_info("d_inode = %p\n", parent->d_inode);
        return rc;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
        int rc;
        u32 tmp;

        rc = kstrtoint_from_user(buf, lbuf, 10, &tmp);
        if (rc)
                return rc;
        val = tmp;
        pr_info("\n WRITING function, nbytes=%ld, val=%d\n", lbuf, val);
        return lbuf;
}


static const struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = my_read,
        .write = my_write,
};

static int __init my_init(void)
{
        parent = debugfs_create_dir("mydir", NULL);
        debugfs_create_u32("myname", 0644, parent, &val32);
        filen = debugfs_create_file("filen", 0644, parent, NULL, &fops);
        pr_info("Hello: module loaded at 0x%p\n", my_init);
        return 0;
}

static void __exit my_exit(void)
{
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
        debugfs_remove(filen);
        debugfs_remove(parent); /* A remove is recursive by default */
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD420:5.17 s_16/lab_debugfs.c");
MODULE_LICENSE("GPL v2");
