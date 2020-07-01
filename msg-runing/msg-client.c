#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <unistd.h>

int find_pid_by_name(char *ProcName)
{
        DIR *dir;
        struct dirent *d;
        int pid;
        char *s;
        int pnlen;
        pnlen = strlen(ProcName); //名字长度
        /* 打开文件夹 /proc */
        dir = opendir("/proc");
        if (!dir)
        {
                return 0;
        }
        /* 遍历文件夹 */
        while ((d = readdir(dir)) != NULL)
        {
                char exe[PATH_MAX + 1];
                char path[PATH_MAX + 1];
                int len;
                int namelen;
                /* 看看是不是一个进程,(名字是不是数字字符) */
                if ((pid = atoi(d->d_name)) == 0)
                        continue;
                snprintf(exe, sizeof(exe), "/proc/%s/exe", d->d_name);
                /*取得符号连接所指的文件，存入path*/
                if ((len = readlink(exe, path, PATH_MAX)) < 0)
                        continue;
                path[len] = '\0';
                s = strrchr(path, '/');
                if (s == NULL)
                        continue;
                s++;
                /* 名字长度不够的不要 */
                namelen = strlen(s);
                if (namelen < pnlen)
                        continue;
                /*判断名字长度是否相等*/
                if (!strncmp(ProcName, s, pnlen))
                {
                        /* 判断名字的最后是不是空格或'\0' */
                        if (s[pnlen] == ' ' || s[pnlen] == '\0')
                        {
                                return 1;
                        }
                }
        }
        closedir(dir);
        return 0;
}

struct mymesg
{
        long int mtype;
        char mtext[512];
};

int msgsend(char *argv)
{
        int id = 0;
        struct mymesg ckxmsg;
        key_t key = ftok("/tmp", 66);
        id = msgget(key, IPC_CREAT | 0666);
        if (id == -1)
        {
                printf("create msg error \n");
                return 0;
        }

        char msg[512];
        memset(msg, 0, sizeof(msg));
        ckxmsg.mtype = 1;
        strcpy(ckxmsg.mtext, argv);

        if (msgsnd(id, (void *)&ckxmsg, 512, 0) < 0)
        {
                printf("send msg error \n");
                return 0;
        }
}

int msgrcvget(char *argv)
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

        if (msgrcv(id, (void *)&ckxmsg, 512, 1, 0) < 0)
        {
                printf("receive msg error \n");
                return 0;
        }
        printf("%s:%s\n", argv, ckxmsg.mtext);

        return 0;
}

void bwrap_run(char *argv)
{
        char str[10240];
        sprintf(str, "bwrap \
                --bind  /opt/%s/bin  /bin \
                --bind  /opt/%s/boot /boot \
                --ro-bind /dev /dev   \
                --bind  /opt/%s/etc /etc \
                --bind  /opt/%s/home  /home \
                --bind  /opt/%s/lib  /lib \
                --bind  /opt/%s/lib64 /lib64\
                --bind  /opt/%s/media /media\
                --bind  /opt/%s/mnt /mnt\
                --bind  /opt/%s/opt  /opt\
                --ro-bind  /proc /proc\
                --bind  /opt/%s/root /root\
                --bind  /run /run\
                --bind  /opt/%s/sbin /sbin\
                --bind  /opt/%s/srv /srv\
                --ro-bind /sys /sys\
                --bind  /tmp /tmp\
                --bind  /opt/%s/usr /usr\
                --bind  /opt/%s/var /var\
                --bind /home /home\
                --bind /etc/passwd  /etc/passwd\
                --bind /etc/group   /etc/group\
                %s ", argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv);
	puts(str);
	system(str);

}

int main(int argc, char *argv[])
{
        if (argc < 2)
        {
                puts("please input appname");
                return 0;
        }
        if (!find_pid_by_name("msg-service"))
        {
                //puts("The server 'msg-service' is not running");
                //return 0;
        }
        msgsend(argv[1]);
        msgrcvget(argv[1]);
	bwrap_run(argv[1]);
        return 0;
}
