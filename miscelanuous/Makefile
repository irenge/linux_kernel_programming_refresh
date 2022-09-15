obj-m += lab_taints.o hello_kernel.o iterate_over_processes.o iterate_over_threads.o utsname.o list.o refresh_linklist.o symtable_examp.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
