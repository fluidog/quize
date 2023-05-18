BUILDROOT ?= /root/workspace/buildroot

_OUTPUT := $(PWD)/output
_LINUX := $(OUTPUT)/buildroot/images/bzImage
_IMG := $(OUTPUT)/buildroot/images/rootfs.ext2

run-qemu:
	make -f Makefile.qemu run KERNEL=${_LINUX} IMG=${_IMG}

buildroot:
	make -C ${OUTPUT}/buildroot -j$(shell nproc) all

buildroot-defconfig:
	make -C ${BUILDROOT} O=${OUTPUT}/buildroot qemu_x86_64_defconfig

buildroot-%:
	make -C ${OUTPUT}/buildroot -j$(shell nproc) $(patsubst buildroot-%,%,$@)

linux-%:
	make -C ${OUTPUT}/linux -j$(shell nproc) $(patsubst linux-%,%,$@)


export KDIR := /root/workspace/quize/linux-kernel/de/output/linux
export CROSS_COMPILE := /root/workspace/quize/linux-kernel/de/output/buildroot/host/bin/x86_64-buildroot-linux-gnu-
