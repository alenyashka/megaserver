#ifndef MEGATHREAD_H
#define MEGATHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "data.h"

static QMutex mutex;

class MegaThread : public QThread
{
    Q_OBJECT
public:
    MegaThread(int socketDescription, QObject *parent = 0);
    void run();
    void setData(Data*);
private:
    int socketDescription;
    Data *data;
};

#endif // MEGATHREAD_H
