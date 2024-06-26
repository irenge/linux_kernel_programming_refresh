#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>

#define SHARED_IRQ 156

static unsigned int irq = SHARED_IRQ, mydev_id, irq_counter;
int ret;

// interrupt handler
static irqreturn_t my_interrupt(int irq, void *dev_id) {
	struct timeval tv;
	struct tm broken;
	int written;
	do_gettimeofday(&tv);


	time64_to_tm(tv.tv_sec, 0, &broken);
	pr_info("%d:%d:%d:%ld\n", broken.tm_hour, broken.tm_min, broken.tm_sec, tv.tv_usec);
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);
	return  IRQ_NONE;
}


static int __init my_interrupt_init(void) {
	if((ret = request_irq(irq, my_interrupt, IRQF_SHARED, "hello_interrupt", &mydev_id))){
		pr_info("\nFailed to allocate irq %d\n", irq);
		return ret;
	}
	pr_info("\n%d: Successfully loading ISR handler\n", ret);
	return 0;
}

static void __exit my_interrupt_exit(void)
{
	free_irq(irq, &mydev_id);
	pr_info("Successfully unloading, irq_counter = %d\n", irq);
}

module_init(my_interrupt_init);
module_exit(my_interrupt_exit);

MODULE_AUTHOR("Jules");
MODULE_DESCRIPTION("Learning interrupt");
MODULE_LICENSE("GPL v2");
