all: client service


client : client.c
	gcc $< `pkg-config --libs --cflags dbus-1` -o $@

service : service.c
	gcc $< `pkg-config --libs --cflags dbus-1` -o $@


clean :
	rm -f *.o service client
