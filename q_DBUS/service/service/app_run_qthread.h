#include "app_run_qthread.h"
#include <QDebug>

App_run_Qthread::App_run_Qthread(QString app):
    appname(app)
{
    workCount = 0;
}

void App_run_Qthread::run()
{
    std::string str = appname.toStdString();
    const char* ch = str.c_str();
    system(ch);
}

