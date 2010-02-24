#include "megathread.h"
#include "megasocket.h"

MegaThread::MegaThread(int socketDescription, QObject *parent) : QThread(parent)
{
    this->socketDescription = socketDescription;
}

void MegaThread::run()
{
    qDebug() << "Enter: MegaThread::run()";

    MegaSocket megaSocket;
    if (!megaSocket.setSocketDescriptor(socketDescription))
    {
        qDebug() << megaSocket.errorString();
        return;
    }
    megaSocket.waitForDisconnected(-1);
    qDebug() << "Leave: MegaThread::run()";
}
