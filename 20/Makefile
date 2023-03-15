obj-m +=  lab_cache.o

export KROOT=/lib/modules/6.1.15-200.fc37.x86_64/build

allofit:  modules
modules:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean

clean: kernel_clean
	rm -rf   Module.symvers modules.order
