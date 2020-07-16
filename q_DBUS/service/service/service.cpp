#include "service.h"
#include "service_2.h"

Service::Service()
{
    QDBusConnection::systemBus().unregisterService("com.citos.service");
    if (QDBusConnection::systemBus().registerService("com.citos.service"))
    {
        qDebug() << "error:" ;
        exit(-1);
    }
    QDBusConnection::systemBus().registerObject("/citos/path", this,QDBusConnection :: ExportAllSlots | QDBusConnection :: ExportAllSignals);
    QDBusConnection::systemBus().connect(QString(),
                                         QString("/citos/path"),
                                         "com.citos.test",
                                         "send_to_service",
                                         this,
                                         SLOT(service_get(QString)));
}

void Service::service_get(QString st)
{
    qDebug()<<"~~~~~~~~~~~~~~~~~~~"<<st<<"~~~~~~~~~~~~~~~~~~~";
    QStringList list = st.split("-");
    QString aaa=list.at(0);
    if(aaa.compare("2") == 0 )
    {
        hash_del(list.at(2));
        qDebug() <<st<<"hash删除";
        QString umount =list.at(2);
        umount_app(umount);
    }
    else if(aaa.compare("3") == 0 )
    {
        hash.insert(list.at(2), 1);         //插入数据，方法2： value: <"two",2>
        qDebug() <<st<<"hash增加";
        qDebug() <<list.at(2)<<"app启动成功";
    }
    else if(aaa.compare("1") == 0 )
    {
        qDebug() <<st<<"app启动成功";
    }
    else
    {
        qDebug() <<"      *******************"<<st<<"*******************";
    }
}

//删除表值
void Service::hash_del(QString appname_base)
{
    hash.remove(appname_base);
}

//结束bwrap, umount文件夹
void Service::umount_app(QString appname_base)
{
    QStringList list = appname_base.split("@");
    QString umount = "umount /opt/"+list.at(0);
    std::string appname = umount.toStdString();
    const char* app_umount = appname.c_str();
    puts(app_umount);
    system(app_umount);
}
