#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/module.h>

static int __init my_init(void)
{
	struct task_struct *p;

	for_each_process(p)
		 pr_info("Task %s (pid = %d), 0x%8x\n",p->comm, task_pid_nr(p), p->__state );
        
	pr_info("\nmodule loaded at 0x%p\n", my_init);
        return 0;
}
static void __exit my_exit(void)
{
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

