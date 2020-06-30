CFLAGS = -g -Wall -Werror

all:  msg-client  msg-service


msg-service:msg-service.c
	gcc $< -o $@  
	
msg-client:msg-client.c
	gcc $< -o $@  


clean:
	rm -f msg-client
	rm -f msg-service


.PHONY: all clean


install:
	cp ./msg-service  /usr/bin/
	cp ./kylin-container.service  /usr/lib/systemd/system/
	systemctl start kylin-container.service
	systemctl enable kylin-container.service

