#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

MODULE_LICENSE("GPL v2");

static LIST_HEAD(simple_list);

struct sl_entry {
	struct list_head tlist; /* link in list */
	int a;
};

static int __init todos_init(void)
{
	struct sl_entry *q;
	struct sl_entry *m;
	int j;
	struct list_head *p;
	//struct todos_entry *f;

	q = kmalloc(sizeof(struct todos_entry), GFP_KERNEL);

	if(!q) {
		pr_info("Failed to allocate memory for ToDos list\n");
		return -ENOMEM;
	}

	q->a = 2;
	list_add(&q->tlist, &simple_list);

	//struct list_head *p;
	//struct todos_entry *f;


	pr_info("\nTraversing a list ");
	pr_info(" ====================");

	list_for_each(p, &simple_list) {

		struct sl_entry *f = list_entry(p, struct sl_entry, tlist);
		pr_info("\n % d", f->a);


	}
	// Add to todo list



	m = kmalloc(sizeof(struct sl_entry), GFP_KERNEL);

	if(!m) {
		pr_info("Failed to allocate memory for ToDos list\n");
		return -ENOMEM;
	}
	m->a = 6;

	//list_add_tail(&mwisho->tlist, &todos_list);
	list_move (&m->tlist, &simple_list);

	list_for_each(p, &simple_list) {
		struct sl_entry *f = list_entry(p, struct sl_entry, tlist);
		pr_info("%d ", f->a);
	}
	pr_info("\n");



	return 0;
}

static void __exit todos_exit(void) 
{
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&simple_list)) {
		pr_info(" Todos list is empty, exiting\n");
		return;
	}
	pr_info("Todos list is not empty, emptying\n");

	list_for_each_safe(list, tmp, &simple_list) {

		struct sl_entry *pili = list_entry(list, struct sl_entry, tlist);
		list_del(&pili->tlist);

		pr_info("T %d removed from list\n", pili->a);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&simple_list))
		pr_info("Done: Todos list is empty!\n");
	else
		pr_info("Todos list is still NOT empty though!\n");


}
module_init(todos_init);
module_exit(todos_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");

