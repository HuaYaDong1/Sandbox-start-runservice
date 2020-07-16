#include "client.h"

Client::Client(QString path)
{
    qDebug()<<"----------"<<path<<"-----------";
    //QString PATH  =
    QDBusConnection::systemBus().connect(QString(),
                                          QString("/citos/client/path"),
                                          "com.client.test",
                                          "send_to_client",
                                          this,
                                          SLOT(client_get(QString)));
}

void Client::client_get(QString st)
{
    qDebug() << "Client get!" << st;
}

void client_listen()
{
    while(true)
    {
        char appname[1024];
        scanf("%s",&appname);

        if(strstr(appname, "@") == NULL)
        {
            qDebug()<<"请输入app@platform!";
            continue;
        }

        QDBusMessage message =QDBusMessage::createSignal("/citos/client/path", "com.client.test", "send_to_service");
        message << QString(appname);
        QDBusConnection::systemBus().send(message);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 2)
    {
        qDebug()<< "not input path";
        return 0;
    }

    system("xhost +");

    QString path = QString(QLatin1String(argv[1]));

    qDebug()<<path;

    Client client(path);

    std::thread th1(client_listen);
    th1.detach();

    return a.exec();
}
