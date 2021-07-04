#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int __init process_init(void)
{

	struct task_struct *process, *thr;
	pr_info("\nThreadId\tpid\tstate\ttCommand\tpCommand\tptrace\tpriority\texit code\n");
	for_each_process_thread(process,thr) {
		printk("%d\t%d\t0x%08lx\t%s\t%s\t%d\t%d\t%d\n",thr->tgid, thr->pid, thr->state, thr->comm,process->comm, process->ptrace, process->prio, process->exit_code);
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



