#include <linux/module.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>

static struct kt_data {
	struct hrtimer timer;
	ktime_t period;
} *data;

static  enum hrtimer_restart ktfun(struct hrtimer *var)
{
	struct  kt_data *data = container_of(var, struct kt_data, timer);
	ktime_t now = var->base->get_time();
	pr_info("\nin kfun: process PID %d\n", current->pid);

	pr_info("timer running at jiffies=%ld\n", jiffies);
	hrtimer_forward(var, now, data->period); /* called to reset a new expiration time */
	return HRTIMER_RESTART; /* return for a recurring timer if not would have been HRTIMER_NORESTART */
}
static int __init my_init(void)
{
	data = kmalloc(sizeof(*data), GFP_KERNEL);
	
	if(!data)
		return -ENOMEM;

	data->period = ktime_set(1, 0);
	hrtimer_init(&data->timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	data->timer.function = ktfun;
	pr_info("\nCurrent process PID %d\n", current->pid);
	hrtimer_start(&data->timer, data->period, HRTIMER_MODE_REL);

	return 0;
}
static void __exit my_exit(void)
{
	if (!hrtimer_cancel(&data->timer)) {
		pr_info("\nTimer expired!\n");
	} else 
		pr_info("\nTimer successfully cancelled\n");

	kfree(data);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");

