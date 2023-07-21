# 
# IMG: image file, default is the first *.img in current directory
# ISO: install iso file, specify it when you want to install a new os
#

with_debug ?=0

with_display ?= 0
 with_vnc ?= 0

# cpu and memory
BASE ?= -smp $(shell nproc) -m 4G
EXTPARM ?=

# if install iso, you should define ISO
ifeq ($(MAKECMDGOALS),install)
  with_display := 1
  ISO ?= $(firstword $(wildcard *.iso))
  $(warning Install $(ISO))
  # use the name of ISO as the name of IMG
  IMG ?= $(basename $(notdir $(ISO))).img
  INSTALL := -boot d -cdrom $(ISO)
else ($(MAKECMDGOALS),download_cloud_img)
  RELEASE ?= jammy
  $(warning Download cloud image of $(RELEASE))
  IMG ?= $(RELEASE).img
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


install: create_img run

create_img:
	qemu-img create -f qcow2 $(IMG) 128G

download_cloud_img: $(RELEASE)-server-cloudimg-amd64.img
	curl -L -o $(IMG) \
	https://cloud-images.ubuntu.com/$(RELEASE)/current/$(RELEASE)-server-cloudimg-amd64.img

help:
	@echo 'Target:'
	@echo ' run		- run the $$IMG'
	@echo ' install	- install a new os'
	@echo ' download_cloud_img	- download a cloud image of $$RELEASE from ubuntu.com'
	@echo ' help		- print this message'
	@echo 'Enviroment:'
	@echo ' $$ISO		- install iso file, specify it when you want to install a new os'
	@echo ' $$IMG		- image file, default is the first *.img in current directory'
	@echo ' $$RELEASE	- release name, default is jammy'
	@echo ' $$KERNEL	- kernel file'

.PHONY: run create_img help download_cloud_img
