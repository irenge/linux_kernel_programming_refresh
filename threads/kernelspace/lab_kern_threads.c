#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>

#include <linux/percpu.h>

static DEFINE_PER_CPU(long, cpuvar) = 2;


static struct task_struct *task[NR_CPUS];

static void disp_cpu( char *s)
{
	preempt_disable();
	pr_info("%s cpu=%d ", s, smp_processor_id());
	preempt_enable();
	pr_info(" with cpu var is %ld  \n", get_cpu_var(cpuvar)++);
        put_cpu_var(cpuvar);

}

static int kthr_fun(void *arg) {
	do {

		disp_cpu("About to start the thread ...");
		
		//x = x + 3;

		msleep_interruptible(3000);
		disp_cpu("Sleeping over ");
		//pr_info ("x = %d\n", x);

	}while (!kthread_should_stop());
	return 0;
}



static int __init start_fun(void) 
{
	int i;


	disp_cpu("Running on ");

	//pr_info("cpu var is %ld  \n", get_cpu_var(cpuvar)++);
        //put_cpu_var(cpuvar);

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
	int i;
	//pr_info("Bye horrible world !");
	for_each_online_cpu(i) {
		kthread_stop(task[i]);
		disp_cpu("Kill thread  was done");
	}
	pr_info("Bye horrible world !");

}

module_init(start_fun);
module_exit(end_fun);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");

