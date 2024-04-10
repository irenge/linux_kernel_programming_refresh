#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>

static struct kt_data {
	struct hrtimer timer;
	int period_in_secs;
	unsigned long start_time;
	ktime_t period;
} *data_x, *data_y;

static  enum hrtimer_restart ktfun(struct hrtimer *var)
{
	struct  kt_data *data = container_of(var, struct kt_data, timer);
	ktime_t now = var->base->get_time();

	pr_info(" Kernel hrtimer: period = %d elapsed = %ld\n", data->period, jiffies - data->start_time);
	pr_info("\nCurrent pid = %d\n", (int) current->pid);


	hrtimer_forward(var, now, data->period);
	return HRTIMER_RESTART;
}
static int __init my_init(void)
{
	pr_info("\nCurrent pid = %d\n", (int) current->pid);

	data_x = kmalloc(sizeof(*data_x), GFP_KERNEL);
	data_y = kmalloc(sizeof(*data_y), GFP_KERNEL);

	data_x->period = ktime_set(1, 0);
	data_y->period = ktime_set(5,0);

	data_x->start_time = data_y->start_time = jiffies; // Not sure about this
        

	hrtimer_init(&data_x->timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	hrtimer_init(&data_y->timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	
	data_x->timer.function = ktfun;
	data_y->timer.function = ktfun;

	hrtimer_start(&data_x->timer, data_x->period, HRTIMER_MODE_REL);
	hrtimer_start(&data_y->timer, data_y->period, HRTIMER_MODE_REL);

	pr_info("\nLaunching timer with period of %ld seconds\n", data_x->period);
	pr_info("\nLaunching timer with period of %ld seconds\n", data_y->period);

	return 0;
}
static void __exit my_exit(void)
{
	hrtimer_cancel(&data_x->timer);
	hrtimer_cancel(&data_y->timer);

	kfree(data_x);
	kfree(data_y);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
