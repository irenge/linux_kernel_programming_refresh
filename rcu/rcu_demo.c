#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

struct field {
	int a;
	int b;
	int c;
};

struct field  *gp = NULL;
static int __init my_init(void) 
{
	struct field *p, *q;
	p = kmalloc(sizeof(struct field), GFP_KERNEL);
	p->a = 1;
	p->b = 2;
	p->c = 3;
	pr_info("a = %d, b = %d and c = %d\n", p->a, p->b, p->c);
	rcu_assign_pointer(gp, p); /* The rcu_assign_pointer() would publish the new structure, forcing both the compiler and the CPU to execute the assignment to gp after the assignments to the fields referenced by p. 
	*/
	// This enfoce ordering on the updater 
	gp->a += 1;
	gp->b += 1;
	gp->c += 1;


	pr_info("p->a = %d, p->b = %d and p->c = %d\n", p->a, p->b, p->c);
	pr_info("gp->a = %d, gp->b = %d and gp->c = %d\n", gp->a, gp->b, gp->c);
	
	rcu_read_lock();
	q = rcu_dereference(gp); /* subscribing */
	if (q != NULL) {
		q->a += 1;
		q->b += 1;
		q->c += 1;
	}
	rcu_read_unlock();

	pr_info("a = %d, b = %d and c = %d\n", q->a, q->b, q->c);
	pr_info("Before was a = %d, b = %d and c = %d\n", gp->a, gp->b, gp->c);

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
