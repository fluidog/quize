
MODULE := test-netlink-kernel
FILES := kernel.c

ifdef KERNELRELEASE 

obj-m += $(MODULE).o
$(MODULE)-m := $(FILES:%.c=%.o)

else

run: build
	# insmod $(MODULE).ko
	# ./test-netlink-user
	# rmmod $(MODULE)

build clean: test-netlink-user test-netlink-kernel

test-netlink-kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

test-netlink-user : user.c
	gcc $< --static -o $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm -f $^

endif
