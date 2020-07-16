#include "service_2.h"
#include "service.h"
#include "QDebug"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Service Service1;
    //    std::thread th1(Service1.service_listen1());
    //    th1.detach();
    //    Service1.service_listen1();
   // qDebug()<<"------1-------";


    service_2 service2;
    //    std::thread th2(service2.service_listen2());
    //    th2.detach();
    //    service2.service_listen2();
    qDebug()<<"------2-------";


    return a.exec();
}
