#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
static int calc(int a, int b) {
	return a + b;
}
static int __init hello_init(void)
{
	pr_info("Sum of %d and %d = %d\n", 3, 4, calc(3,4));
	return 0;
}
static void __exit hello_exit(void) 
{
	pr_alert("Goodbye, cruel \n");
}
int x = 2;
int y = 3;
EXPORT_SYMBOL(x);
EXPORT_SYMBOL(y);

EXPORT_SYMBOL(calc);
module_init(hello_init);
module_exit(hello_exit);



