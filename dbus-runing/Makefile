
CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-2.0)
CFLAGS_GUNIX  = $(shell pkg-config --cflags --libs gio-unix-2.0)

CFLAGS = -g -Wall -Werror

all: kylin-runing-server  kylin-runing-client


kylin-runing-server: kylin-runing-server.c
	gcc $< -o $@ $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_GIO) $(CFLAGS_GUNIX)
	
kylin-runing-client: kylin-runing-client.c
	gcc $< -o $@ $(CFLAGS) $(CFLAGS_GIO)


clean:
	rm -f kylin-runing-server
	rm -f kylin-runing-client


.PHONY: all clean
