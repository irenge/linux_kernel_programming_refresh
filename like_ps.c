#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int __init process_init(void)
{

	struct task_struct *task;
	pr_info("\npid\tstate\tcommand\tptrace\tpriority\texit code\n");
	for_each_process(task) {
		printk("%d\t%ld\t%s\t%d\t%d\t%d\n", task->pid, task->state, task->comm, task->ptrace, task->prio, task->exit_code);
	}
	pr_info("Hello PS process\n");
	return 0;
}
static void __exit process_exit(void) 
{
	pr_alert("Goodbye, PS world\n");
}
module_init(process_init);
module_exit(process_exit);



