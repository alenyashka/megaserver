#ifndef MEGASERVER_H
#define MEGASERVER_H

#include <QTcpServer>
#include "data.h"

class MegaServer : public QTcpServer
{
    Q_OBJECT
public:
    MegaServer(QObject *parent = 0);
    bool start();
protected:
    void incomingConnection(int socketDescription);
private:
    void readSettings();
    QString host;
    qint16 port;
    QString path;
};

#endif // MEGASERVER_H
