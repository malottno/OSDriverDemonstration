obj-m := netDriver.o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc netTest.c -o netTest

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm netTest

test:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc netTest.c -o netTest
	./netTest >> netTest.log

rmall:
	rmall '*.o' '*.ko' '*.mod.c' Mod* mod* '.[a-z]*' '.tmp_versions/*'
	rmdall .tmp_versions
