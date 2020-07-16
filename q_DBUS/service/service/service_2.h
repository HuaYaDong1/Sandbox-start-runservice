#ifndef SERVICE_2_H
#define SERVICE_2_H
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
#include "bwrap_run_thread.h"


extern QHash<QString, int> hash;       //定义hash, key:QString, value:int

class service_2: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.client.test")
public:
    service_2();
public slots:
    void service_get(QString appname_base);
signals:
    void send_to_client(QString st);
};


#endif // SERVICE_2_H
