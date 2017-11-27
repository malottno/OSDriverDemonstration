obj-m := charDriver.o charDriverM.o charDriverRO.o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc charTest.c -o charTest -lpthread

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm charTest
	rm -f /dev/charDevice[0-3]

test:
	$(MAKE) -C $(KDIR) M=$(PWD) modules 2>charTest.log
	gcc charTest.c -o charTest -lpthread 2>charTest.log
	chmod +x ./charTest
	insmod ./charDriver.ko $* || exit 1
	rm -f /dev/charDevice[0-3]
	mknod /dev/charDevice0 c 250 0
	mknod /dev/charDevice1 c 250 1
	
	for i in 1 2 3 4 5 6 7 8 9 10 ; do \
		./charTest >> charTest.log ; \
	done

	rmmod charDriver
	insmod ./charDriverM.ko $* || exit 1
	rm -f /dev/charDevice[0-3]
	mknod /dev/charDevice0 c 250 0
	mknod /dev/charDevice1 c 250 1

	for i in 1 2 3 4 5 6 7 8 9 10 ; do \
		./charTest >> charTestM.log ; \
	done
	
	rmmod charDriverM
	insmod ./charDriverRO.ko $* || exit 1
	rm -f /dev/charDevice[0-3]
	mknod /dev/charDevice0 c 250 0
	mknod /dev/charDevice1 c 250 1

	for i in 1 2 3 4 5 6 7 8 9 10 ; do \
		./charTest >> charTestRO.log ; \
	done

	rmmod charDriverRO

	cat charTest.log
	cat charTestM.log
	cat charTestRO.log

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	insmod ./charDriver.ko $* || exit 1

uninstall:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm charTest
	rm charTestM
	rm -f /dev/charDevice[0-3]
	rmmod charDriver

rmall:
	rmall '*.o' '*.ko' '*.mod.c' Mod* mod* '.[a-z]*' '.tmp_versions/*'
	rmdall .tmp_versions
