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
    Data *data = Data::Instance();
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
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_5);
            out << quint16(0) << records[i].getTitle()
                    << records[i].getComment() << records[i].isReadOnly()
                    << records[i].getType() << records[i].getValue();
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
    }
    if (requestType == MegaProtocol::ADD_TABLE)
    {
        QString name;
        QString comment;
        in >> name >> comment;
        Table *table = data->addTable(name, comment);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        if (table == NULL)
        {
            out << quint16(0) << MegaProtocol::TABLE_EXIST;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
        else
        {
            data->save();
        }
    }
    if (requestType == MegaProtocol::EDIT_TABLE)
    {
        QString oldName;
        QString name;
        QString comment;
        in >> oldName >> name >> comment;
        Table *table = data->getTable(oldName);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        if ((oldName != name) && (table->setName(name)))
        {
            out << quint16(0) << MegaProtocol::TABLE_EXIST;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
        else
        {
            table->setComment(comment);
            data->save();
        }
    }
    if (requestType == MegaProtocol::DEL_TABLE)
    {
        QString name;
        in >> name;
        data->delTable(name);
        data->save();
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
    }
    if (requestType == MegaProtocol::ADD_RECORD)
    {
        QString table;
        QString title;
        QString comment;
        bool readOnly;
        QVariant::Type type;
        QVariant value;
        in >> table >> title >> comment >> readOnly >> type >> value;
        Table *t = data->getTable(table);
        Record *r = t->addRecord(title, comment, readOnly, type, value);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        if (r == NULL)
        {
            out << quint16(0) << MegaProtocol::RECORD_EXIST;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
        else
        {
            data->save();
        }
    }
    if (requestType == MegaProtocol::EDIT_RECORD)
    {
        QString table;
        QString title;
        QString oldTitle;
        QString comment;
        bool readOnly;
        QVariant::Type type;
        QVariant value;
        in >> table >> oldTitle >> title >> comment >> readOnly >> type
                >> value;
        Table *t = data->getTable(table);
        Record *r = t->getRecord(oldTitle);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        if ((oldTitle != title) && (r->setTitle(title)))
        {
            out << quint16(0) << MegaProtocol::RECORD_EXIST;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
        else
        {
            r->setComment(comment);
            r->setReadOnly(readOnly);
            r->setType(type);
            r->setValue(value);
            data->save();
        }
    }
    if (requestType == MegaProtocol::DEL_RECORD)
    {
        QString name;
        QString title;
        in >> name >> title;
        Table *t = data->getTable(name);
        Record *r = t->getRecord(title);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        out << quint16(0);

        if (r->isReadOnly())
        {
            out << MegaProtocol::RECORD_IS_READ_ONLY;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
        }
        else
        {
            t->delRecord(title);
            data->save();
        }
    }
    QDataStream out(this);
    out << quint16(0xFFFF);
    close();
    syslog(LOG_DEBUG, "Leave: MegaSocket::readClient");
}
