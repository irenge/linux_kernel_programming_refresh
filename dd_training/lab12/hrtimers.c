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
	char *name;
} *dats;

static  enum hrtimer_restart ktfun(struct hrtimer *var)
{
	struct  kt_data *data = container_of(var, struct kt_data, timer);
	ktime_t now = var->base->get_time();

	pr_info(" Kernel hrtimer %s: period = %lld elapsed = %ld\n", data->name, data->period, jiffies - data->start_time);
	pr_info("\nCurrent pid = %d\n", (int) current->pid);


	hrtimer_forward(var, now, data->period);
	return HRTIMER_RESTART;
}
static int __init my_init(void)
{
	int i;
	struct kt_data *d;

	pr_info("\nCurrent pid = %d\n", (int) current->pid);

	dats = kmalloc_array(2, sizeof(*dats), GFP_KERNEL);
	for(d = dats, i = 0; i < 2; i++, d++) {
		d->period_in_secs = (i == 0) ? 1 : 5;

		d->period = ktime_set(d->period_in_secs, 0);

		d->name =  (i ==0) ? "A" : "B";


		d->start_time =  jiffies; // Not sure about this


		hrtimer_init(&d->timer, CLOCK_REALTIME, HRTIMER_MODE_REL);

		d->timer.function = ktfun;

		hrtimer_start(&d->timer, d->period, HRTIMER_MODE_REL);


		pr_info("\nkernel timers %d: Launching timer with period of %lld seconds\n", i+1, d->period);
	}

	return 0;
}
static void __exit my_exit(void)
{
	int i;
	struct kt_data *d = dats;
	for(i=0; i < 2; i++, d++) { 
		hrtimer_cancel(&d->timer);
		pr_info("Freeing %d timer\n",i+1);
		//kfree(d);
	}
	kfree(dats);
	pr_info("\nBye bye fantastic world\n");
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
