#include <linux/hashtable.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/init.h>

struct mystruct {
	int data ;
	struct hlist_node my_hash_list ;
} ;

static int  __init myhash_init(void) {


	int bkt;
	struct mystruct first,second, third, * curr;
	/* 2^3 = 8 buckets */
	DECLARE_HASHTABLE(a, 3);
	hash_init(a);

	/* initialising */
	first.data = 10;
	second.data = 20;
	third.data = 44;

	hash_add(a, &first.my_hash_list, first.data);
	hash_add(a, &second.my_hash_list, second.data);
	hash_add(a, &third.my_hash_list, third.data);

	hash_for_each(a, bkt, curr, my_hash_list){
		pr_info("data=%d is in bucket %d\n", curr->data, bkt);
	}
	// Remove elements.
	pr_info("Remove first item");
	hash_del(&first.my_hash_list);

	hash_for_each(a, bkt, curr, my_hash_list){
		pr_info("data=%d is in bucket %d\n", curr->data, bkt);
	}
	pr_info("Remove second item");
	hash_del(&second.my_hash_list);
	hash_for_each(a, bkt, curr, my_hash_list){
		pr_info("data=%d is in bucket %d\n", curr->data, bkt);
	}
	pr_info("Remove the third item ");
	hash_del(&third.my_hash_list);


	return 0;
}

static void __exit myhash_exit(void)
{
	pr_info("Bye: queue module unloaded from 0x%p\n", myhash_exit);
}

module_init(myhash_init);
module_exit(myhash_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");

