## Makefile for the Linux kernel module

MODULE := timer_example
FILES := main.c

ifdef KERNELRELEASE

obj-m += $(MODULE).o
$(MODULE)-m := $(FILES:%.c=%.o)

else

ifndef KDIR
KDIR := /lib/modules/$(shell uname -r)/build
endif

build:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f $^

run: build
	insmod $(MODULE).ko
	rmmod $(MODULE)
endif
