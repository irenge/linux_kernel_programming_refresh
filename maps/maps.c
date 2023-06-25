#include <linux/idr.h>
#include <linux/module.h>
#include <linux/init.h>

struct idr id_huh;

//idr_init(&id_huh);

static int  __init my_init(void) {

//	int ret;
	int id;
	void *ptr;


	idr_init(&id_huh);


	//do {

	//if (idr_pre_get(&id_huh, GFP_KERNEL)== 0) 
	//	return -ENOSPC;
	//ret = idr_get_next(&id_huh, ptr, &id);
	//} while (ret == -EAGAIN); 


	return 0;
}

static void __exit my_exit(void)
{
	pr_info("Bye: queue module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

