# 
# IMG: image file, default is the first *.img in current directory
# ISO: install iso file, specify it when you want to install a new os
#

with_debug ?=0

with_display ?= 0
 with_vnc ?= 0

# cpu core and memory
BASE ?= -smp $(shell nproc) -m 4G
EXTPARM ?=

# if install iso, you should define ISO
ifeq ($(MAKECMDGOALS),install)
  ISO ?=
  ifeq ($(ISO),)
    $(error "ISO is not defined")
  endif
  # use the name of ISO as the name of IMG
  IMG ?= $(basename $(notdir $(ISO))).img
  INSTALL := -boot d -cdrom $(ISO)
else
  IMG ?= $(firstword $(wildcard *.img))
endif

ifeq ($(with_display),1)
  ifeq ($(with_vnc),1)
    VNC := -vnc :1
  endif
  DISPLAY :=  $(VNC)
else
  # -nographic
  DISPLAY := -display none -serial mon:stdio
endif

ifeq ($(with_debug),1)
DEBUG := -s -S
endif

ifneq ($(KERNEL),)
  KERNEL_PARM ?= "root=/dev/sda rw console=ttyS0 nokaslr"
  LINUX_KERNEL := -kernel $(KERNEL) \
  	-append "$(KERNEL_PARM)"
endif

KVM := --enable-kvm -cpu host
# NET := -net nic -net tap,ifname=tap0,script=no
NET := -net nic,model=virtio -net user
DISK := -drive file=$(IMG),index=0,media=disk,format=qcow2
# DISK := -hda $(IMG)

run:
	qemu-system-x86_64 $(BASE) $(KVM) $(DEBUG) \
		$(DISPLAY) \
		$(NET) \
		$(INSTALL) \
		$(LINUX_KERNEL) \
		$(DISK) \
		$(EXTPARM)


install: run create_img

create_img:
	qemu-img create $(IMG) 128G
	mkfs.ext4 $(IMG)

help:
	@echo 'run: run the IMG'
	@echo '	$$IMG: image file, default is the first *.img in current directory'
	@echo '	$$KERNEL: kernel file'
	@echo 'install: install a new os'
	@echo '	$$ISO: install iso file, specify it when you want to install a new os'
	@echo 'help: print this message'

.PHONY: run create_img help
