#include "app_run_qthread.h"
#include "client_bwrap.h"
#include <QDebug>

App_run_Qthread::App_run_Qthread(QString app, Client *client, QString path):
    appname(app),
    PATCH(path),
    bwrap_client(client)
{
    workCount = 0;
}

void App_run_Qthread::run()
{
    std::string str = appname.toStdString();
    const char* ch = str.c_str();
    puts(ch);
    system(ch);
    app_num--;
    if(!app_num)
    {
        qDebug() <<"-------关闭"<<app_num<<"-------";
        QDBusMessage message =QDBusMessage::createSignal("/citos/path", "com.citos.test", "send_to_service");
        message << QString("2-bwrap死亡:from bwrap_client-"+PATCH);
        QDBusConnection::systemBus().send(message);

        bwrap_client->closeEvent();
    }
}

