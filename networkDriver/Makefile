obj-m := netDriver.o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

test:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	insmod ./netDriver.ko $* || exit 1
	ifconfig NetDevEx 192.168.0.1
	ping 192.168.0.1 -c 10 >> netTest.log
	ifconfig NetDevEx >> netTest.log
	rmmod netDriver
	cat netTest.log

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	insmod ./netDriver.ko $* || exit 1
	ifconfig NetDevEx 192.168.0.1

uninstall:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rmmod netDriver

rmall:
	rmall '*.o' '*.ko' '*.mod.c' Mod* mod* '.[a-z]*' '.tmp_versions/*'
	rmdall .tmp_versions
