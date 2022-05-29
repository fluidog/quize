
# STATIC_LINK := 1

run: build
	./main

build clean: main

main: main.o libtest
	$(CC) -o $@ $< -L . -ltest

ifdef STATIC_LINK

libtest: libtest.a
libtest.a: obj1.o obj2.o
	ar cr $@ $^

else

CFLAGS := -fPIC
export LD_LIBRARY_PATH := .

libtest: libtest.so
libtest.so: obj1.o obj2.o
	$(CC) -shared -o $@ $^

endif

# Implicit Rules
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $^ *.o *.a *.so