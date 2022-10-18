all: build

build:
	touch test

clean:
	rm -f test

install:build
	mkdir -p $(DESTDIR)/usr/bin
	install -p test $(DESTDIR)/usr/bin

include rpm.Makefile
