#include<linux/init.h>
#include <linux/random.h>
#include <linux/module.h>
#include <linux/types.h>

static int __init xrandom_init(void) {
        u64 i=4,j;
	int z;
	u64 k,l;
	for ( z = 0; z < 12; z++) {
	get_random_bytes(&k, sizeof(u64));
	k = k % 10;
	for (l=0; l < k; l++) {
	get_random_bytes(&i, sizeof(u64));
	j = i %100;
	pr_info("%llu: i is %llu and J is %llu\n",k,i,j);
	}
	pr_info("\n=======================\n");
	}
	return 0;
}

static void __exit xrandom_exit(void) {
        
	pr_info("\nBye bye\n");
}

module_init(xrandom_init);
module_exit(xrandom_exit);

MODULE_LICENSE("GPL v2");

