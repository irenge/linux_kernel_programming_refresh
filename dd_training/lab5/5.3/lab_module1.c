#include <linux/module.h>
#include <linux/init.h>

int og = 12;

EXPORT_SYMBOL(og);

int x, y;

EXPORT_SYMBOL(x);
EXPORT_SYMBOL(y);

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
		if(*num > 20)
			break;

	}
}

EXPORT_SYMBOL(print_face);

void avgx(int x, int y) {

	int avg;
	avg = ((x+y)/2);
	pr_info("The average of %d and %d is %d", x, y, avg);
}

EXPORT_SYMBOL(avgx);

static int __init mstart(void) {
	
	int n;
	
	pr_info("Hi");

	for(n = 0; n < 4; n++)
		print_face(&og);
	return 0;
}

static void __exit mexit(void) {
	
	pr_info("Hello");
	
	print_face(&og);
}

module_init(mstart);
module_exit(mexit);

MODULE_DESCRIPTION("Stacked modules demonstration");
MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com");
MODULE_LICENSE("GPL v2");
