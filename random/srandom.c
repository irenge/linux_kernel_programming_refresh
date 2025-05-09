#include<linux/init.h>
#include <linux/random.h>
#include <linux/module.h>
#include <linux/types.h>

static int __init srandom_init(void) {
        short unsigned i=4,j;
	int z;
	short unsigned  k,l;
	for ( z = 0; z < 12; z++) {
	get_random_bytes(&k, sizeof(short unsigned ));
	k = k % 10;
	for (l=0; l < k; l++) {
	get_random_bytes(&i, sizeof(short unsigned));
	j = i %100;
	pr_info("%llu: i is %d and J is %d\n",k,i,j);
	}
	pr_info("\n=======================\n");
	}
	return 0;
}

static void __exit srandom_exit(void) {
        
	pr_info("\nBye bye\n");
}

module_init(srandom_init);
module_exit(srandom_exit);

MODULE_LICENSE("GPL v2");

