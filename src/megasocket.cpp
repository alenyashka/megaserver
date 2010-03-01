#include "megasocket.h"
#include "megaserver.h"
#include "table.h"
#include "megaprotocol.h"

MegaSocket::MegaSocket(QObject *parent) :
    QTcpSocket(parent)
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
        for (int i = 0; i < tables.length(); ++i)
        {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_5);
            out << quint16(0) << tables[i].getName() << tables[i].getComment();
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
    }
    if (requestType == MegaProtocol::GET_RECORDS_LIST)
    {
        QString tableName;
        in >> tableName;
        QList<Record> records = data->getTable(tableName)->getRecords();
        for (int i = 0; i < records.length(); ++i)
        {
            QString readOnly = records[i].isReadOnly() ? "true" : "false";
            QString type = records[i].getType() == QVariant::Double ? "double"
                           : records[i].getType() == QVariant::Int ? "int"
                           : "string";
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_5);
            out << quint16(0) << records[i].getTitle()
                    << records[i].getComment() << readOnly << type
                    << records[i].getValue().toString();
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
    }
    QDataStream out(this);
    out << quint16(0xFFFF);
    close();
    syslog(LOG_DEBUG, "Leave: MegaSocket::readClient");
}

void MegaSocket::setData(Data *data)
{
    this->data = data;
}
