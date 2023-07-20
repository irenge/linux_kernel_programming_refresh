### Automatic Makefile generation by 'genmake' script        ####
### Copyright, Jerry Cooperstein, coop@linuxfoundation.org 2003-2023 ####
### License: GPLv2 ###

obj-m +=  char_drivers.o ioctl_dev.o

export KROOT=/lib/modules/6.5.0-0.rc2.217.vanilla.fc38.x86_64/build

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
	 test_ioctl

clean: kernel_clean
	rm -rf  test_ioctl  Module.symvers modules.order
