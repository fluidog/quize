
TARGET ?= $(notdir $(CURDIR))
FILES ?= $(wildcard *.c)

export TARGET FILES

MAKEFLAGS += --no-print-directory

build:
	make -f Kbuild build

clean:
	make -f Kbuild clean

test:
	make -f Kbuild test