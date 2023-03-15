#include <linux/module.h>
#include <linux/init.h>
#include <linux/mempool.h>
#include <linux/slab.h>

#define SIZE 16384
#define MIN_NR 10240
static int size = SIZE, min_nr = MIN_NR;

module_param(size, int, 0444);
module_param(min_nr, int, 0444);

static char **bufs;
static mempool_t *my_mempool;

static int __init my_init(void)
{
	int j;

	my_mempool = mempool_create_kmalloc_pool(min_nr, size);
	pr_info("my_mempool = %p\n", my_mempool);

	bufs = kmalloc_array(min_nr, sizeof(ulong), GFP_KERNEL);

        for(j = 0; j < min_nr; j++) {
                bufs[j] = mempool_alloc(my_mempool, GFP_ATOMIC);
                if(!bufs[j]) {
                        pr_info("could not alloc bufs [%d]\n", j);
                } else {
                        pr_info(" bufs [%d] allocated\n", j);
                }
        }
	return 0;
}
static void __exit my_exit(void)
{
	int j;

	for(j = 0; j < min_nr; j++) {

		 if (bufs[j]) {
			 mempool_free(bufs[j], my_mempool);
			 pr_info("Freeing %d\n",j);
		 }  else {
			 pr_info("Problem freeing: %d\n", j);
		 }

	 }

}



module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("pooling memory...");
