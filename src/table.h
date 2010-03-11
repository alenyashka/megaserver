#ifndef TABLE_H
#define TABLE_H

#include <QtCore>
#include "record.h"
#include "data.h"

class Record;

class Table
{
public:
    Table(const QString&, const QString&);
    Table(const QString&);
    int setName(const QString&);
    QString getName() const;
    void setComment(const QString&);
    QString getComment() const;
    bool operator ==(const Table&) const;
    QList<Record> getRecords() const;
    Record* addRecord(const QString&, const QString&, const bool&,
                      const QVariant::Type&, const QVariant&);
    Record* getRecord(const QString&);
    bool delRecord(const QString&);

private:
    QString name;
    QString comment;
    QList<Record> records;

};

#endif // TABLE_H
