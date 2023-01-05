## Makefile for the Linux kernel module

TARGET := list_example
FILES := main.c

ifdef KERNELRELEASE

obj-m += $(TARGET).o
$(TARGET)-m := $(FILES:%.c=%.o)

else

KDIR ?= /lib/modules/$(shell uname -r)/build

build:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f $^

run: build
	insmod $(TARGET).ko
	rmmod $(TARGET)
endif
