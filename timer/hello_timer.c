
/*  
 *  Timer - Jiffies
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include<linux/jiffies.h>
#include <linux/delay.h>

int __init timer_start(void)
{
	unsigned long start;
	unsigned long total_time;
	int i;

	start = jiffies;
	pr_info("counting...");
	for (;;) {
		if (start < jiffies - 10000)
			break;
	}
	total_time = jiffies - start;
	pr_info("displaying that, took %lu ticks\n", jiffies_to_clock_t(total_time));
	pr_info("User space will understand it as %lu seconds\n", total_time/HZ);

	mdelay(10000);
	total_time = jiffies - start;
        pr_info("Using udelay: displaying that, took %lu ticks\n", jiffies_to_clock_t(total_time));
        pr_info("Using udelay: User space will understand it as %lu seconds\n", total_time/HZ);


	return 0;
}

void __exit timer_end(void)
{
	pr_info("Goodbye world 1.\n");
}

MODULE_LICENSE("GPLv2");

MODULE_AUTHOR("Jules");
module_init(timer_start);
module_exit(timer_end);
