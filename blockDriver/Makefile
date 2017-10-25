obj-m := blockDriver.o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc blockTest.c -o blockTest

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm 'blockTest'

test:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc blockTest.c -o blockTest
	./blockTest >> blockTest.log

rmall:
	rmall '*.o' '*.ko' '*.mod.c' Mod* mod* '.[a-z]*' '.tmp_versions/*'
	rmdall .tmp_versions
