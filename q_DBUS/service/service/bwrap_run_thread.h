#ifndef BWRAP_RUN_THREAD_H
#define BWRAP_RUN_THREAD_H

#include <QThread>

class Bwrap_run_Thread: public QThread
{
public:
    Bwrap_run_Thread(QString appname);
    ~Bwrap_run_Thread();

private:
    virtual void run();//任务处理线程
    void stop();
    bool mStop;
    QString appname_base;
    void bwrap_run(QString appname);
public:
    int workCount;//计数

signals:

public slots:
};

#endif // BWRAP_RUN_THREAD_H
