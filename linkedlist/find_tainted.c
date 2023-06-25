/* Finding Tainted Modules
 *
 * All modules loaded on the system are linked in a list that can be
 * accessed from any module:
 *
 *    struct module {
 *     ....
 *    struct list_head list;
 *    ....
 *    char name[MODULE_NAME_LEN];
 *    ....
 *    unsigned int taints;
 *    ....
 *    }
 *
 * Write a module that walks through this linked list and prints out
 * the value of taints and any other values of interest.  (The module
 * structure is defined in /usr/src/linux/include/linux/module.h.)
 *
 * You can begin from THIS_MODULE.
 */



#include <linux/module.h>
#include <linux/init.h>

static int __init list_init(void)
{
	int i = 0;
	struct list_head *modules;
	struct module *m = THIS_MODULE;

	modules = &m->list;
	modules = modules->prev;

	pr_info("\n");

	// iterate over the list 
	
	list_for_each_entry(m, modules, list) {
		pr_info("%3d MOD: %20s, taints = %08lx", i++, m->name, m->taints);
	}

	return 0;
}

static void __exit list_exit(void)
{
	pr_info("Bye: module unloaded from 0x%p\n", list_exit);
}

module_init(list_init);
module_exit(list_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");


