#include <linux/sched.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timer.h>

#define TIMEOUT 5000    //milliseconds

static struct timer_list my_timer;
static unsigned int count = 0;

//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
	/* do your timer stuff here */
	pr_info("Timer Callback function Called [%d]\n",count++);

	/*
	   Re-enable timer. Because this function will be called only first time. 
	   If we re-enable this will work like periodic timer. 
	   */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int __init hello_init_ktimers(void) {
	/*static struct timer_list my_timer;

	  init_timer(&my_timer);
	  my_timer->function = my_function;
	  my_timer->data = (unsigned long) &my_data;
	  my_timer->expires = jiffies + secs*HZ;
	  */

	/* setup your timer to call my_timer_callback */
	timer_setup(&my_timer, timer_callback, 0);

	//If you face some issues and using older kernel version, then you can try setup_timer API(Change Callback function's argument to unsingned long instead of struct timer_list *.
	/* setup timer interval to based on TIMEOUT Macro */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));

	return 0;
}

static void __exit hello_exit_ktimers(void) {

	pr_info("\nBye bye ugly world\n");

}

module_init(hello_init_ktimers);
module_exit(hello_exit_ktimers);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
