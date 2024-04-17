#include<linux/module.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include <linux/version.h>

static void t_fun(unsigned long t_arg);

static struct simp {
	int i;
	int j;
} t_data;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,9,0)
static DECLARE_TASKLET(t_name, t_fun, (unsigned long) &t_data);
#else
static DECLARE_TASKLET_OLD(t_name, t_fun);
#endif

static int __init my_init(void)
{
	t_data.i = 100;
	t_data.j = 200;
	pr_info("Scheduling my tasklet, jiffies=%ld\n", jiffies);
	tasklet_schedule(&t_name);
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("\nUnloading module\n");
}
static void t_fun(unsigned long t_arg)
{
	struct simp *datum = &t_data;
	pr_info("\nEntering t_fun, datum->i = %d, jiffies = %ld\n", datum->i, jiffies);
	pr_info("\nEntering t_fun, datum->j = %d, jiffies = %ld\n", datum->j, jiffies);
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("JC");
MODULE_LICENSE("GPL v2");
