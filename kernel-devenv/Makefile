KERNEL_VERSION ?= v5.15.79
KERNEL_SRC ?= /root/workspace/kernel/linux

_OUTPUT ?= $(PWD)/output
__KERNEL := $(_OUTPUT)/linux/arch/x86/boot/bzImage
__KERNEL_PARM := "root=/dev/sda1 rw console=ttyS0 nokaslr"
# _WITH_KERNEL := "KERNEL=$(__KERNEL)" KERNEL_PARM=${__KERNEL_PARM}
#_RELEASE ?= jammy
# _RELEASE ?= focal
_IMG := $(_OUTPUT)/$(_RELEASE).img
#_EXTPARM := "-drive file=output/user-data.img,format=raw"


run-qemu: ${_IMG}
	make -f Makefile.qemu run ${_WITH_KERNEL} \
		IMG=${_IMG} EXTPARM=${_EXTPARM}

linux:
	git -C ${KERNEL_SRC} checkout ${KERNEL_VERSION}
	make -C ${KERNEL_SRC} O=${_OUTPUT}/linux -j$(shell nproc) defconfig
	cp linux.config ${_OUTPUT}/linux/.config && make -C ${_OUTPUT}/linux olddefconfig
	make -C ${_OUTPUT}/linux -j$(shell nproc) all

linux-%:
	make -C ${_OUTPUT}/linux -j$(shell nproc) $(patsubst linux-%,%,$@)
