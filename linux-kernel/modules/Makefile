## Makefile for the Linux kernel module

MODULE := hello_module
FILES := main.o

ifdef KERNELRELEASE

obj-m += $(MODULE).o
$(MODULE)-m := $(FILES)

else

ifndef KERNEL
KERNEL := /lib/modules/$(shell uname -r)/build
endif

build:
	make -C $(KERNEL) M=$(shell pwd) modules
clean:
	make -C $(KERNEL) M=$(shell pwd) clean
	rm -f $^

run: build
	insmod $(MODULE).ko
	rmmod $(MODULE)
endif
