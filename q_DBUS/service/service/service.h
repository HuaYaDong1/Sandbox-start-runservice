#ifndef SERVICE_H
#define SERVICE_H
#include <QtCore/QObject>
#include <QTextStream>
#include <QCoreApplication>
#include <QtDBus>
#include <QDebug>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <QHash>
#include <QHashIterator>

class Service: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.citos.test")
public:
    Service();
    void hash_del(QString appname_base);
    void umount_app(QString appname_base);
public slots:
    void service_get(QString appname);
signals:
    void send_to_client(QString st);
};

#endif // SERVICE_H
