#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init  module_taint(void) {

struct list_head  *listx;
struct module *modx = THIS_MODULE;
int i = 0;

listx = &modx->list;
listx = listx->prev;

pr_info("\n");

list_for_each_entry(modx, listx, list) {

	pr_info("%2d MOD:%20s, taints = %08lx\n", i++, modx->name, modx->taints);

}

return 0;

}

static void __exit exit_b(void) {

	pr_info("Bye: unloading the module 0x%p\n", exit_b);

}
module_init(module_taint);
module_exit(exit_b);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");

