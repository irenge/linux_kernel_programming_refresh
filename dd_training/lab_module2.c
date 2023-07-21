#include <linux/module.h>
#include <linux/init.h>

extern int og;
extern void print_face(int *num);
extern int x;
extern int y;
extern void avgx(int x, int y);

static int __init mmstart(void) {
	x = 10;
	y = 12;
	og = 12;

	pr_info("second module says 'Hi' ");
	avgx(x, y);
	print_face(&og);

	return 0;
}

static void __exit mmexit(void) {
	pr_info("Hi my man");
	print_face(&og);
	avgx(x, y);
}

module_init(mmstart);
module_exit(mmexit);

MODULE_DESCRIPTION("Stacked modules demonstration");
MODULE_AUTHOR("Jules Irenge <jbi.octave@gmail.com");
MODULE_LICENSE("GPL v2");
