#include "megathread.h"
#include "megasocket.h"
#include <sys/syslog.h>

MegaThread::MegaThread(int socketDescription, QObject *parent) : QThread(parent)
{
    this->socketDescription = socketDescription;
}

void MegaThread::run()
{
    syslog(LOG_DEBUG, "Enter: MegaThread::run()");

    MegaSocket megaSocket;
    megaSocket.setData(data);
    if (!megaSocket.setSocketDescriptor(socketDescription))
    {
        syslog(LOG_ERR, "%s", megaSocket.errorString().toLatin1().data());
        return;
    }
    QString adr = megaSocket.peerAddress().toString();
    syslog(LOG_INFO, "Client from %s connected", adr.toLatin1().data());
    megaSocket.waitForDisconnected(-1);
    syslog(LOG_INFO, "Client from %s disconnected", adr.toLatin1().data());
    syslog(LOG_DEBUG, "Leave: MegaThread::run()");
}

void MegaThread::setData(Data *data)
{
    this->data = data;
}
