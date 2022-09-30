/* 
 * pass wall message
 */

#include<linux/module.h>
#include<linux/init.h>
#include<linux/kmod.h>

static char *str;
module_param(str, charp, 0444);

static char* argv[] = {"wall", "Hello there this is Jules from the kernel space", NULL};

static int __init my_init(void)
{
	int j = 0;
	int xt;
	static char *envp[] = {NULL};

	pr_info("Start");
	if (str) 
		argv[1] = str;


	while (argv[j])
		pr_info(" %s ", argv[j++]);

        pr_info("Hello: Usermod module loaded at 0x%p\n", my_init);

	xt = call_usermodehelper("/usr/bin/wall" , argv,  envp, UMH_NO_WAIT);
	
	pr_info("return value of xt = %d", xt);

	if(xt) {
		pr_info("Failed to execute %s %s", argv[0],argv[1]);
		return xt;
	}
	pr_info ("message passed");

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

