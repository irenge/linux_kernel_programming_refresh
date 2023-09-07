#include<linux/module.h>
#include<linux/mm.h>

static int  mycdrv_mmap(struct file *filp, struct  vm_area_struct *vma) {
	pr_info("I entered the mmap function\n");
	if(remap_pfn_range(vma, vma->vmstart, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}


static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.mmap = mycdrv_mmap,
};

module_init();
module_exit();

MODULE_AUTHOR("Jules");
MODULE_DESCRIPTION("S");
MODULE_LICENSE("GPL v2
