#include<linux/init.h>
#include <linux/random.h>
#include <linux/module.h>
#include <linux/types.h>

static int __init xrandom_init(void) {
        u64 i=4,j;
	get_random_bytes(&i, sizeof(u64));
	j = i %100;
	pr_info("i is %d and J is %d\n",i,j);
	return 0;
}

static void __exit xrandom_exit(void) {
        
	pr_info("\nBye bye\n");
}

module_init(xrandom_init);
module_exit(xrandom_exit);

MODULE_LICENSE("GPL v2");

