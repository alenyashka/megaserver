#ifndef MEGASERVER_H
#define MEGASERVER_H

#include <QTcpServer>
#include "data.h"

class MegaServer : public QTcpServer
{
    Q_OBJECT
public:
    MegaServer(QObject *parent = 0);
protected:
    void incomingConnection(int socketDescription);
private:
    Data *data;
};

#endif // MEGASERVER_H
