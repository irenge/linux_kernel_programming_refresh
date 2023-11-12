/*
 * Simple low resolution timer with periodic reinstall
 *
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/slab.h>

static struct kt_data {
	struct timer_list timer;
	unsigned long start_time;
	unsigned long period;
	char *name;
} *data_x, *data_y;

static void funrun(struct timer_list *tl) {

	struct kt_data *ndata = from_timer(ndata, tl, timer);
	pr_info("\nKernel timer %s: period = %ld elapsed = %ld\n",ndata->name, ndata->period, jiffies - ndata->start_time);
	pr_info("\nCurrent pid = %d\n", (int) current->pid);
        mod_timer(tl, jiffies + ndata->period); /* re activate the timer */

}

static int __init my_init(void)
{
	struct timer_list *tl_x, *tl_y;

	data_x = kmalloc(sizeof(*data_x), GFP_KERNEL);
	data_y = kmalloc(sizeof(*data_y), GFP_KERNEL);

	data_x->name = "X";
	data_y->name = "Y";

	data_x->period = 1*HZ;
	data_y->period = 10*HZ;

	data_x->start_time = jiffies;
        data_y->start_time = jiffies;

	tl_x = &data_x->timer;
	tl_y = &data_y->timer;

	pr_info("\nCurrent pid = %d\n", (int) current->pid);

	timer_setup(tl_x, funrun, 0); /* Initialise the timer list 2 */
	timer_setup(tl_y, funrun, 0); /* Initialise the timer list 2 */
	
	mod_timer(tl_x, jiffies + data_x->period);
	mod_timer(tl_y, jiffies + data_y->period);


	pr_info("\nLaunching first timer with period of %ld seconds\n", data_x->period);
	pr_info("\nLaunching second timer with period of %ld seconds\n", data_y->period);


	return 0;
}

static void __exit my_exit(void)
{
	del_timer_sync(&data_x->timer);
	del_timer_sync(&data_y->timer);
	kfree(data_x);
	kfree(data_y);
	pr_info("\nBye bye world!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");

