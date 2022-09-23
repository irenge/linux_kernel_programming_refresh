#include <linux/module.h>
#include <linux/init.h>

#define BUFSIZE 4

static int __init my_init(void)
{
	char buf[BUFSIZE];

	int x,y;

	x = snprintf(buf, BUFSIZE, "Linux"); // length is 5 here : return length of expected to be written

	pr_info(" With length %d, The string is %s\n", x, buf);

	y = scnprintf(buf, BUFSIZE, "Linux"); //length is 3 : return length of what is actually written to buff

	pr_info(" With length %d, The string is %s\n", y, buf);

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
