#include "table.h"
#include "record.h"

Table::Table(const QString &name, const QString &comment)
{
    this->name = name;
    this->comment = comment;
}

Table::Table(const QString &name)
{
    this->name = name;
}

int Table::setName(const QString &name)
{
    Table table(name);
    if (Data::Instance()->getTables().contains(table))
    {
        return 1;
    }
    this->name = name;
    return 0;
}

QString Table::getName() const
{
    return this->name;
}

void Table::setComment(const QString &comment)
{
    this->comment = comment;
}

QString Table::getComment() const
{
    return this->comment;
}

bool Table::operator ==(const Table &table) const
{
    return name == table.getName();
}

QList<Record> Table::getRecords() const
{
    return this->records;
}

Record* Table::addRecord(const QString &title, const QString &comment,
                         const bool &readOnly, const QVariant::Type &type,
                         const QVariant &value)
{
    Record record(title, comment, readOnly, type, value, this);
    if (!records.contains(record))
    {
        this->records.append(((Record) record));
        return &records[records.indexOf(record)];
    }
    else
    {
        return NULL;
    }
}

Record* Table::getRecord(const QString &title)
{
    int index = records.indexOf(Record(title));
    if (index == -1) return NULL;
    return &records[index];
}

bool Table::delRecord(const QString &title)
{
    return this->records.removeOne(Record(title));
}
