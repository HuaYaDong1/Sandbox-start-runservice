#include<stdio.h>
#include<syslog.h> 
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>

struct mymesg{
        long int mtype;
        char mtext[512];
};

int msgsend()
{
        int id = 0;
        struct mymesg ckxmsg;
        key_t key = ftok("/tmp/1",66);
        id = msgget(key,IPC_CREAT | 0666);
        if(id == -1)
        {
                printf("create msg error \n");
                return 0;
        }
                char msg[512];
                memset(msg,0,sizeof(msg));
                ckxmsg.mtype = 1;

		strcpy(ckxmsg.mtext,"挂载成功");
                if(msgsnd(id,(void *)&ckxmsg,512,0) < 0)
                {
                        printf("send msg error \n");
                        return 0;
                }
        return 0;
}

int main()
{
	char mount[1024];
        char umount[1024];
	int id = 0;
	struct mymesg ckxmsg;
	key_t key = ftok("/tmp",66);
	id = msgget(key,0666|IPC_CREAT);
	if(id == -1)
	{
		printf("open msg error \n");
		return 0;
	}

	while(1)
	{
		if(msgrcv(id,(void *)&ckxmsg,512,1,0) < 0)
		{
			printf("receive msg error \n");
			return 0;
		}
		printf("data:%s\n",ckxmsg.mtext);

		ckxmsg.mtext[strlen(ckxmsg.mtext)-1]=0;
                
		memset(umount, 0, sizeof(umount));
                memset(mount, 0, sizeof(mount));
                
		sprintf(umount, "umount /opt/%s",ckxmsg.mtext);
                system(umount);
               // puts(umount);
		syslog(LOG_DEBUG, "msg-service------------------------------: '%s'\n",  umount); 

                sprintf(mount, "mount -t aufs -o  dirs=/opt/%s=rw:/opt/app_runtime=ro:/opt/base_runtime=ro none /opt/%s", ckxmsg.mtext, ckxmsg.mtext);
                system(mount);
               // puts(mount);
		syslog(LOG_DEBUG, "msg-service------------------------------: '%s'\n",  mount);

		msgsend();

	}
        if(msgctl(id,IPC_RMID,NULL) < 0)//删除接收信息msg
        {
                printf("del msg error \n");
                return 0;
        }
	return 0;
}
