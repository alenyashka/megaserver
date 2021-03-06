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
    data->lock();
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_5);
    if (nextBlockSize == 0)
    {
        if (bytesAvailable() < sizeof(quint16))
        {
            data->unlock();
            return;
        }
        in >> nextBlockSize;
    }
    if (nextBlockSize == 0xFFFF)
    {
        nextBlockSize = 0;
        data->unlock();
        return;
    }
    if (bytesAvailable() < nextBlockSize)
    {
        data->unlock();
        return;
    }
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
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        Table *table = data->getTable(tableName);
        if (table != NULL)
        {
            out << MegaProtocol::OK;
            write(block);
            QList<Record> records = table->getRecords();
            for (int i = 0; i < records.length(); ++i)
            {
                block.clear();
                out.device()->seek(0);
                out << quint16(0) << records[i].getTitle()
                        << records[i].getComment() << records[i].isReadOnly()
                        << records[i].getType() << records[i].getValue();
                out.device()->seek(0);
                out << quint16(block.size() - sizeof(quint16));
                write(block);
            }
        }
        else
        {
            out << MegaProtocol::ERROR << MegaProtocol::TABLE_DELETED;
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
        if (table != NULL)
        {
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
        else
        {
            out << quint16(0) << MegaProtocol::TABLE_DELETED;
            out.device()->seek(0);
            out << quint16(block.size() - sizeof(quint16));
            write(block);
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
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        Table *t = data->getTable(table);
        if (t != NULL)
        {
            Record *r = t->addRecord(title, comment, readOnly, type, value);
            if (r == NULL)
            {
                out << MegaProtocol::ERROR << MegaProtocol::RECORD_EXIST;
                write(block);
            }
            else
            {
                data->save();
                out << MegaProtocol::OK;
                write(block);
            }
        }
        else
        {
            out << MegaProtocol::ERROR << MegaProtocol::TABLE_DELETED;
            write(block);
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
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);

        Table *t = data->getTable(table);
        if (t != NULL)
        {
            Record *r = t->getRecord(oldTitle);
            if (r != NULL)
            {
                if ((oldTitle != title) && (r->setTitle(title)))
                {
                    out << MegaProtocol::ERROR << MegaProtocol::RECORD_EXIST;
                    write(block);
                }
                else
                {
                    r->setComment(comment);
                    r->setReadOnly(readOnly);
                    r->setType(type);
                    r->setValue(value);
                    data->save();
                    out << MegaProtocol::OK;
                    write(block);
                }
            }
            else
            {
                out << MegaProtocol::ERROR << MegaProtocol::RECORD_DELETED;
                write(block);
            }
        }
        else
        {
            out << MegaProtocol::ERROR << MegaProtocol::TABLE_DELETED;
            write(block);
        }
    }
    if (requestType == MegaProtocol::DEL_RECORD)
    {
        QString name;
        QString title;
        in >> name >> title;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        Table *t = data->getTable(name);
        if (t != NULL)
        {
            Record *r = t->getRecord(title);
            if (r != NULL)
            {
                t->delRecord(title);
                data->save();
                out << MegaProtocol::OK;
            }
            else
            {
                out << MegaProtocol::ERROR << MegaProtocol::RECORD_DELETED;
            }
        }
        else
        {
            out << MegaProtocol::ERROR << MegaProtocol::TABLE_DELETED;
        }
        write(block);
    }
    QDataStream out(this);
    out << quint16(0xFFFF);
    nextBlockSize = 0;
    data->unlock();
    syslog(LOG_DEBUG, "Leave: MegaSocket::readClient");
}
