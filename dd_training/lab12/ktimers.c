#include <linux/sched.h>
#include <linux/module.h>
#include <linux/delay.h>

static int __init hello_init_timers(void) {
	
	int delay,y, jifdone;
	unsigned int mls = 7000;
	delay = 2;
	jifdone =  jiffies + delay * HZ;

	while(time_before(jiffies, jifdone))
	{
		pr_info("\nBefore...\n");
	}

	pr_info("\nAfter...\n");
	mdelay(7000);
	
	pr_info("\nAfter 7 seconds using mdelay() function\n");

	msleep(mls);

	//pr_info("\n remain %ld\n", msleep_interruptible(mls));

	pr_info("\nAfter 7 seconds using msleep() function\n");


	return 0;
}

static void __exit hello_exit_timers(void) {

	pr_info("\nBye bye ugly world\n");

}

module_init(hello_init_timers);
module_exit(hello_exit_timers);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
