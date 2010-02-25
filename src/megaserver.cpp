#include "megaserver.h"
#include "megathread.h"
#include <sys/syslog.h>

MegaServer::MegaServer(QObject *parent) : QTcpServer(parent)
{
    data = new Data("data.xml");
}

void MegaServer::incomingConnection(int socketDescriptor)
{
    syslog(LOG_DEBUG, "Enter MegaServer::incommingConnetion");
    MegaThread *thread = new MegaThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    syslog(LOG_DEBUG, "Leave MegaServer::incommingConnetion");
}
