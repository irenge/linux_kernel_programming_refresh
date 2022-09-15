#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/module.h>

static int __init my_init(void)
{
	struct task_struct *p, *t;

	for_each_process_thread(p, t)
		 pr_info("command= %s - Thread id = %d - pid = %d, %d\n",t->comm,t->tgid, t->pid, t->__state );
        
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

