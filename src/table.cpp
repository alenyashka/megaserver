#include "table.h"
#include "record.h"

Table::Table(const QString &name, const QString &comment, Data *data)
{
    this->name = name;
    this->comment = comment;
    this->data = data;
}

Table::Table(const QString &name)
{
    this->name = name;
}

int Table::setName(const QString &name)
{
    Table table(name);
    if (data->getTables().contains(table))
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

void Table::addRecord(const QString &title, const QString &comment,
                      const bool &readOnly, const QVariant::Type &type,
                      const QVariant &value)
{
    Record record(title, comment, readOnly, type, value);
    if (!records.contains(record))
    {
        this->records.append(((Record) record));
    }
}

bool Table::delRecord(const QString &title)
{
    return this->records.removeOne(Record(title));
}
