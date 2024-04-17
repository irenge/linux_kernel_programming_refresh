#include <linux/module.h>
#include <linux/workqueue.h>  
#include <linux/interrupt.h>
#include <linux/init.h>

#define SHARED_IRQ  174

static unsigned int irq = SHARED_IRQ, mydev_id, irq_counter;
int ret;

void wq_fn(struct work_struct *work); 

DECLARE_WORK(workqueue, wq_fn);

/*Workqueue Function*/
void wq_fn(struct work_struct *work)
{
	pr_info("Executing Workqueue Function\n");
}
/*
   static struct my_dat 
   {
   int  irq;
   struct work_struct work;
   };

   static void w_fun (struct work_struct *w_arg)
   {
   struct my_dat *data = container_of (w_arg, struct my_dat, work);
   atomic_inc(data->irq);
   }
   */
static irqreturn_t my_interrupt(int irq, void *dev_id) {
	irq_counter++;
	pr_info("In the ISR: counter = %d\n", irq_counter);
	schedule_work(&workqueue);
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
MODULE_DESCRIPTION("Learning interrupti and workqueue");
MODULE_LICENSE("GPL v2");

