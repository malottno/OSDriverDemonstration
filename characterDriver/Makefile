obj-m := charDriver.o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc charTest.c -o charTest

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm charTest

test:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc charTest.c -o charTest
	chmod +x ./charTest
	./charTest >> charTest.log

rmall:
	rmall '*.o' '*.ko' '*.mod.c' Mod* mod* '.[a-z]*' '.tmp_versions/*'
	rmdall .tmp_versions
