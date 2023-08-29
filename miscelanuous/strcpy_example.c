#include <linux/module.h>
#include <linux/init.h>
#include  <linux/slab.h>

char * str = "demonstrate strcpy()";
char * example;
//char examples[21];
char* examples;



static int __init my_init(void)
{

	size_t count = strlen(str) + 1;
	pr_info("\nsize is %ld\n", count);

	size_t n;
	example =  kmalloc(count, GFP_KERNEL);
	examples =  kmalloc(count, GFP_KERNEL);
	//if (!example)
	//	return -ENOMEM;

	//c = sizeof(str);

	n = strscpy(example, str, count);
//	strncpy(examples, str, sizeof(examples));
	strncpy(examples, str, count);


	//if (n > count){
	pr_info("\n%ld copied\n",n);

	pr_info("\n%s\n", example);
	pr_info("\n%s\n", examples);
	
	pr_info("\nModule loaded\n");
	return 0;
}
static void __exit my_exit(void)
{
	pr_info("Module unloaded\n");
	kfree(example);
	kfree(examples);

}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");


