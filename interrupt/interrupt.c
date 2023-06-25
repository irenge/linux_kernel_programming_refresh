#include <linux/module.h>
#include <linux/init.h>
#include <linux/hardirq.h>

void check(int x){

	 if (x){
                 pr_info("We are in interrupt context");
               
         } else {
                 pr_info("We are in process context");
         }


}


static int __init my_init(void)
{
	int y;
	unsigned long flags;

	pr_info("Hello: module loaded at 0x%p\n", my_init);
	pr_info("PAGESIZE = %d\n", PAGE_SIZE);
	
	local_irq_disable();
	y = in_interrupt();
	check(y);
	local_irq_enable();
	y = in_interrupt();
        check(y);

		
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
