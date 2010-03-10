//TODO #pragma one
#ifndef RECORD_H
#define RECORD_H

#include <QtCore>
#include "table.h"
class Table;
class Record
{
public:
    Record(const QString&);
    Record(const QString&, const QString&, const bool&,
           const QVariant::Type&, const QVariant&, Table*);
    QString getTitle() const;
    int setTitle(const QString&);
    QString getComment() const;
    void setComment(const QString&);
    bool isReadOnly() const;
    void setReadOnly(const bool&);
    QVariant getValue() const;
    void setValue (const QVariant&);
    QVariant::Type getType () const;
    void setType (const QVariant::Type&);
    bool operator == (const Record&) const;

private:
    Table *table;
    QString title;
    QString comment;
    bool readOnly;
    QVariant::Type type;
    QVariant value;
};

#endif // RECORD_H
