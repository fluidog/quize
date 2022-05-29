MODULE := hello_module
FILES := main.o

ifdef KERNELRELEASE

obj-m += $(MODULE).o
$(MODULE)-m := $(FILES)

else

run: build
	insmod $(MODULE).ko
	rmmod $(MODULE)

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm -f $^

endif