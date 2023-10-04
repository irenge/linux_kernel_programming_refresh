/*
 * simple low resolution timer 
 *
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/slab.h>

static struct kt_data {
	struct timer_list timer;
	unsigned long value;
} *data;


static void funrun(struct timer_list *tl) {

	struct kt_data *ndata = from_timer(ndata, tl, timer);
	pr_info("\nI am in funrun function: jiffies = %ld\n", jiffies);
	pr_info("\nCurrent pid = %d\n", (int) current->pid);
	pr_info("\n My data value is %ld\n",ndata->value);

}

static int __init my_init(void)
{
	struct timer_list *tl;
	static int len = 42;

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	
	data->value = len;
	tl = &data->timer;
	timer_setup(tl, funrun, 0);

	mod_timer(tl, jiffies + HZ);


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

