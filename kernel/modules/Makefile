export TARGET ?= $(notdir $(CURDIR))
export FILES ?= $(wildcard *.c)


MAKEFLAGS += --no-print-directory


build:
	make -f Kbuild build

clean:
	make -f Kbuild clean

install:
	make -f Kbuild install

test:
	make -f Kbuild test
