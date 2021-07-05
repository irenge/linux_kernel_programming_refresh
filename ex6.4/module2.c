#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
extern int x;
extern int y;
extern int calc(int a , int b);

static int __init hello_init(void)
{
	pr_info("Add %d %d = %d\n", x,y, calc(x,y));
	return 0;
}
static void __exit hello_exit(void) 
{
	pr_alert("Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);



