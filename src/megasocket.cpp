#include "megasocket.h"

MegaSocket::MegaSocket(QObject *parent) : QTcpSocket(parent)
{
    nextBlockSize = 0;
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void MegaSocket::readClient()
{
    syslog(LOG_DEBUG, "Enter: MegaSocket::readClient");
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << qint16(0) << QString("Test");
    out.device()->seek(0);
    out << qint16(block.size() - sizeof(qint16));
    write(block);
    out << qint16(0xFFFF);
    close();
    syslog(LOG_DEBUG, "Leave: MegaSocket::readClient");
}
