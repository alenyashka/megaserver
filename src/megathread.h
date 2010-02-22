#ifndef MEGATHREAD_H
#define MEGATHREAD_H

#include <QThread>
#include <QTcpSocket>

class MegaThread : public QThread
{
    Q_OBJECT
public:
    MegaThread(int socketDescription, QObject *parent = 0);
    void run();
private:
    int socketDescription;
};

#endif // MEGATHREAD_H
