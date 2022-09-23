#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>

static struct task_struct *task[NR_CPUS];

static void disp_cpu( char *s)
{
	preempt_disable();
	pr_info("%s cpu=%d\n", s, smp_processor_id());
	preempt_enable();

}

static int kthr_fun(void *arg) {
	do {
		disp_cpu("About to start the thread ...");
		msleep_interruptible(3000);
		disp_cpu("Sleeping over ");
	}while (!kthread_should_stop());
	return 0;
}



static int __init start_fun(void) 
{
	int i;
	

	disp_cpu("Running on ");

	for_each_online_cpu(i) {
		task[i] = kthread_create(kthr_fun, NULL, "Thread %d", i);

		if(!task[i]) {
			pr_info("Failed to generate a kernel thread\n");
		return -1;
		}
		kthread_bind(task[i], i);
		pr_info("waking up and run thread for cpu %d\n", i);
		wake_up_process(task[i]);
		disp_cpu("Thread on ");


	}
	return 0;
}
static void __exit end_fun(void)
{
	pr_info("Bye horrible world !");
}

module_init(start_fun);
module_exit(end_fun);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");

