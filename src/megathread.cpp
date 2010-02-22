#include "megathread.h"
#include "megasocket.h"

MegaThread::MegaThread(int socketDescription, QObject *parent) : QThread(parent)
{
    this->socketDescription = socketDescription;
}

void MegaThread::run()
{
    MegaSocket *megaSocket = new MegaSocket();
    megaSocket->setSocketDescriptor(socketDescription);
}
