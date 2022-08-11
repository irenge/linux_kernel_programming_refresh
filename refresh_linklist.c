#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(odd_numbers_list);

struct odd_numbers_entry {
	struct list_head odd_list;
	int currents, following;
	char name[20];
};

static int __init systems_init(void) {
	struct odd_numbers_entry *system_entry;
	int j;

	for (j = 0; j <20; j++) {
		system_entry = kmalloc(sizeof(struct odd_numbers_entry), GFP_KERNEL);
		if(!system_entry) {
			pr_info("Failed to allocate memory for system list\n");
			return -ENOMEM;
		}
		if (j % 2 != 0) {
			(system_entry->currents) = j  ;
			(system_entry->following) = j + 20;
		} else {

			(system_entry->currents) = j + 11;
			(system_entry->following) = j + 13;
		}

		sprintf(system_entry->name, "odd_numbers_%d", j + 1);

		pr_info(" Odds: adding %s with current %d  and following %d to camel_list\n",
				system_entry->name, system_entry->currents, system_entry->following);


		//INIT_LIST(&system_entry->odd_list);
		list_add(&system_entry->odd_list, &odd_numbers_list);
	}
	return 0;
}
static void __exit systems_exit(void) {
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&odd_numbers_list)) {
		pr_info("Odd numbers list is empty, exiting\n");
		return;
	}
	pr_info("Odd numbers list is not empty, emptying\n");

	list_for_each_safe(list, tmp, &odd_numbers_list) {

		struct odd_numbers_entry *pili = list_entry(list, struct odd_numbers_entry, odd_list);
		list_del(&pili->odd_list);

		pr_info("Odd numbers (exit) : %s with current %d and following %d removed from list\n",pili->name, pili->currents, pili->following);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&odd_numbers_list))
		pr_info("Done: Odd numbers list is empty!\n");
	else
		pr_info("Odd numbers list is still NOT empty though!\n");


}
module_init(systems_init);
module_exit(systems_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");



