#include <linux/init.h>
#include <linux/module.h>
#include <linux/utsname.h>

MODULE_LICENSE("GPL v2");

//static struct new_utsname* name ;

static int __init uname_init(void)
{
	struct new_utsname *name = init_utsname();

	pr_info("%s %s %s %s %s %s\n", name->sysname, name->nodename,name->release,name->version, name->machine, name->domainname);
	return 0;
}
static void __exit uname_exit(void) 
{
	pr_alert("Goodbye, cruel world\n");
}
module_init(uname_init);
module_exit(uname_exit);



