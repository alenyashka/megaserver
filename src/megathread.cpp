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
    if (!megaSocket.setSocketDescriptor(socketDescription))
    {
        syslog(LOG_ERR, "%s", megaSocket.errorString().toLatin1().data());
        return;
    }
    QString msg;
    msg.append("Client from ").append(megaSocket.peerAddress().toString()).
            append(" connected");
    syslog(LOG_INFO, "%s", msg.toLatin1().data());
    megaSocket.waitForDisconnected(-1);
    syslog(LOG_DEBUG, "Leave: MegaThread::run()");
}
