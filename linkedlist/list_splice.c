#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

static LIST_HEAD(update_list);
static LIST_HEAD(insert_list);
static LIST_HEAD(demo_list);

struct driver_entry {
	struct list_head tlist; /* link in list */
	int data;
};

static int __init simple_init(void)
{
	struct driver_entry *q;
	struct driver_entry *r;
	struct list_head *p;
	struct driver_entry *tx; // traversing 
	//struct driver_entry *a,*b;
	//struct driver_entry *ds;
	//struct driver_entry *tst;
	//struct driver_entry *z;

	q = kmalloc(sizeof(struct driver_entry), GFP_KERNEL);
	if(!q)
		return -ENOMEM;
	q->data = 2 ;
	list_add(&q->tlist, &update_list); //linking the list 

	pr_info("\nTraversing a update list ");
	pr_info(" ====================");

	list_for_each(p, &update_list) {

		struct driver_entry *f = list_entry(p, struct driver_entry, tlist);
		pr_info("\n % d", f->data);
	}

	//struct driver_entry *r;
	r = kmalloc(sizeof(struct driver_entry), GFP_KERNEL);
	if(!r)
		return -ENOMEM;

	r->data = 11;
	list_add(&r->tlist, &insert_list);


	pr_info("\nTraversing insert list \n");
	pr_info("\n========================\n");

	list_for_each_entry(tx, &insert_list, tlist) {
		pr_info("\n%d\n", tx->data);
	}
	struct driver_entry *a, *b;

	a = kmalloc(sizeof(struct driver_entry), GFP_KERNEL);
	if(!a)
		return -ENOMEM;
	a->data = 25;
	list_add(&a->tlist, &demo_list);

	b = kmalloc(sizeof(struct driver_entry), GFP_KERNEL);
	if(!b)
		return -ENOMEM;
	b->data = 52;
	list_add(&b->tlist, &demo_list);

	pr_info("\nTraversing the demo list\n");

	struct driver_entry *ds;
	list_for_each_entry(ds, &demo_list, tlist) {
		pr_info("\n%d\n", ds->data);
	}

	pr_info("\nSplicing: joining insert list into  update list \n");

	list_splice(&insert_list, &update_list);

	struct driver_entry *test;
	list_for_each_entry(test, &update_list, tlist) {
		pr_info("\n%d\n", test->data);
	}
	// getting the first entry 
	struct driver_entry *y = list_first_entry_or_null(&update_list, struct driver_entry, tlist);
	pr_info("\nFirst entry is %d\n",y->data);


	// getting the last entry 
	struct driver_entry *z = list_last_entry(&update_list, struct driver_entry, tlist); 

	pr_info("\nlast entry is %d\n",z->data);

	// splice 
	list_splice(&demo_list, &y->tlist);
	pr_info("\nUpdate list\n");
	
	struct driver_entry *tst;

	list_for_each_entry(tst, &update_list, tlist) {
		pr_info("\n%d\n", tst->data);
	}

	return 0;
}

static void __exit simple_exit(void) 
{
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&update_list)) {
		pr_info("\nThe list is empty, exiting\n");
		return;
	}
	pr_info("\nThe list is not empty, emptying\n");
	pr_info("\nTraversing using list_for_each_safe\n");

	list_for_each_safe(list, tmp, &update_list) {

		struct driver_entry *pili = list_entry(list, struct driver_entry, tlist);
		list_del(&pili->tlist);

		pr_info("\n%d removed from list\n", pili->data);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&update_list))
		pr_info("Done: Simple list is empty!\n");
	else
		pr_info("Simple list is still NOT empty though!\n");


}
module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("simple linked list demo");
MODULE_LICENSE("GPL v2");

