obj-m +=  cache_7.o exercise7_dynamic.o exercise7_misc.o exercise7_udev.o exercise7_withpriv.o

export KROOT=/lib/modules/6.4.10-200.fc38.x86_64/build

allofit:  userprogs modules
modules:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean

userprogs:
	@$(MAKE) \
		CFLAGS="-O2 -Wall -pthread" \
	 lab_char_test

clean: kernel_clean
	rm -rf  lab_char_test  Module.symvers modules.order
