obj-m += hrtimers.o lab_periodic_timers.o lab_periodic_timer.o driver_multi_timer.o driver_timer.o hrexamp.o hrtimer_example.o inserting_delays.o ktimers.o low_res_timers.o

export KROOT=/lib/modules/6.5.7-gentoo-dist/build

allofit:  modules
modules:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean

clean: kernel_clean
	rm -rf   Module.symvers modules.order
