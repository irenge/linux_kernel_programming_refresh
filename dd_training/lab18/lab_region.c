#define IOSTART 0x200
#define EXTEND 0x11

#include<linux/module.h>
#include<linux/ioport.h>
#include <linux/io.h>
#include <linux/init.h>

static unsigned long iostart = IOSTART, extend= EXTEND, ioend;

static int __init hard_init(void) {

	unsigned long ultest = 100;

	ioend = iostart + extend;

	for(;;) {
	pr_info("\nRequesting IO region from 0x%lx to 0x%lx\n", iostart, ioend);
	if(request_region(iostart, ioend, "my_ioport")) {
		break;
	}
	pr_info("\nThe requested region is busy...Not returning -EBUSY but incrementing...\n");
	iostart += 1;
	ioend += 1;
	}

	pr_info("writing a long %ld\n", ultest);
	outl(ultest, iostart);
	ultest = inl(iostart);
	pr_info(" Reading a long=%ld\n", ultest);


	return 0;
}

static void __exit hard_exit(void) {

	pr_info(" releasing  the IO region from 0x%lx to 0x%lx\n", iostart, ioend);
	release_region(iostart, ioend);

}

module_init(hard_init);
module_exit(hard_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");

