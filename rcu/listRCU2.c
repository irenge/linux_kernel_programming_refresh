#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

static LIST_HEAD(simple_list);

struct  f_entry {
	struct list_head odlist;
	int a, b, c;
	char name[20];
};

static int __init systems_init(void) {
	pr_info("NO RCU");

	struct f_entry *system_entry;
	int j;

	for (j = 0; j < 20; j++) {
		system_entry = kmalloc(sizeof(struct f_entry), GFP_KERNEL);
		if(!system_entry) {
			pr_info("Failed to allocate memory for system list\n");
			return -ENOMEM;
		}
		system_entry->a = j;
		system_entry->b = j + 1 ;
		system_entry->c = j + 2;
		sprintf(system_entry->name, "entry %d", j + 1);
		pr_info("... adding %s with a = %d, b = %d and c = %d\n", system_entry->name, system_entry->a, system_entry->b, system_entry->c);
		list_add(&system_entry->odlist, &simple_list);
	}
	return 0;
}
static void __exit systems_exit(void) {
	pr_info("NO RCU");
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&simple_list)) {
		pr_info("Odd numbers list is empty, exiting\n");
		return;
	}
	pr_info("Traverse and emptying\n");

	list_for_each_safe(list, tmp, &simple_list) {

		struct f_entry *pili = list_entry(list, struct f_entry, odlist);
		list_del(&pili->odlist);

		pr_info("Exit : %s with a = %d, b = %d and c = %d removed from list\n",pili->name, pili->a, pili->b, pili->c);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&simple_list))
		pr_info("Done: simple list is empty!\n");
	else
		pr_warn("simple list  still NOT empty though!\n");


}
module_init(systems_init);
module_exit(systems_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");



