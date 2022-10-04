#include <linux/module.h>
#include <linux/init.h>
#include <linux/sysfs.h>

#define BUFSIZE 4

static int __init my_init(void)
{
//	int BUFSIZE = 4;

	char __user buf[BUFSIZE];

	//const char* str = "Linux";

	int x;

	x = sysfs_emit(buf, "Hello"); // length is 5 here : return length of expected to be written

	pr_info(" With length %d, The string is %s\n", x, buf);
	/*

	y = sysfs_emit_at(buf, BUFSIZE, "Linux"); //length is 3 : return length of what is actually written to buff

	pr_info(" With length %d, The string is %s\n", y, buf);
*/
	return 0;
}
static void __exit my_exit(void)
{
	pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");
