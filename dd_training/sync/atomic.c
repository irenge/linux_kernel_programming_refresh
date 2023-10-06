#include <linux/module.h>
#include <linux/types.h>

static int __init atom_init(void) {
	atomic_t v;
	atomic_t u = ATOMIC_INIT(0);
	atomic_set(&v, 4);
	pr_info("\nset v = %d\n", atomic_read(&v));
	atomic_add(2, &v);
	pr_info("\nadd 2 to v, v = %d\n", atomic_read(&v));
	atomic_inc(&v);
	pr_info("\nIncrease v by 1, v = %d\n", atomic_read(&v));
	return 0;
}

static void atom_exit(void) {
	pr_info("\nBye bye\n");
}

module_init(atom_init);
module_exit(atom_exit);

MODULE_LICENSE("GPL v2");
