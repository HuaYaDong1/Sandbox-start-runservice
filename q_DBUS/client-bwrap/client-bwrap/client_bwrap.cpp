#include "client_bwrap.h"
#include "app_run_qthread.h"

int app_num = 0;

Client::Client(QString path)
    :PATH(path)
{
    qDebug()<<"----------"<<PATH<<"-----------";

    //path  = "/citos/path/"+ path;
    QDBusConnection::systemBus().connect(QString(),
                                         QString("/citos/path"),
                                         "com.citos.test",
                                         "send_to_client",
                                         this,
                                         SLOT(client_get(QString))
                                         );

  //---------------------------------首次启动应用----------------------------------------------------
    QStringList list = path.split("@");

    QDBusMessage message =QDBusMessage::createSignal("/citos/path", "com.citos.test", "send_to_service");
    message << QString("3-bwrap_成功：from bwrap_client-"+PATH);
    QDBusConnection::systemBus().send(message);

    app_num++;
    App_run_Qthread *task = new App_run_Qthread(list.at(0), this , path); //在线程中启动应用
    task->start();
  //-----------------------------------------------------------------------------------------------
}


void Client::client_get(QString st)
{
    /*
        st 总服务器发来的app + PATH
        pluma-v10-1
    */
    QStringList list = st.split("@");
    QDBusMessage message =QDBusMessage::createSignal("/citos/path", "com.citos.test", "send_to_service");

    if(st.compare(PATH) != 0 )
    {
        qDebug()<<"--------"<<st<<"--------";
        message << QString("4-app_错误：from bwrap_client:"+PATH);
        QDBusConnection::systemBus().send(message);
        return ;
    }else{

        App_run_Qthread *task = new App_run_Qthread(list.at(0), this, st); //在线程中启动应用
        task->start();
        app_num++;

        message << QString("1-app_成功：from bwrap_client-"+PATH);
        QDBusConnection::systemBus().send(message);
    }
}

void Client::closeEvent()
{
    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //        if (argc < 2)
    //        {
    //            qDebug()<< "not input path";
    //            return 0;
    //        }

    QString path = QString(QLatin1String(argv[1]));
    Client client(path);
    return a.exec();
}
