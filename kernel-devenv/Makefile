KERNEL_VERSION ?= v5.15.79
KERNEL_SRC ?= /root/workspace/kernel/linux

_OUTPUT ?= $(PWD)/output
_KERNEL := $(_OUTPUT)/linux/arch/x86/boot/bzImage
_KERNEL_PARM := "root=/dev/sda1 rw console=ttyS0 nokaslr"
_RELEASE ?= jammy
_IMG := $(_OUTPUT)/$(_RELEASE).img
_EXTPARM := "-drive file=output/user-data.img,format=raw"

run-qemu:
	make -f Makefile.qemu run KERNEL=${_KERNEL} KERNEL_PARM=${_KERNEL_PARM}\
		IMG=${_IMG} EXTPARM=${_EXTPARM}

linux:
	git -C ${KERNEL_SRC} checkout ${KERNEL_VERSION}
	make -C ${KERNEL_SRC} O=${_OUTPUT}/linux -j$(shell nproc) defconfig
	cp linux.config ${_OUTPUT}/linux/.config && make -C ${_OUTPUT}/linux olddefconfig
	make -C ${_OUTPUT}/linux -j$(shell nproc) all

linux-%:
	make -C ${_OUTPUT}/linux -j$(shell nproc) $(patsubst linux-%,%,$@)

download_img: $(_IMG)
	curl -L -o $(_IMG) \
	https://cloud-images.ubuntu.com/$(RELEASE)/current/$(RELEASE)-server-cloudimg-amd64.img