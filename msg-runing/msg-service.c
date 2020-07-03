#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>

char app_name[1024];
char local_name[1024];
char base_name[1024];

struct mymesg
{
        long int mtype;
        char mtext[512];
};

int msgsend()
{
        int id = 0;
        struct mymesg ckxmsg;
        key_t key = ftok("/tmp/1", 66);
        id = msgget(key, IPC_CREAT | 0666);
        if (id == -1)
        {
                printf("create msg error \n");
                return 0;
        }
        char msg[512];
        memset(msg, 0, sizeof(msg));
        ckxmsg.mtype = 1;

        strcpy(ckxmsg.mtext, "挂载成功");
        if (msgsnd(id, (void *)&ckxmsg, 512, 0) < 0)
        {
                printf("send msg error \n");
                return 0;
        }
        return 0;
}

void split(char *src, const char *separator, char **dest)
{
        /*
        src 源字符串的首地址(buf的地址) 
        separator 指定的分割字符
        dest 接收子字符串的数组
        */
        char *pNext;
        int count = 0;
        
        if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止
                return;

        if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止
                return;

        pNext = (char *)strtok(src, separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
        //puts(pNext);
        while (pNext != NULL)
        {
                *dest++ = pNext;
                pNext = (char *)strtok(NULL, separator); //必须使用(char *)进行强制类型转换
        }
}

int main()
{
        char mount[10480];
        char umount[2048];
        char *msg_all[2] = {0};
        int id = 0;
        struct mymesg ckxmsg;
        key_t key = ftok("/tmp", 66);
        id = msgget(key, 0666 | IPC_CREAT);
        if (id == -1)
        {
                printf("open msg error \n");
                return 0;
        }
        while (1)
        {
                if (msgrcv(id, (void *)&ckxmsg, 512, 1, 0) < 0)
                {
                        printf("receive msg error \n");
                        return 0;
                }

                split(ckxmsg.mtext, "-", msg_all);//分割传过来的字符
                
                strcpy(app_name, msg_all[0]);
                strcpy(local_name, msg_all[1]);
                strcpy(base_name, msg_all[2]);
                puts(app_name);puts(local_name);puts(base_name);

                //ckxmsg.mtext[strlen(ckxmsg.mtext) - 1] = 0;//删除末尾字符

                memset(umount, 0, sizeof(umount));
                memset(mount, 0, sizeof(mount));

                sprintf(umount, "umount /opt/%s", ckxmsg.mtext);
                system(umount);
                // puts(umount);
                syslog(LOG_DEBUG, "msg-service----------------------: '%s'\n", umount);

                sprintf(mount, "mount -t aufs -o  dirs=/opt/%s=rw:%s=ro:%s=ro none /opt/%s", app_name,  local_name,  base_name,  app_name);
                system(mount);
                // puts(mount);
                syslog(LOG_DEBUG, "msg-service----------------------: '%s'\n", mount);

                msgsend(); //挂载完成后通知客户端。
        }
        if (msgctl(id, IPC_RMID, NULL) < 0) //删除接收信息msg
        {
                printf("del msg error \n");
                return 0;
        }
        return 0;
}