#ifndef MEGASOCKET_H
#define MEGASOCKET_H

#include <QTcpSocket>
#include <QHostAddress>
#include <sys/syslog.h>
#include "data.h"

class MegaSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MegaSocket(QObject *parent = 0);
private slots:
    void readClient();
private:
    quint16 nextBlockSize;
};

#endif // MEGASOCKET_H
