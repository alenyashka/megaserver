#include "megaserver.h"
#include "megathread.h"
#include <sys/syslog.h>

MegaServer::MegaServer(QObject *parent) : QTcpServer(parent)
{
    readSettings();
    Data::Instance()->setFileName(AppPath::getDataPath(), "data.xml");
}

bool MegaServer::start()
{
    return listen(QHostAddress(host), port);
}

void MegaServer::incomingConnection(int socketDescriptor)
{
    syslog(LOG_DEBUG, "Enter MegaServer::incommingConnetion");
    MegaThread *thread = new MegaThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    syslog(LOG_DEBUG, "Leave MegaServer::incommingConnetion");
}

void MegaServer::readSettings()
{
    QSettings settings(AppPath::getSettingsPath(), QSettings::NativeFormat);
    host = settings.value("Network/host", "127.0.0.1").toString();
    port = settings.value("Network/port", 6178).toInt();
}
