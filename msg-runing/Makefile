CFLAGS = `pkg-config --cflags --libs gtk+-2.0`

all:  msg-client  msg-service

msg-service:msg-service.c
	gcc $< -o $@ 
	
msg-client:msg-client.c
	gcc $< -o $@ -lsqlite3 

clean:
	rm -f msg-client 
	rm -f msg-service

.PHONY: all clean

install:
	touch /tmp/1
	cp ./msg-service  /usr/bin/
	cp ./kylin-container.service  /usr/lib/systemd/system/
	systemctl start kylin-container.service
	systemctl enable kylin-container.service

