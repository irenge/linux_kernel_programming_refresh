#include <linux/init.h>
#include <linux/module.h>


/* Finding Tainted Modules
 *
 * All modules loaded on the system are linked in a list that can be
 * accessed from any module:
 *
 *    struct module {
 *     ....
 *    struct list_head list;
 *    ....
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
MODULE_LICENSE("GPL v2");

static int __init taints_init(void)
{
	struct list_head *modules;
	struct module *ml = THIS_MODULE;

	modules = &ml->list;
	modules = modules->prev;
	int i = 0;
	list_for_each_entry(ml, modules, list) {
		pr_info("%2d MODULES %10s, taints = %03lX\n",i++, ml->name, ml->taints);
	}
	return 0;
}

static void __exit taints_exit(void)
{
	pr_alert("Goodbye, cruel world\n");
}
module_init(taints_init);
module_exit(taints_exit);



