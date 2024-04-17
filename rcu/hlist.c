/*Code to show usage of hllist :
 * !! Not Working Rogue for learning urpose only */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/list.h>
#include<linux/slab.h>

#define MAX_NUM_ARGS 10

struct k_list {
	struct hlist_node test_list;
	int temp;
};


static int __init create_list_init(void) {

	//struct k_list *one,*two,*three,*entry,*four,*five;
	struct k_list **alist;
	struct k_list *entry;
	struct hlist_head test_head;
	//struct hlist_head *ptr;
	struct hlist_node*tmp;
	int i = 0;

	alist = kmalloc(MAX_NUM_ARGS * sizeof(struct k_list *), GFP_KERNEL);

	for (i = 0; i < MAX_NUM_ARGS; i++)
		alist[i]=kmalloc(sizeof(struct k_list *),GFP_KERNEL);
	for (i = 0; i < MAX_NUM_ARGS; i++)
		alist[i]->temp = 6 * i;


	INIT_HLIST_HEAD(&test_head);
	for (i = 0; i < MAX_NUM_ARGS; i++)
		hlist_add_head(&alist[i]->test_list,&test_head);
	i = 0;
	hlist_for_each_entry_safe(entry,tmp,&test_head,test_list){
		++i;
		printk(KERN_INFO " Entry %d %d  \n \n ", i,entry->temp);

	}

	printk(KERN_INFO " Done....  \n \n\n ");
	return 0;

}

static void __exit create_list_exit(void) {
	printk(KERN_INFO "\n exiting  \n \n\n ");
}

module_init(create_list_init);
module_exit(create_list_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
