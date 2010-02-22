#include "megaserver.h"
#include "megathread.h"

MegaServer::MegaServer(QObject *parent) : QTcpServer(parent)
{
    data = new Data("data.xml");
}

void MegaServer::incomingConnection(int socketDescriptor)
{
    qDebug() << "Enter MegaServer::incommingConnetion" << endl;
    //TODO Logging client conection
    MegaThread *thread = new MegaThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    qDebug() << "Leave MegaServer::incommingConnetion" << endl;
}
