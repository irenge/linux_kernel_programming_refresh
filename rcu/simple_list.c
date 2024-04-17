#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>


static LIST_HEAD(simple_list);
//static LIST_HEAD(next_list);

struct simple_entry {
	struct list_head tlist; /* link in list */
	int a;
};

static int __init simple_init(void)
{
	struct simple_entry *q;
	struct simple_entry *m, *n;
	struct list_head *p;

	q = kmalloc(sizeof(struct simple_entry), GFP_KERNEL);
	if(!q)
		return -ENOMEM;

	q->a = 2 ;
	list_add(&q->tlist, &simple_list); //linking the list 


	pr_info("\nTraversing a list ");
	pr_info(" ====================");

	list_for_each(p, &simple_list) {

		struct simple_entry *f = list_entry(p, struct simple_entry, tlist);
		pr_info("\n % d", f->a);


	}
	pr_info("\nTraversing over\n");


	pr_info("\n...adding element...\n");
	m = kmalloc(sizeof(struct simple_entry), GFP_KERNEL);
	if(!m)
		return -ENOMEM;
	m->a = 6;
	list_add(&m->tlist, &simple_list);

	pr_info("\nTraversing again using list_for_each\n");

	list_for_each(p, &simple_list) {
		struct simple_entry *f = list_entry(p, struct simple_entry, tlist);
		pr_info("\n %d \n", f->a);
	}

	pr_info("\n...add element to tail...");
	n = kmalloc(sizeof(struct simple_entry), GFP_KERNEL);
	if(!n) 
		return -ENOMEM;
	n->a = 26;
	list_add_tail(&n->tlist, &simple_list);

	pr_info("\nTraversing using list_for_each_entry");
	struct simple_entry *temp;
	list_for_each_entry(temp, &simple_list, tlist) {
		pr_info("\n%d\n", temp->a);
	}
	pr_info("\nDemo list_splice\n");

	struct simple_entry *lsx;
	//*lsy;
//	struct list_head next_list;

	//INIT_LIST_HEAD(&next_list);

	lsx = kmalloc(sizeof(struct simple_entry), GFP_KERNEL);
//	lsy = kmalloc(sizeof(struct simple_entry), GFP_KERNEL);
//	if((!lsx) || (!lsy))
//		return -ENOMEM;

	lsx->a = 11;
//	list_add(&lsx->tlist, &simple_list);

//	lsy->a = 21;
//	list_add(&lsy->tlist, &next_list);

/*	struct simple_entry *tmp;
	list_for_each_entry(tmp, &next_list, tlist) {
		pr_info("\n%d\n", tmp->a);
	}
*/
	pr_info("\nSplicing...\n");

	list_splice(&simple_list, &lsx->tlist);
	
	struct simple_entry *tx;
        list_for_each_entry(tx, &simple_list, tlist) {
                pr_info("\n%d\n", tx->a);
        }



	return 0;
}

static void __exit simple_exit(void) 
{
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&simple_list)) {
		pr_info("\nThe list is empty, exiting\n");
		return;
	}
	pr_info("\nThe list is not empty, emptying\n");
	pr_info("\nTraversing using list_for_each_safe\n");

	list_for_each_safe(list, tmp, &simple_list) {

		struct simple_entry *pili = list_entry(list, struct simple_entry, tlist);
		list_del(&pili->tlist);

		pr_info("\n%d removed from list\n", pili->a);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&simple_list))
		pr_info("Done: Simple list is empty!\n");
	else
		pr_info("Simple list is still NOT empty though!\n");


}
module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("simple linked list demo");
MODULE_LICENSE("GPL v2");

