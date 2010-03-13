#ifndef MEGASERVER_H
#define MEGASERVER_H

#include <QTcpServer>
#include "data.h"
#include "apppath.h"

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
};

#endif // MEGASERVER_H
