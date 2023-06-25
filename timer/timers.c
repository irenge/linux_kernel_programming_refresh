
/*  
*/
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include<linux/jiffies.h>
#include <linux/timer.h>

#define TIMEOUT 5000

static struct timer_list my_timer;
static unsigned int count = 0;
void timer_callback(struct timer_list *base);

int __init timer_start(void)
{
	struct timer_list my_timer;

	//init_timers(&my_timer);


///	my_timer.expires =  jiffies + delay;

//	my_timer.data = 0;

	/* setup your timer to call my_timer_callback */
	timer_setup(&my_timer, timer_callback, 0);       //If you face some issues and using older kernel version, then you can try setup_timer API(Change Callback function's argument to unsingned long instead of struct timer_list *.

	/* setup timer interval to based on TIMEOUT Macro */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));


	return 0;
}

void timer_callback(struct timer_list *base) {

	/* do your timer stuff here */
	pr_info("Timer Callback function Called [%d]\n",count++);

	/*
	   Re-enable timer. Because this function will be called only first time. 
	   If we re-enable this will work like periodic timer. 
	   */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));


}

void __exit timer_end(void)
{
	pr_info("Goodbye world 1.\n");
}

MODULE_LICENSE("GPLv2");

MODULE_AUTHOR("Jules");
module_init(timer_start);
module_exit(timer_end);
