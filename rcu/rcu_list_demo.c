#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

struct field {
	struct list_head list;
	int a;
	int b;
	int c;
};
LIST_HEAD(head);

struct field  *gp = NULL;

static int __init my_init(void) 
{
	struct field *p, *q, *qr;
	// struct field *qs = NULL;
	int i = 0;
	p = kmalloc(sizeof(struct field), GFP_KERNEL);
	p->a = 1;
	p->b = 2;
	p->c = 3;

	pr_info("\nAdded to list");

	INIT_LIST_HEAD(&p->list);
	list_add_rcu(&p->list, &head); /* publish */


	pr_info("a = %d, b = %d and c = %d\n", p->a, p->b, p->c);
	rcu_assign_pointer(gp, p); /* The rcu_assign_pointer() would publish the new structure, forcing both the compiler and the CPU to execute the assignment to gp after the assignments to the fields referenced by p. 
	*/
	// This enfoce ordering on the updater 
	gp->a += 1;
	gp->b += 1;
	gp->c += 1;

INIT_LIST_HEAD(&gp->list);

	list_add_rcu(&gp->list, &head);
	pr_info("\nAdd to list");
	
	pr_info("gp->a = %d, gp->b = %d and gp->c = %d\n", gp->a, gp->b, gp->c);

	rcu_read_lock();
	q = rcu_dereference(gp); /* subscribing */
	if (q != NULL) {
		q->a += 2;
		q->b += 2;
		q->c += 2;
	}
	rcu_read_unlock();

	pr_info("a = %d, b = %d and c = %d\n", q->a, q->b, q->c);
	INIT_LIST_HEAD(&q->list);
	list_add_rcu(&q->list, &head);


	// traverse using rcu -- subscribing */ 
rcu_read_lock();
	list_for_each_entry_rcu(qr, &head, list) {
		pr_info("%d: %d %d %d",i, qr->a, qr->b, qr->c);
		++i;
	}
rcu_read_unlock();

	return 0;
}



static void __exit my_exit(void)
{
	pr_info("\nUnload module");
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
