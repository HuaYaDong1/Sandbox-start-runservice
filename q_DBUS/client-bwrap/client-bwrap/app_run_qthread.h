#ifndef APP_RUN_QTHREAD_H
#define APP_RUN_QTHREAD_H

#include <QThread>

class Client;
class App_run_Qthread : public QThread
{
public:
    App_run_Qthread(QString app, Client *client, QString path);

private:
    Client *bwrap_client;
    QString appname;
    QString PATCH;
    virtual void run();//任务处理线程

public:
    int workCount;//计数

signals:

public slots:
};
#endif // APP_RUN_QTHREAD_H

