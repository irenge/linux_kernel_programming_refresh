#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>


#define MAXIRQS nr_irqs

int *interrupts;

static irqreturn_t my_interrupt(int irq, void *dev_id) {
	interrupts[irq]++;
	return  IRQ_NONE;
}

static int __init my_interrupt_init(void) {
	
	int irq;
	interrupts = kmalloc(MAXIRQS * sizeof(int), GFP_KERNEL);

	for(irq = 0; irq < MAXIRQS; irq++) {
		interrupts[irq] = -1;
		if((!request_irq(irq, my_interrupt, IRQF_SHARED, "hello_interrupt", interrupts))) {
			interrupts[irq] = 0;
			pr_info("\nSucceeded in registering IRQ %d\n", irq);
		}

	}
	return 0;
}

static void __exit my_interrupt_exit(void)
{
	int irq;

	for(irq = 0; irq < MAXIRQS; irq++) {
		if (interrupts[irq] >= 0) {
			free_irq(irq, interrupts);


		}

	}

	kfree(interrupts);
	pr_info("Successfully unloading, irq_counter = %d\n", irq);
}

module_init(my_interrupt_init);
module_exit(my_interrupt_exit);

MODULE_AUTHOR("Jules");
MODULE_DESCRIPTION("Learning interrupt");
MODULE_LICENSE("GPL v2");
