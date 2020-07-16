#include "bwrap_run_thread.h"

#define LOCAL "/opt/app_runtime"
#define BASE  "/opt/base_runtime"

Bwrap_run_Thread::Bwrap_run_Thread(QString appname):
    appname_base(appname)
{
    workCount = 0;
}

Bwrap_run_Thread::~Bwrap_run_Thread()
{
    //stop();
}

void Bwrap_run_Thread::run()
{
    char mount[10480];
    char umount[2048];

    QStringList list = appname_base.split("@");

    QString app_name=list.at(0);
    QString base_name=list.at(1);


    std::string str_app = app_name.toStdString();
    std::string str_base = base_name.toStdString();

    const char* ch_app_name = str_app.c_str();
    //const char* ch_base_name = str_base.c_str();//base  目前写死------------------


    sprintf(umount, "umount /opt/%s", ch_app_name);
    system(umount);
    puts(umount);

    sprintf(mount, "mount -n -t overlay overlayfs:/overlay -o lowerdir=%s:%s,upperdir=/opt/%s,workdir=/opt/worker /opt/%s",LOCAL,BASE,ch_app_name,ch_app_name);
    system(mount);
    puts(mount);

    bwrap_run(ch_app_name);

    return ;
}

void Bwrap_run_Thread::bwrap_run(QString appname)
{
    std::string appname_1 = appname.toStdString();
    const char* app = appname_1.c_str();

    std::string appname_base_1 = appname_base.toStdString();
    const char* app_base = appname_base_1.c_str();

    char str[10240];
    sprintf(str, "bwrap \
            --bind  /opt/%s/bin  /bin \
            --bind  /opt/%s/boot /boot \
            --ro-bind /dev /dev   \
            --bind  /opt/%s/etc /etc \
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
            --setenv PATH /bin:/usr/bin:/usr/games:/bin/sbin \
            --setenv DISPLAY :0 \
            --bind /etc/passwd  /etc/passwd\
            --bind /etc/group   /etc/group\
            /usr/bin/client_bwrap %s",
            app, app, app, app, app, app, app, app, app, app, app, app, app, app_base);
    puts(str);
    system(str);
}
