linux-src := /root/workspace/kernel/linux
busybox-src := /root/workspace/busybox
buildroot-src := /root/workspace/buildroot

linux-out := $(shell pwd)/output/linux
busybox-out := $(shell pwd)/output/busybox
buildroot-out := $(shell pwd)/output/buildroot

rootfs := $(shell pwd)/output/rootfs.img
# rootfs := $(shell pwd)/output/buildroot/images/rootfs.ext2

linux := $(linux-out)/arch/x86_64/boot/bzImage
# linux := $(shell pwd)/output/buildroot/images/bzImage

# 
run-qemu:
	qemu-system-x86_64 \
		-kernel ${linux} \
		-append "root=/dev/sda rw console=ttyS0 nokaslr" \
		-hda ${rootfs} \
		-net nic -net tap,ifname=tap0,script=no \
		--enable-kvm -cpu host\
		-m 8G \
		-smp 8 \
		-s \
		-display none -serial mon:stdio 

build-linux:
	make -C ${linux-src} O=${linux-out} x86_64_defconfig && \
		make -C ${linux-src} O=${linux-out} -j16


build-rootfs:
	mkdir -p /run/.rootfs && mount -o loop ${rootfs} /run/.rootfs 
	debootstrap jammy /run/.rootfs 
	umount /run/.rootfs && rm -rf /run/.rootfs


build-buildroot:
	make -C ${buildroot-src} O=${buildroot-out} qemu_x86_64_defconfig && \
		make -C ${buildroot-src} O=${buildroot-out} -j16 all


create-img:
	qemu-img create -f qcow2 ${rootfs} 256G
	mkfs.ext4 ${rootfs}
