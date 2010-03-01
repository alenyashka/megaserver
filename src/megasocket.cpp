#include "megasocket.h"
#include "megaserver.h"
#include "table.h"
#include "megaprotocol.h"

MegaSocket::MegaSocket(QObject *parent) : QTcpSocket(parent)
{
    nextBlockSize = 0;
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void MegaSocket::readClient()
{
    syslog(LOG_DEBUG, "Enter: MegaSocket::readClient");
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_5);
    if (nextBlockSize == 0)
    {
        if (bytesAvailable() < sizeof(quint16)) return;
        in >> nextBlockSize;
    }
    if (bytesAvailable() < nextBlockSize) return;
    int requestType;
    in >> requestType;
    if (requestType == MegaProtocol::GET_TABLES_LIST)
    {
        QList<Table> tables = data->getTables();
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        for (int i = 0; i < tables.length(); ++i)
        {
            out << quint16(0);
            out << tables[i].getName() << tables[i].getComment();
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            out.device()->seek(0);
            write(block);
        }
        out << quint16(0xFFFF);
        write(block);
    }
    close();
    syslog(LOG_DEBUG, "Leave: MegaSocket::readClient");
}

void MegaSocket::setData(Data *data)
{
    this->data = data;
}
