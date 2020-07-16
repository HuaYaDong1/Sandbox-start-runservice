#include "service_2.h"
#include "service.h"
#include "base_dbus_service_control.h"

QHash<QString, int> hash;

service_2::service_2()
{
    QDBusConnection::systemBus().unregisterService("com.client.service");
    if(QDBusConnection::systemBus().registerService("com.client.service"))
    {
        qDebug() << "error:" ;
        exit(-1);
    }

    QDBusConnection::systemBus().registerObject("/citos/client/path",
                                                this,
                                                QDBusConnection :: ExportAllSlots |
                                                QDBusConnection :: ExportAllSignals
                                                );
    QDBusConnection::systemBus().connect(QString(),
                                         QString("/citos/client/path"),
                                         "com.client.test",
                                         "send_to_service",
                                         this,
                                         SLOT(service_get(QString))
                                         );
}

//com.citos.test 服务端----转发给---->com.[local_runtime].test客户端
void service_2::service_get(QString appname_base)
{
    qDebug() << "Message  get from host-client: "<< appname_base;
    int app_in_flag = 0;
    QHash<QString, int>::const_iterator iter1 = hash.constBegin(); //遍历hash

    while(iter1 != hash.constEnd())
    {
        qDebug() <<"hash key:"<<iter1.key();
        if(iter1.key() == appname_base)
        {
            app_in_flag = 1;
            qDebug() <<"hash key:"<<appname_base<<"--------------------------------------------------------------------------------------";
        }
        ++iter1;
    }
    if(app_in_flag)
    {
        qDebug() <<appname_base<<"已经运行";
    }else
    {
        //*--------------------
        //bwrap位置
        Bwrap_run_Thread *bwrap_run_thread = new Bwrap_run_Thread(appname_base);
        bwrap_run_thread->start();

        //*--------------------
        qDebug() << "bwrap 成功";
    }

    QDBusMessage message =QDBusMessage::createSignal("/citos/path", "com.citos.test", "send_to_client");
    message << QString(appname_base);
    QDBusConnection::systemBus().send(message);
}
