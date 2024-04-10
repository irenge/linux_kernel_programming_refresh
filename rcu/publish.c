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
	struct field *p;
	p = kmalloc(sizeof(struct field), GFP_KERNEL);
	p->a = 1;
	p->b = 2;
	p->c = 3;
	rcu_assign_pointer(gp, p); /* The rcu_assign_pointer() would publish the new structure, forcing both the compiler and the CPU to execute the assignment to gp after the assignments to the fields referenced by p. 
	*/
	// This enfoce odering on the updater 

	pr_info("a = %d, b = %d and c = %d\n", gp->a, gp->b, gp->c);
	return 0;
}



static void __exit my_exit(void)
{
	pr_info("Unload module");
}
module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_LICENSE("GPL v2");
