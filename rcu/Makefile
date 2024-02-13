obj-m +=  publish.o subscribe.o linkedlist.o list.o listrcu.o

export KROOT=/lib/modules/6.6.13-gentoo-gentoo-dist/build

allofit:  modules
modules:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean

clean: kernel_clean
	rm -rf   Module.symvers modules.order
