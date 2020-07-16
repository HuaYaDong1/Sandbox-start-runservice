#ifndef CLIENT_H
#define CLIENT_H
#include <QtCore/QObject>
#include <QTextStream>
#include <QCoreApplication>
#include <QtDBus>
#include <QDebug>
#include <thread>
#include <stdlib.h>
#include <QPaintEvent>
#include <QCloseEvent>

extern int app_num;

class Client: public QObject
{
    Q_OBJECT
public:
    Client(QString path);
    QString PATH;

    void closeEvent();

public slots:
    void client_get(QString st);
signals:
    void send_to_service(QString st);
};

#endif // CLIENT_H
