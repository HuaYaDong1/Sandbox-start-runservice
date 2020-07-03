#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <unistd.h>

char app_name[1024];
char local_name[1024];
char base_name[1024];

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
                --proc  /proc\
                --bind  /opt/%s/root /root\
                --bind  /run /run\
                --bind  /opt/%s/sbin /sbin\
                --bind  /opt/%s/srv /srv\
                --ro-bind /sys /sys\
                --bind  /tmp /tmp\
                --bind  /opt/%s/usr /usr\
                --bind  /opt/%s/var /var\
                --bind /home /home\
                --unshare-pid\
                --bind /etc/passwd  /etc/passwd\
                --bind /etc/group   /etc/group\
                %s ",
                argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv, argv);
        //puts(str);
        system(str);
}

int main(int argc, char *argv[])
{
        if (!find_pid_by_name("msg-service"))
        {
                //puts("The server 'msg-service' is not running");
                //return 0;
        }

        char *appmsg_all[1] = {0};
        char msg_all[10240];
        memset(msg_all, 0, sizeof(msg_all));
        memset(app_name, 0, sizeof(app_name));
        memset(local_name, 0, sizeof(local_name));
        memset(base_name, 0, sizeof(base_name));

        if (argc < 2 || (!strcmp("--help", argv[1])) || (!strcmp("-h", argv[1])))
        {
                puts("-h    or  --help                              打印帮助(本信息)并退出.");
                puts("-l    or  --list                              打印以安装应用信息列表.");
                puts("-d    app_name                                默认地址挂载运行.");
                puts("-b    app_name@platform_alias                 指定base_runtime平台 挂载运行.");
                puts("-c    appName     localdir                    指定local_runtimer平台挂载运行.");
                puts("-p    app_name@platform_alias     localdir    指定base_runtime平台，local_runtimer平台 挂载运行.");
                return 0;
        }
        else if ((!strcmp("-l", argv[1])) || (!strcmp("--list", argv[1])))
        {
                puts("---------------");
                return 0;
        }
        else if (!strcmp("-d", argv[1]))
        {
                if (argc != 3)
                {
                        puts("please input : app_name");
                        return 0;
                }
                strcpy(app_name, argv[2]);
                strcpy(local_name, "/opt/app_runtime");
                strcpy(base_name, "/opt/base_runtime");
        }
        else if (!strcmp("-b", argv[1]))
        {
                if (argc != 3)
                {
                        puts("please input : app_name@platform_alias");
                        return 0;
                }
                split(argv[2], "@", appmsg_all); //分割app_name@platform_alias字符

                puts(appmsg_all[0]);
                puts(appmsg_all[1]);

                strcpy(app_name, appmsg_all[0]);
                strcpy(local_name, "/opt/app_runtime"); //默认local_runtime
                strcpy(base_name, appmsg_all[1]);
        }
        else if (!strcmp("-c", argv[1]))
        {
                if (argc != 4)
                {
                        puts("please input : appName  localdir");
                        return 0;
                }
                strcpy(app_name, argv[2]);
                strcpy(local_name, argv[3]);
                strcpy(base_name, "/opt/base_runtime"); //默认base_runtime
        }
        else if (!strcmp("-p", argv[1]))
        {
                if (argc != 4)
                {
                        puts("please input ： app_name@platform_alias  localdir");
                        return 0;
                }
                split(argv[2], "@", appmsg_all); //分割app_name@platform_alias字符

                puts(appmsg_all[0]);
                puts(appmsg_all[1]);

                strcpy(app_name, appmsg_all[0]);
                strcpy(local_name, argv[3]);
                strcpy(base_name, appmsg_all[1]);
        }
        else
        {
                puts("输入错误  -h or --help");
                return 0;
        }

        puts(app_name);
        puts(local_name);
        puts(base_name);
        sprintf(msg_all, "%s-%s-%s", app_name, local_name, base_name);

        puts(msg_all);

        msgsend(msg_all);

        msgrcvget(app_name); //接收返回信息

        bwrap_run(app_name);

        return 0;
}