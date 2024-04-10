obj-m +=  lab_module1.o lab_module2.o

<<<<<<< HEAD
export KROOT=/lib/modules/`uname -r`/build
=======
export KROOT=/lib/modules/6.4.10-200.fc38.x86_64/build
>>>>>>> 610bb83d0171b8ab389843d89d376a0c6a2f783e

allofit:  modules
modules:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean

clean: kernel_clean
	rm -rf   Module.symvers modules.order
