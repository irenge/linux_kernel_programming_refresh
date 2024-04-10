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
} *data;


static void funrun(struct timer_list *tl) {

	struct kt_data *ndata = from_timer(ndata, tl, timer);
	pr_info("\nKernel timer: period = %ld elapsed = %ld\n", ndata->period, jiffies - ndata->start_time);
	pr_info("\nCurrent pid = %d\n", (int) current->pid);
        mod_timer(tl, jiffies + ndata->period); /* re-activate the timer */

}

static int __init my_init(void)
{
	struct timer_list *tl;

	data = kmalloc(sizeof(*data), GFP_KERNEL);

	data->period = 2*HZ;
	data->start_time = jiffies;

	tl = &data->timer;
	pr_info("\nCurrent pid = %d\n", (int) current->pid);

	timer_setup(tl, funrun, 0); /* Initialise the timer list */
	mod_timer(tl, jiffies + data->period);

	pr_info("\nLaunching timer with period of %ld seconds\n",
                data->period);


	return 0;
}

static void __exit my_exit(void)
{
	del_timer_sync(&data->timer);
	kfree(data);
	pr_info("\nBye bye world!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");

