#include <linux/module.h>
#include <linux/init.h>
int og = 12;
EXPORT_SYMBOL(og);
void print_face(int *num){
	(*num)++;
	for (;;) {
		*num = *num + 1;

	if ((*num) % 2 == 1){
		pr_info("Yin: %d", *num);
		//*num = *num + 1;
	} else {
		pr_info("Yang: %d", *num);
	//	(*num)--;
	}
	if(*num >20)
		break;

	}
}
EXPORT_SYMBOL(print_face);
static int __init mstart(void) {
	pr_info("Hi");
	int n =0;
	for(;;){
		
		print_face(&og);
		if (n > 3)
			break;
		n++;
	}
	return 0;
}

static void __exit mexit(void) {
	pr_info("Hello");
        print_face(&og);
}
module_init(mstart);
module_exit(mexit);

MODULE_DESCRIPTION("Module to demonstrate the EXPORT_SYMBOL functionality");
MODULE_AUTHOR("Rishi Agrawal <rishi.b.agrawal@gmail.com");
MODULE_LICENSE("GPL v2");
