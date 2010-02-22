#ifndef MEGASOCKET_H
#define MEGASOCKET_H

#include <QTcpSocket>

class MegaSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MegaSocket(QObject *parent = 0);
private slots:
    void readClient();
private:
    int nextBlockSize;
};

#endif // MEGASOCKET_H
