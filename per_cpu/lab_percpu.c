#include <linux/percpu.h>
#include <linux/module.h>
#include <linux/init.h>

static DEFINE_PER_CPU(long, cpuvar) = 10;

static DEFINE_SPINLOCK(cpulock);

static long __percpu *cpualloc;


// From Teacher
/*
static int which_cpu(void) {
	int cpu;
	spin_lock(&cpulock);
	cpu = smp_processor_id();
	spin_unlock(&cpulock);
	return cpu;

}*/

// From Jules

int det_cpu(void) {
	int cpu;
	cpu = get_cpu();
	put_cpu();
	return cpu;
}

static int __init my_init(void)
{
	int cpu;

	pr_info("cpu var is %ld  \n", get_cpu_var(cpuvar)++);
	cpu = det_cpu();
	// cpu = which_cpu();
	put_cpu_var(cpuvar);

	cpualloc = alloc_percpu(long);

	*per_cpu_ptr(cpualloc, cpu) = 1000;

	// pr_info("Hello: module loaded at 0x%p\n", my_init);
	pr_info("Set variables on cpu:%d\n", cpu);

	return 0;
}

static void __exit my_exit(void) {
	int cpu;
	pr_info("Now on cpu: %d\n", det_cpu());

	for_each_online_cpu(cpu) {
		pr_info("cpu: %d cpuvar=%ld per_cpu_ptr=%ld\n", \
			 cpu, per_cpu(cpuvar, cpu), *per_cpu_ptr(cpualloc, cpu));
	}
	free_percpu(cpualloc);

	pr_info("Bye: module unloaded from 0x%p\n", my_exit);

}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules");
MODULE_LICENSE("GPL v2");


