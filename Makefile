# Compile hello world kernel module

ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean