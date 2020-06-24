CFLAGS = -g -Wall -Werror

all: shmread  shmwrite  


shmread:shmread.c 
	gcc $< -o $@  
	
shmwrite:shmwrite.c
	gcc $< -o $@  


clean:
	rm -f shmwrite
	rm -f shmread


.PHONY: all clean


install:
	cp ./shmread  /usr/bin/
	cp ./kylin-container.service  /usr/lib/systemd/system/
	systemctl start kylin-container.service
	systemctl enable kylin-container.service

